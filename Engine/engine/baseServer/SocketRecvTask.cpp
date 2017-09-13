#include "engine/def/environment_def.h"
#include "engine/baseServer/SocketRecvTask.h"
#include "../net/socket/WinSocket.h"
#include "framework/log/Log.h"
#include "framework/util/typetransform.h"
#include "framework/util/sysUtil.h"
#include "../net/socket/socketManager.h"
#include "engine/net/socket/LinuxSocket.h"

using namespace csg;

static const int DEFAULT_BACK_UP_NUM = 10;
static const int DEFAULT_MAX_CONNECT_NUM = 10000;

csg::CSocketRecvTask::~CSocketRecvTask()
{
#ifdef CSG_WIN
	CWinSocket::close(_socketfd);
	CWinSocket::clearWinSocket();
#elif defined(CSG_LINUX)
	CLinuxSocket::closeSocket(_socketfd);
#endif 


}

int csg::CSocketRecvTask::init(int family ,int type ,int protocol ,int port ,std::string host ,bool isInnner)
{
	_family = family;
	_type = type;
	_protocol = protocol;
	_port = port;
	_innerPort = isInnner;
	_host = host;
	return CSG_RETURN_OK;
}

int csg::CSocketRecvTask::startListen()
{
	_isClient = false;
	int result = -1;
#ifdef CSG_WIN
	 result=initWinSocket();
#elif defined(CSG_LINUX)
	 result=initLinuxSocket();
#endif // CSG_WIN
	 if ( -1 == result )
	 {
		 return CSG_RETURN_ERROR;
	 }

	csg_thread recvThread(CSocketRecvTask::svc_run ,this);
	recvThread.detach();

	return CSG_RETURN_OK;
}

int csg::CSocketRecvTask::startConnect()
{
	_isClient = true;
	_socketfd = CSocketHelper::getClientSocket(_family ,_type ,_protocol ,_port ,_host);
	if ( -1 == _socketfd )
	{
		CSG_LOG_DEBUG("tcsg::CSocketRecvTask::startConnect failed");
		return -1;
	}
	csg_thread recvThread(CSocketRecvTask::svc_run ,this);
	recvThread.detach();
	return _socketfd;
}

int csg::CSocketRecvTask::svc_run(void* task)
{
	CSocketRecvTask *t = (CSocketRecvTask*) task;
	if ( !t )
	{
		assert(false);
		return CSG_RETURN_ERROR;
	}
	t->run();
	return CSG_RETURN_OK;
}

int csg::CSocketRecvTask::run()
{

	if ( _isClient )
	{
		doRecvLoop();
	} else
	{
		CSG_LOG_SYS("CSocketRecvTask::run listen start,port=" << ToStr(_port));
#ifdef CSG_WIN
		doWinLoop();
#elif defined(CSG_LINUX)
		doLinuxLoop();
#endif
		CSG_LOG_SYS("CSocketRecvTask::run exit");
	}
	return 0;
}





void csg::CSocketRecvTask::doRecvLoop()
{
	CSG_LOG_SYS("CSocketRecvTask::doRecvLoop,ip=" <<_host<<",port="<<ToStr(_port));
	std::unique_lock<CLock> lock(getLock());
	const int SOCKET_CLIENT_BUFFER_MAX_SIZE = 1024 * 2;
	char buffer[SOCKET_CLIENT_BUFFER_MAX_SIZE];
	while ( !isExit() )
	{
		if ( isStop() )
		{
			CSG_LOG_SYS("CSocketRecvTask::doRecvLoop waiting");
			_cv.wait(lock);
		}
		memset(buffer ,0,SOCKET_CLIENT_BUFFER_MAX_SIZE);
		int readRecvSize = 0;
#ifdef CSG_WIN
		readRecvSize = recv(_socketfd ,buffer ,SOCKET_CLIENT_BUFFER_MAX_SIZE ,0);
#elif defined(CSG_LINUX)
		readRecvSize=CLinuxSocket::Recv(_socketfd,buffer,SOCKET_CLIENT_BUFFER_MAX_SIZE,0);
#endif
		if ( readRecvSize > 0 )
		{
			CSocketHelper::onRecvMsg(_socketfd ,buffer,readRecvSize);
		}
		CThread::sleep_for(100);
	}
	CSG_LOG_SYS("CSocketRecvTask::run exit ip = " <<_host<<" ,port = "<<ToStr(_port));
}

