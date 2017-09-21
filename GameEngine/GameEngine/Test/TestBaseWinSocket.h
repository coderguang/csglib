#ifndef _TEST_TEST_BASE_WIN_SOCKET_H_
#define _TEST_TEST_BASE_WIN_SOCKET_H_

#include "engine/def/environment_def.h"

#ifdef CSG_WIN

#include <WinSock2.h>
#pragma comment(lib, "Ws2_32.lib")      // Socket编程需用的动态链接库
#include "engine/net/socket/WinSocket.h"
#include "framework/util/sysUtil.h"
#include "engine/net/socket/socket.h"
#include "framework/log/Log.h"
#include "TestSocket.h"
#include "framework/util/typetransform.h"

const char *host = "127.0.0.1";
const int port = 9001;
const int MSGSIZE = 128;

using namespace std;
using namespace csg;

void startWinClient()
{

	WORD mVersionRequested;
	WSADATA wsaData;//WSADATA结构被用来保存AfxSocketInit函数返回的WindowsSockets初始化信息。
	int ret;

	SOCKET sClient;
	struct sockaddr_in saServer;
	bool fSuccess = true;

	//winSock 初始化

	mVersionRequested = MAKEWORD(2 ,2);//WinSock dll的版本

	ret = WSAStartup(mVersionRequested ,&wsaData);

	if ( ret != 0 )
	{
		cout << "WSASartup  failed!" << endl;
		getchar();
		getchar();
		exit(1);
	}

	if ( LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2 )
	{
		WSACleanup();
		cout << "invaild version" << endl;
		getchar();
		getchar();
		exit(1);
	}

	sClient = socket(AF_INET ,SOCK_STREAM ,IPPROTO_TCP);
	if ( sClient == INVALID_SOCKET )
	{
		WSACleanup();
		cout << "socket failed" << endl;
		getchar();
		getchar();
		exit(0);
	}



	saServer.sin_family = AF_INET;
	saServer.sin_port = htons(port);
	saServer.sin_addr.S_un.S_addr = inet_addr(host);

	ret = connect(sClient ,( struct sockaddr * )&saServer ,sizeof(saServer));
	if ( ret == SOCKET_ERROR )
	{
		cout << "连接服务器失败!点击任意键退出....." << endl;
		closesocket(sClient);
		WSACleanup();
	}

}

void startWinServer()
{
	WORD mVersionRequested;
	WSADATA wsaData;//WSADATA结构被用来保存AfxSocketInit函数返回的WindowsSockets初始化信息。
	int ret;

	SOCKET sServer;
	struct sockaddr_in saServer;
	bool fSuccess = true;

	//winSock 初始化

	mVersionRequested = MAKEWORD(2 ,2);//WinSock dll的版本

	ret = WSAStartup(mVersionRequested ,&wsaData);

	if ( ret != 0 )
	{
		cout << "WSASartup  failed!" << endl;
	}

	if ( LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2 )
	{
		WSACleanup();
		cout << "invaild version" << endl;
	}

	sServer = socket(AF_INET ,SOCK_STREAM ,IPPROTO_TCP);
	if ( sServer == INVALID_SOCKET )
	{
		WSACleanup();
		cout << "socket failed" << endl;
	}

	saServer.sin_family = AF_INET;
	saServer.sin_port = htons(port);
	saServer.sin_addr.S_un.S_addr = inet_addr(INADDR_ANY);

	auto bindResult = bind(sServer ,( struct sockaddr_in* )&saServer ,sizeof(struct sockaddr_in));

	int ref=listen(sServer ,10);
	
	if ( ref == SOCKET_ERROR )
	{
		cout << "启动服务器失败!点击任意键退出....." << endl;
		cout << "code=....." << GetLastError()<<endl;
		closesocket(sServer);
		WSACleanup();
	}

}

int startServerEx()
{
	// 加载socket动态链接库
	WORD wVersionRequested = MAKEWORD(2 ,2); // 请求2.2版本的WinSock库
	WSADATA wsaData;    // 接收Windows Socket的结构信息
	DWORD err = WSAStartup(wVersionRequested ,&wsaData);

	if ( 0 != err )
	{ // 检查套接字库是否申请成功
		cerr << "Request Windows Socket Library Error!\n";
		system("pause");
		return -1;
	}
	if ( LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2 )
	{// 检查是否申请了所需版本的套接字库
		WSACleanup();
		cerr << "Request Windows Socket Version 2.2 Error!\n";
		system("pause");
		return -1;
	}

	// 建立流式套接字
	SOCKET srvSocket = socket(AF_INET ,SOCK_STREAM ,IPPROTO_TCP);

	//绑定IP和端口    
	SOCKADDR_IN sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(port);
	sin.sin_addr.S_un.S_addr =INADDR_ANY;

	int error = WSAGetLastError();

	int length = sizeof(sin);

	int bindResult=::bind(srvSocket ,(struct sockaddr*) &sin ,sizeof(sin));

	error = WSAGetLastError();

	// 将SOCKET设置为监听模式
	int listenResult = listen(srvSocket ,1);

	error = WSAGetLastError();

	if ( SOCKET_ERROR == listenResult )
	{
		cerr << "Listen failed. Error: " << WSAGetLastError() << endl;
		return -1;
	}
	return 0;
}


