#include "engine/def/environment_def.h"
#ifdef CSG_WIN
#include "engine/net/socket/WinSocket.h"
#include "framework/log/Log.h"
#include "framework/util/typetransform.h"
#include "framework/util/sysUtil.h"
#include "socket.h"
#include "../../def/csg_def.h"

#pragma comment(lib, "Ws2_32.lib")      // Socket编程需用的动态链接库
#pragma comment(lib, "Kernel32.lib")    // IOCP需要用到的动态链接库

using namespace csg;
HANDLE csg::CWinSocket::CreateIOCPPortInit()
{
	HANDLE completionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE ,NULL ,0 ,0);
	if ( NULL == completionPort )
	{
		CSG_RECORD_ERROR("WinSocket::createIocpPort failed!");
	}
	return completionPort;
}

int csg::CWinSocket::createIOCPThread(std::vector<HANDLE>& threadVec ,HANDLE completionPort ,SOCKET_SERVER_WORKER_THREAD_FUNCTION workerThreadFunction ,int threadNum/*=0*/)
{
	if ( 0 == threadNum )
	{
		threadNum = CSysUtil::getNumberOfProcessors() * 2;
	}
#ifdef _DEBUG
	threadNum = 2;
#endif
	for ( int i = 0; i < threadNum; i++ )
	{
		// 创建服务器工作器线程，并将完成端口传递到该线程
		//第一NULL代表默认安全选项，第一个0，代表线程占用资源大小，第二个0，代表线程创建后立即执行
		HANDLE thread = CreateThread(NULL ,0 ,workerThreadFunction,completionPort ,0 ,NULL);
		if ( NULL == thread )
		{
			CSG_RECORD_ERROR("CWinSocket::createIOCPThread failed!");
			return CSG_RETURN_ERROR;
		}
		threadVec.push_back(thread);
		CloseHandle(thread);
	}
	return threadVec.size();
}


HANDLE csg::CWinSocket::createIOCPPort(HANDLE FileHandle ,HANDLE ExistingCompletionPort ,ULONG_PTR completionKey ,DWORD NumberOfConcurrentThread)
{
	return CreateIoCompletionPort(FileHandle ,ExistingCompletionPort ,completionKey ,NumberOfConcurrentThread);
}

int csg::CWinSocket::getSocket(int af ,int type ,int protocol)
{
	WORD wVersionRequested = MAKEWORD(2 ,2);
	WSADATA wsaData;
	DWORD err = WSAStartup(wVersionRequested ,&wsaData);

	if ( 0 != err )
	{ // 检查套接字库是否申请成功
		CSG_RECORD_ERROR("CWinSocket::getSocket");
		return CSG_RETURN_ERROR;
	}
	if ( LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2 )
	{// 检查是否申请了所需版本的套接字库
		CSG_RECORD_ERROR("CWinSocket::getSocket version");
		WSACleanup();
		return CSG_RETURN_ERROR;
	}

	// 建立流式套接字
	SOCKET socketfd = socket(AF_INET ,SOCK_STREAM ,0);

	if ( socketfd <= 0 )
	{
		CSG_RECORD_ERROR("CWinSocket::getSocket version");
		WSACleanup();
		return CSG_RETURN_ERROR;
	}
	return socketfd;
}

int csg::CWinSocket::bind(int socketfd ,int af ,int port ,int saddr)
{
	SOCKADDR_IN sin;
	sin.sin_family = af;
	sin.sin_port = htons(port);
	sin.sin_addr.S_un.S_addr = htonl(saddr);
	sin.sin_zero[0] = '\0';

	int bindResult = ::bind(socketfd ,(struct sockaddr *) &sin ,sizeof( sin ));

	if ( 0 != bindResult )
	{
		CSG_RECORD_ERROR("CWinSocket::bind faild!");
		return CSG_RETURN_ERROR;
	}
	return CSG_RETURN_OK;
}

int csg::CWinSocket::listen(int socketfd ,int backlog)
{
	int result=::listen(socketfd ,backlog);
	if ( 0 != result )
	{
		CSG_RECORD_ERROR("CWinSocket::listen faild!");
		return CSG_RETURN_ERROR;
	}
	return CSG_RETURN_OK;
}

int csg::CWinSocket::accept(int socketfd ,sockaddr* addr ,int *addrlen)
{
	int socket = ::accept(socketfd ,addr ,addrlen);
	if ( -1 == socket )
	{
		CSG_RECORD_ERROR("CWinSocket::accept faild!");
	}
	return socket;
}

int csg::CWinSocket::recv(int socketfd ,LPWSABUF lpBuffers ,DWORD dwBufferCount ,LPDWORD lpNumberOfBytesRecvd ,LPDWORD lpFlags ,LPWSAOVERLAPPED lpOverlapped ,LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRout)
{
	return WSARecv(socketfd ,lpBuffers ,dwBufferCount ,lpNumberOfBytesRecvd ,lpFlags ,lpOverlapped ,lpCompletionRout);
}

bool csg::CWinSocket::getQueuedCompletionStatus(HANDLE completionPort ,LPDWORD lpNumberOfBytesTransferred ,PULONG_PTR lpCompletionKey ,LPOVERLAPPED* lpOverlapped ,DWORD dwMilliseconds)
{
	return GetQueuedCompletionStatus(completionPort ,lpNumberOfBytesTransferred ,lpCompletionKey ,lpOverlapped ,dwMilliseconds);
}

int csg::CWinSocket::close(int socketfd)
{
	int result=closesocket(socketfd);
	if ( -1 == result )
	{
		CSG_RECORD_ERROR("CWinSocket::close faild!");
	}
	return result;
}

int csg::CWinSocket::connect(int socketfd ,struct sockaddr* name ,int nameLen)
{
	int result = ::connect(socketfd ,name ,nameLen);
	if ( -1 == result )
	{
		CSG_RECORD_ERROR("CWinSocket::connect faild!");
	}
	return result;
}

int csg::CWinSocket::send(int clientfd ,const char* buf ,int len ,int flags)
{
	int result = ::send(clientfd ,buf ,len ,flags);
	if ( -1 == result )
	{
		CSG_RECORD_ERROR("CWinSocket::send faild!");
	}
	return result;
}

int csg::CWinSocket::InitSocket()
{
	// 加载socket动态链接库
	WORD wVersionRequested = MAKEWORD(2 ,2); // 请求2.2版本的WinSock库
	WSADATA wsaData;    // 接收Windows Socket的结构信息
	DWORD err = WSAStartup(wVersionRequested ,&wsaData);

	if ( 0 != err )
	{ // 检查套接字库是否申请成功
		CSG_RECORD_ERROR("CWinSocket::InitSocket faild!");
		return -1;
	}
	if ( LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2 )
	{// 检查是否申请了所需版本的套接字库
		CSG_RECORD_ERROR("CWinSocket::InitSocket version faild!");
		WSACleanup();
		return -1;
	}
	return 0;
}

void csg::CWinSocket::clearWinSocket()
{
	WSACleanup();
}

#endif