#ifdef CSG_WIN
DWORD WINAPI csg::CSocketRecvTask::workerThreadRun(LPVOID IpParam)
{
	HANDLE CompletionPort = (HANDLE) IpParam;

	while ( true )
	{
		DWORD BytesTransferred;
		LPOVERLAPPED IpOverlapped;
		CSG_SOCKET_DATA* PerHandleData = NULL;
		CSG_SOCKET_IO_DATA* PerIoData = NULL;
		DWORD Flags = 0;
		BOOL bRet = false;
		bRet = CWinSocket::getQueuedCompletionStatus(CompletionPort ,&BytesTransferred ,(PULONG_PTR) &PerHandleData ,(LPOVERLAPPED*) &IpOverlapped ,INFINITE);//此处可以将IpOverlapped换为PerIoData，然后将下面CONTAINING_RECORD注释掉
		if ( bRet == 0 )
		{
			int errorCode = CSysUtil::getLastError();
			CSG_RECORD_ERROR(" GetQueuedCompletionStatus");
			CSocketHelper::onDisconnected(PerHandleData->socketfd ,errorCode);
			continue;
		}
		PerIoData = (CSG_SOCKET_IO_DATA*) CONTAINING_RECORD(IpOverlapped ,CSG_SOCKET_IO_DATA ,overlapped);
		//这个宏的作用是：根据一个结构体实例中的成员的地址，取到整个结构体实例的地址
		//PER_IO_DATA的成员overlapped的地址为&IpOverlapped，结果就可以获得PER_IO_DATA的地址

		// 检查在套接字上是否有错误发生
		if ( 0 == BytesTransferred )
		{
			CWinSocket::close(PerHandleData->socketfd);
			free(PerHandleData);
			free(PerIoData);
			continue;
		}
 		
		CSocketHelper::onRecvMsg(PerHandleData->socketfd ,PerIoData->buffer ,BytesTransferred);

		// 为下一个重叠调用建立单I/O操作数据
		ZeroMemory(&( PerIoData->overlapped ) ,sizeof(OVERLAPPED)); // 清空内存
		PerIoData->databuff.len = DATA_BUFF_SOCKET_SIZE;
		PerIoData->databuff.buf = PerIoData->buffer;//buf是个指针，这一过程会清空buffer的内容
		PerIoData->operationType = 0;   // read
		DWORD RecvBytes;
		CWinSocket::recv(PerHandleData->socketfd ,&( PerIoData->databuff ) ,1 ,&RecvBytes ,&Flags ,&( PerIoData->overlapped ) ,NULL);
	}
}

int csg::CSocketRecvTask::initWinSocket()
{
	CWinSocket::InitSocket();
	_completionPort = CSocketManager::instance()->getIOCPPort();
	_socketfd = CWinSocket::getSocket(_family ,_type ,_protocol);

	if ( -1 == _socketfd )
	{
		CSG_RECORD_ERROR("CSocketRecvTask::init listen failed");
		return CSG_RETURN_ERROR;
	}

	int bindResult = CWinSocket::bind(_socketfd ,_family ,_port ,INADDR_ANY);

	if ( -1 == bindResult )
	{
		CSG_RECORD_ERROR("CSocketRecvTask::init bind failed");
		return CSG_RETURN_ERROR;
	}

	int listenResult = CWinSocket::listen(_socketfd ,DEFAULT_BACK_UP_NUM);
	if ( -1 == listenResult )
	{
		CSG_RECORD_ERROR("CSocketRecvTask::init listen failed");
		return CSG_RETURN_ERROR;
	}
	return CSG_RETURN_OK;
}

