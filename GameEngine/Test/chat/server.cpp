#include "server.h"
#include "engine/def/environment_def.h"
#include "framework/util/sysUtil.h"
#include "framework/log/Log.h"
#include "framework/util/typetransform.h"
#include "engine/net/socket/socket.h"


#ifdef CSG_WIN

using namespace csg;

csg::CChatServer::CChatServer(int af ,int type ,int protol ,int port ,HANDLE completionPort) :_af(af) ,_type(type) ,_protocol(protol) ,_port(port)
{
	_completionPort = completionPort;
	 CWinSocket::InitSocket();

	 _listenfd = CWinSocket::getSocket(_af ,_type ,_protocol);
	
	CWinSocket::bind(_listenfd ,_af ,_port ,INADDR_ANY);
	
	 CWinSocket::listen(_listenfd ,10);
	
	 CSG_LOG_DEBUG("本服务器已准备就绪，正在等待客户端的接入...");
}

csg::CChatServer::~CChatServer()
{
	CWinSocket::close(_listenfd);
	CWinSocket::clearWinSocket();
}

void csg::CChatServer::startRun()
{
	std::thread th(CChatServer::run ,this);
	th.detach();
}

void csg::CChatServer::run(CChatServer* srv)
{
	srv->doRun();
}

void csg::CChatServer::doRun()
{
	while ( true )
	{
		SOCKADDR_IN saRemote;
		int RemoteLen;
		int acceptSocket;

		// 接收连接，并分配完成端，这儿可以用AcceptEx()
		RemoteLen = sizeof(saRemote);
		acceptSocket = CWinSocket::accept(_listenfd ,(SOCKADDR*) &saRemote ,&RemoteLen);
		if ( SOCKET_ERROR == acceptSocket )
		{ // 接收客户端失败
			cerr << "Accept Socket Error: " << GetLastError() << endl;
		}
		CSG_SOCKET_DATA * PerHandleData = NULL;
		// 创建用来和套接字关联的单句柄数据信息结构
		PerHandleData = new CSG_SOCKET_DATA;
		PerHandleData->socketfd = acceptSocket;
		memcpy(&PerHandleData->clientAddr ,&saRemote ,RemoteLen);

		// 将接受套接字和完成端口关联
		CWinSocket::createIOCPPort((HANDLE) ( PerHandleData->socketfd ) ,_completionPort ,(DWORD) PerHandleData ,0);


		// 开始在接受套接字上处理I/O使用重叠I/O机制
		// 在新建的套接字上投递一个或多个异步
		// WSARecv或WSASend请求，这些I/O请求完成后，工作者线程会为I/O请求提供服务    
		// 单I/O操作数据(I/O重叠)
		CSG_SOCKET_IO_DATA* PerIoData = NULL;
		PerIoData = new CSG_SOCKET_IO_DATA;
		ZeroMemory(&( PerIoData->overlapped ) ,sizeof(OVERLAPPED));
		PerIoData->databuff.len = 1024;
		PerIoData->databuff.buf = PerIoData->buffer;
		PerIoData->operationType = 0;   // read;

		DWORD RecvBytes;
		DWORD Flags = 0;  //WSARecv中的1，代表缓冲区lpBuffers只包含一个WSABUF，Flags代表接收普通数据
		CWinSocket::recv(PerHandleData->socketfd ,&( PerIoData->databuff ) ,1 ,&RecvBytes ,&Flags ,&( PerIoData->overlapped ) ,NULL);//PerIoData->overlapped就是CONTAINING_RECORD的第一个变量
	}
}

#endif