// 开始服务工作线程函数
DWORD WINAPI myServerWorkerThread(LPVOID IpParam)
{
	HANDLE CompletionPort = (HANDLE) IpParam;
	DWORD BytesTransferred;
	LPOVERLAPPED IpOverlapped;
	CSG_SOCKET_DATA* PerHandleData = NULL;
	CSG_SOCKET_IO_DATA* PerIoData = NULL;
	DWORD RecvBytes;
	DWORD Flags = 0;
	BOOL bRet = false;

	while ( true )
	{
		bRet = CWinSocket::getQueuedCompletionStatus(CompletionPort ,&BytesTransferred ,(PULONG_PTR) &PerHandleData ,(LPOVERLAPPED*) &IpOverlapped ,INFINITE);//此处可以将IpOverlapped换为PerIoData，然后将下面CONTAINING_RECORD注释掉
		if ( bRet == 0 )
		{
			cerr << "GetQueuedCompletionStatus Error: " << GetLastError() << endl;
			return -1;
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
// 
// 		cout << "threadId=" << CThread::threadId() << ",socket=" << PerHandleData->socketfd << "," << "A Client getSize: " << strlen(PerIoData->databuff.buf) <<" it say " std::endl;

		PerIoData->buffer[PerIoData->overlapped.InternalHigh] = '\0';
		CSG_LOG_DEBUG("socket=" << ToStr(PerHandleData->socketfd) << ", it say:"<<std::string(PerIoData->buffer));

		// 为下一个重叠调用建立单I/O操作数据
		ZeroMemory(&( PerIoData->overlapped ) ,sizeof(OVERLAPPED)); // 清空内存
		PerIoData->databuff.len = 1024;
		PerIoData->databuff.buf = PerIoData->buffer;//buf是个指针，这一过程会清空buffer的内容
		PerIoData->operationType = 0;   // read
		CWinSocket::recv(PerHandleData->socketfd ,&( PerIoData->databuff ) ,1 ,&RecvBytes ,&Flags ,&( PerIoData->overlapped ) ,NULL);
	}

	return 0;
}

int startWinSrv()
{
	using namespace csg;
	HANDLE completionPort = CWinSocket::CreateIOCPPortInit();
	if ( NULL == completionPort )
	{   // 创建IO内核对象失败
		
		return -1;
	}
	//创建服务进程
	std::vector<HANDLE> handleVec;
	CWinSocket::createIOCPThread(handleVec,completionPort ,myServerWorkerThread);

	int result = CWinSocket::InitSocket();
	if ( -1 == result )
	{  
		
		return -1;
	}

	int srvSocket = CWinSocket::getSocket(AF_INET ,SOCK_STREAM ,0);
	if ( -1 == srvSocket )
	{
		return -1;
	}

	int bindReuslt = CWinSocket::bind(srvSocket ,AF_INET ,DefaultPort ,INADDR_ANY);
	if ( -1 == bindReuslt )
	{
		return -1;
	}
	int listenReslt = CWinSocket::listen(srvSocket ,10);
	if ( -1 == listenReslt )
	{
		return -1;
	}

	std::cout << "本服务器已准备就绪，正在等待客户端的接入...\n";
	
	while ( true )
	{
		SOCKADDR_IN saRemote;
		int RemoteLen;
		int acceptSocket;

		// 接收连接，并分配完成端，这儿可以用AcceptEx()
		RemoteLen = sizeof(saRemote);
		acceptSocket = CWinSocket::accept(srvSocket ,(SOCKADDR*) &saRemote ,&RemoteLen);
		if ( SOCKET_ERROR == acceptSocket )
		{ // 接收客户端失败
			cerr << "Accept Socket Error: " << GetLastError() << endl;
			return -1;
		}
		CSG_SOCKET_DATA * PerHandleData = NULL;
		// 创建用来和套接字关联的单句柄数据信息结构
		PerHandleData = new CSG_SOCKET_DATA;
		PerHandleData->socketfd = acceptSocket;
		memcpy(&PerHandleData->clientAddr ,&saRemote ,RemoteLen);

		// 将接受套接字和完成端口关联
		CWinSocket::createIOCPPort((HANDLE) ( PerHandleData->socketfd) ,completionPort ,(DWORD) PerHandleData ,0);


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

int startClientC()
{
	CWinSocket::InitSocket();

	int clientfd = CWinSocket::getSocket(AF_INET ,SOCK_STREAM ,IPPROTO_TCP);
	if ( INVALID_SOCKET == clientfd )
	{
		CSG_LOG_DEBUG("winsocket socket failed,invalid socket");
	}

	struct sockaddr_in servaddr;
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(DefaultPort);
	servaddr.sin_addr.S_un.S_addr = inet_addr(localAddr);

	if ( SOCKET_ERROR == connect(clientfd ,( struct sockaddr * )&servaddr ,sizeof(servaddr)) )
	{
		CSG_LOG_DEBUG("winsocket socket failed,invalid socket""hello");
		return -1;
	}

	std::string str = "threadId:" + ToStr(CThread::threadId()) + "\0";

	do
	{
		//std::cout << "enter key for send" << std::endl;;
		//cin.get();
		int sResult = send(clientfd ,str.c_str() ,str.length(),0);
		std::cout << "send size=" << ToStr(str.length()) << ",result=" << sResult << std::endl;;
		CThread::sleep_for(1);
	} while ( true );

	return -1;
}

int startMyWinSocket()
{
	char ch;
	do
	{
		std::cout << "enter s fro server,c fro client,other for nothing" << std::endl;
		ch = cin.get();
		if ( 's' == ch )
		{
			startWinSrv();
		} else if ( 'c' == ch )
		{
			startClientC();
		} else if ( 'b' == ch )
		{
			break;
		}
	} while ( true );
	return 0;
}





void testBaseWinSocket()
{
	startMyWinSocket();
	//startServerEx();
	//startWinServer();
	//startWinClient();
}

#endif

#endif