void csg::CSocketRecvTask::doWinLoop()
{
	std::unique_lock<CLock> lock(getLock());
	while ( !isExit() )
	{
		if ( isStop() )
		{
			CSG_LOG_SYS("CSocketRecvTask::run waiting");
			_cv.wait(lock);
		}
		sockaddr saRemote;
		int RemoteLen = sizeof( saRemote );
		int acceptSocket = CWinSocket::accept(_socketfd ,(sockaddr*) &saRemote ,&RemoteLen);
		if ( SOCKET_ERROR == acceptSocket )
		{
			CSG_RECORD_ERROR("CSocketRecvTask::run accept failed!");
			assert(false);
			continue;
		}
		// 创建用来和套接字关联的单句柄数据信息结构
		CSG_SOCKET_DATA * PerHandleData = new CSG_SOCKET_DATA;
		PerHandleData->socketfd = acceptSocket;
		memcpy(&PerHandleData->clientAddr ,&saRemote ,RemoteLen);


		// 将接受套接字和完成端口关联
		CWinSocket::createIOCPPort((HANDLE) ( PerHandleData->socketfd ) ,_completionPort ,(DWORD) PerHandleData ,0);


		// 开始在接受套接字上处理I/O使用重叠I/O机制
		// 在新建的套接字上投递一个或多个异步
		// WSARecv或WSASend请求，这些I/O请求完成后，工作者线程会为I/O请求提供服务    
		// 单I/O操作数据(I/O重叠)
		CSG_SOCKET_IO_DATA* PerIoData = new CSG_SOCKET_IO_DATA;
		ZeroMemory(&( PerIoData->overlapped ) ,sizeof( OVERLAPPED ));
		PerIoData->databuff.len = DATA_BUFF_SOCKET_SIZE;
		PerIoData->databuff.buf = PerIoData->buffer;
		PerIoData->operationType = 0;   // read;

		DWORD RecvBytes;
		DWORD Flags = 0;  //WSARecv中的1，代表缓冲区lpBuffers只包含一个WSABUF，Flags代表接收普通数据
		CWinSocket::recv(PerHandleData->socketfd ,&( PerIoData->databuff ) ,1 ,&RecvBytes ,&Flags ,&( PerIoData->overlapped ) ,NULL);//PerIoData->overlapped就是CONTAINING_RECORD的第一个变量

		std::string remoteAddr(inet_ntoa(PerHandleData->clientAddr.sin_addr));
		int remotePort = ntohs(PerHandleData->clientAddr.sin_port);

		CSocketHelper::onConnected(PerHandleData->socketfd ,remoteAddr ,remotePort ,_innerPort);
	}
}


#elif defined(CSG_LINUX)

int csg::CSocketRecvTask::initLinuxSocket()
{
	_socketfd = CLinuxSocket::getSocket(_family ,_type ,_protocol);

	if ( -1 == _socketfd )
	{
		CSG_RECORD_ERROR("CSocketRecvTask::init listen failed");
		return CSG_RETURN_ERROR;
	}

	struct sockaddr_in serveraddr;
	serveraddr.sin_family = _family;
	serveraddr.sin_addr.s_addr = INADDR_ANY;
	serveraddr.sin_port = htons(_port);
	memset(&( serveraddr.sin_zero ) ,'\0' ,8);

	int bindResult = CLinuxSocket::Bind(_socketfd ,( struct sockaddr * )&serveraddr ,sizeof(serveraddr));

	if ( -1 == bindResult )
	{
		CSG_RECORD_ERROR("CSocketRecvTask::init bind failed");
		return CSG_RETURN_ERROR;
	}

	int listenResult = CLinuxSocket::Listen(_socketfd ,DEFAULT_BACK_UP_NUM);
	if ( -1 == listenResult )
	{
		CSG_RECORD_ERROR("CSocketRecvTask::init listen failed");
		return CSG_RETURN_ERROR;
	}

	_epollfd = -1;

	_events = (epoll_event*) calloc(DEFAULT_MAX_CONNECT_NUM ,sizeof(struct epoll_event));

	if ( ( _epollfd = CLinuxSocket::Epoll_create(DEFAULT_MAX_CONNECT_NUM) ) == -1 )
	{
		CSG_RECORD_ERROR("CSocketRecvTask::init epoll create failed");
		return CSG_RETURN_ERROR;
	}

	int fdmax=_socketfd;

	_ev.events = EPOLLIN;
	_ev.data.fd = fdmax;

	if ( CLinuxSocket::Epoll_ctl(_epollfd ,EPOLL_CTL_ADD ,fdmax ,&_ev)<0 )
	{
		CSG_RECORD_ERROR("CSocketRecvTask::init epoll_ctl create failed");
		return CSG_RETURN_ERROR;
	}
	return CSG_RETURN_OK;
}

void csg::CSocketRecvTask::doLinuxLoop(){

	std::unique_lock<CLock> lock(getLock());
	while ( !isExit() )
	{
		if ( isStop() )
		{
			CSG_LOG_SYS("CSocketRecvTask::run waiting");
			_cv.wait(lock);
		}

		int res = CLinuxSocket::Epoll_wait(_epollfd ,_events ,DEFAULT_MAX_CONNECT_NUM ,-1);
		for (int index = 0; index < res; index++ )
		{
			int client_fd = _events[index].data.fd;//get the first events

			if ( client_fd == _socketfd )
			{//new connections
				struct sockaddr_in clientaddr;
				socklen_t addrlen = sizeof( clientaddr );
				int newfd = CLinuxSocket::Accept(_socketfd ,( struct sockaddr * )&clientaddr ,&addrlen);
				if ( -1 != newfd )
				{
					_ev.events = EPOLLIN;
					_ev.data.fd = newfd;
					if ( CLinuxSocket::Epoll_ctl(_epollfd ,EPOLL_CTL_ADD ,newfd ,&_ev) < 0 )
					{
						CSG_LOG_DEBUG("CSocketBase::Epoll_ctl faild when add new sockfd");
						continue;
					} else
					{
						CSG_LOG_DEBUG("CSocketBase::add new sockfd socketfd="<<ToStr(newfd));
						std::string remoteAddr(inet_ntoa(clientaddr.sin_addr));
						int remotePort = ntohs(clientaddr.sin_port);
						CSocketHelper::onConnected(newfd ,remoteAddr ,remotePort ,_innerPort);
					}
				}
				continue;
			} else
			{
				if ( _events[index].events&EPOLLHUP )
				{
					int result=CLinuxSocket::Epoll_ctl(_epollfd ,EPOLL_CTL_DEL ,client_fd ,&_ev);
					int lastError=CSysUtil::getLastError();
					if ( result< 0 )
					{
						CSG_LOG_DEBUG("CSocketBase::del the socket="<<ToStr(client_fd));
					} else
					{
						CSG_LOG_DEBUG("CSocketBase::the socket="<<ToStr(client_fd)<<" disconneted....");
					}
					CLinuxSocket::closeSocket(client_fd);

					CSocketHelper::onDisconnected(client_fd,lastError);

					continue;
				}
				if ( _events[index].events&EPOLLIN )
				{
					char buf[DATA_BUFF_SOCKET_SIZE];
					memset(buf ,'\0' ,sizeof( buf ));
					int nbytes=0;
					if ( ( nbytes = CLinuxSocket::Recv(client_fd ,buf ,sizeof( buf ) ,0) ) <= 0 )
					{
						if ( nbytes == 0 )
						{
							CSG_LOG_DEBUG("CSocketBase::sockett="<<ToStr(client_fd)<<" is hung up....");
						} else
						{
							CSG_LOG_DEBUG("CSocketBase::sockett="<<ToStr(client_fd)<<" recv error....");
						}
						CLinuxSocket::closeSocket(client_fd);
						int lastError=CSysUtil::getLastError();
						CSocketHelper::onDisconnected(client_fd,lastError);
					} else
					{
						//CSG_LOG_DEBUG("CSocketBase::sockett="<<ToStr(client_fd)<<"get msg="<<std::string(buf));

						CSocketHelper::onRecvMsg(client_fd ,buf,nbytes);

						CLinuxSocket::Write(client_fd ,buf ,nbytes);
					}
				}
			}
		}
	}
}

#endif

