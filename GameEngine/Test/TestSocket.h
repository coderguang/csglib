#ifndef _TEST_TEST_SOCKET_H_
#define _TEST_TEST_SOCKET_H_

#include "engine/def/environment_def.h"

#ifdef CSG_WIN

#include <WinSock2.h>
#include <vector>
#include <iostream>
#include <ws2def.h>
#include "engine\serialize\serializestream.h"
#include "engine\thread\thread.h"

#pragma comment(lib, "Ws2_32.lib")      // Socket编程需用的动态链接库
#pragma comment(lib, "Kernel32.lib")    // IOCP需要用到的动态链接库

/**
* 结构体名称：PER_IO_DATA
* 结构体功能：重叠I/O需要用到的结构体，临时记录IO数据
**/
const int DataBuffSize = 2 * 1024;

struct PER_IO_OPERATEION_DATA
{
	OVERLAPPED overlapped;
	WSABUF databuff;
	char buffer[DataBuffSize];
	int BufferLen;
	int operationType;
};

/**
* 结构体名称：PER_HANDLE_DATA
* 结构体存储：记录单个套接字的数据，包括了套接字的变量及套接字的对应的客户端的地址。
* 结构体作用：当服务器连接上客户端时，信息存储到该结构体中，知道客户端的地址以便于回访。
**/

struct PER_HANDLE_DATA
{
	SOCKET socket;
	SOCKADDR_STORAGE ClientAddr;
};

// 定义全局变量
const int DefaultPort = 9001;
#define localAddr  "127.0.0.1"

using namespace std;
vector < PER_HANDLE_DATA* > clientGroup;        // 记录客户端的向量组
vector<PER_IO_OPERATEION_DATA*> IOOperationDataGroup;

HANDLE hMutex = CreateMutex(NULL ,FALSE ,NULL);
DWORD WINAPI ServerWorkThread(LPVOID CompletionPortID);
DWORD WINAPI ServerSendThread(LPVOID IpParam);


#define INCL_WINSOCK_API_PROTOTYPES 1
#define WINSOCK_API_LINKAGE 1

// 开始主函数
int startIOCP()
{
	//// 创建用于发送数据的线程
	//HANDLE sendThread = CreateThread(NULL, 0, ServerSendThread, 0, 0, NULL);//第二个0，代表回掉函数参数为0

	// 创建IOCP的内核对象
	/**
	* 需要用到的函数的原型：
	* HANDLE WINAPI CreateIoCompletionPort(
	*    __in   HANDLE FileHandle,      // 已经打开的文件句柄或者空句柄，一般是客户端的句柄
	*    __in   HANDLE ExistingCompletionPort,  // 已经存在的IOCP句柄
	*    __in   ULONG_PTR CompletionKey,    // 完成键，包含了指定I/O完成包的指定文件
	*    __in   DWORD NumberOfConcurrentThreads // 真正并发同时执行最大线程数，一般推介是CPU核心数*2
	* );
	**/
	HANDLE completionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE ,NULL ,0 ,0);
	if ( NULL == completionPort )
	{   // 创建IO内核对象失败
		cerr << "CreateIoCompletionPort failed. Error:" << GetLastError() << endl;
		system("pause");
		return -1;
	}

	// 创建IOCP线程--线程里面创建线程池

	// 确定处理器的核心数量
	SYSTEM_INFO mySysInfo;
	GetSystemInfo(&mySysInfo);

	int workerAmount = mySysInfo.dwNumberOfProcessors*2;


	// 基于处理器的核心数量创建线程
	for ( DWORD i = 0; i <workerAmount; ++i )
	{
		// 创建服务器工作器线程，并将完成端口传递到该线程
		HANDLE ThreadHandle = CreateThread(NULL ,0 ,ServerWorkThread ,completionPort ,0 ,NULL);//第一NULL代表默认安全选项，第一个0，代表线程占用资源大小，第二个0，代表线程创建后立即执行
		if ( NULL == ThreadHandle )
		{
			cerr << "Create Thread Handle failed. Error:" << GetLastError() << endl;
			system("pause");
			return -1;
		}
		CloseHandle(ThreadHandle);
	}

	// 加载socket动态链接库
	WORD wVersionRequested = MAKEWORD(2,2); // 请求2.2版本的WinSock库
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
	SOCKET srvSocket = socket(AF_INET ,SOCK_STREAM ,0);



	//绑定IP和端口    
	SOCKADDR_IN sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(DefaultPort);
	sin.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	sin.sin_zero[0] = '\0';
	int bindResult = ::bind(srvSocket ,(struct sockaddr*) &sin ,sizeof(sin));



	// 将SOCKET设置为监听模式
	int listenResult = listen(srvSocket ,10);
	if ( SOCKET_ERROR == listenResult )
	{
		cerr << "Listen failed. Error: " << GetLastError() << endl;
		system("pause");
		return -1;
	}

	// 开始处理IO数据
	cout << "本服务器已准备就绪，正在等待客户端的接入...\n";

	while ( true )
	{
		SOCKADDR_IN saRemote;
		int RemoteLen;
		SOCKET acceptSocket;

		// 接收连接，并分配完成端，这儿可以用AcceptEx()
		RemoteLen = sizeof(saRemote);
		acceptSocket = accept(srvSocket ,(SOCKADDR*) &saRemote ,&RemoteLen);
		if ( SOCKET_ERROR == acceptSocket )
		{ // 接收客户端失败
			cerr << "Accept Socket Error: " << GetLastError() << endl;
			system("pause");
			return -1;
		}
		PER_HANDLE_DATA * PerHandleData = NULL;
		// 创建用来和套接字关联的单句柄数据信息结构
		PerHandleData = new PER_HANDLE_DATA;
		PerHandleData->socket = acceptSocket;
		memcpy(&PerHandleData->ClientAddr ,&saRemote ,RemoteLen);
		clientGroup.push_back(PerHandleData);       // 将单个客户端数据指针放到客户端组中

		// 将接受套接字和完成端口关联
		CreateIoCompletionPort((HANDLE) ( PerHandleData->socket ) ,completionPort ,(DWORD) PerHandleData ,0);


		// 开始在接受套接字上处理I/O使用重叠I/O机制
		// 在新建的套接字上投递一个或多个异步
		// WSARecv或WSASend请求，这些I/O请求完成后，工作者线程会为I/O请求提供服务    
		// 单I/O操作数据(I/O重叠)
		PER_IO_OPERATEION_DATA* PerIoData = NULL;
		PerIoData = new PER_IO_OPERATEION_DATA;
		ZeroMemory(&( PerIoData->overlapped ) ,sizeof(OVERLAPPED));
		PerIoData->databuff.len = 1024;
		PerIoData->databuff.buf = PerIoData->buffer;
		PerIoData->operationType = 0;   // read

		IOOperationDataGroup.push_back(PerIoData);

		DWORD RecvBytes;
		DWORD Flags = 0;  //WSARecv中的1，代表缓冲区lpBuffers只包含一个WSABUF，Flags代表接收普通数据
		WSARecv(PerHandleData->socket ,&( PerIoData->databuff ) ,1 ,&RecvBytes ,&Flags ,&( PerIoData->overlapped ) ,NULL);//PerIoData->overlapped就是CONTAINING_RECORD的第一个变量
	}


	for ( auto it = IOOperationDataGroup.begin(); it != IOOperationDataGroup.end(); it++ )
	{
		if ( *it != NULL )
			free(*it);
	}


	system("pause");
	return -1;
}


// 开始服务工作线程函数
DWORD WINAPI ServerWorkThread(LPVOID IpParam)
{
	HANDLE CompletionPort = (HANDLE) IpParam;
	DWORD BytesTransferred;
	LPOVERLAPPED IpOverlapped;
	PER_HANDLE_DATA* PerHandleData = NULL;
	PER_IO_OPERATEION_DATA* PerIoData = NULL;
	DWORD RecvBytes;
	DWORD Flags = 0;
	BOOL bRet = false;

	while ( true )
	{
		bRet = GetQueuedCompletionStatus(CompletionPort ,&BytesTransferred ,(PULONG_PTR) &PerHandleData ,(LPOVERLAPPED*) &IpOverlapped ,INFINITE);//此处可以将IpOverlapped换为PerIoData，然后将下面CONTAINING_RECORD注释掉
		if ( bRet == 0 )
		{
			cerr << "GetQueuedCompletionStatus Error: " << GetLastError() << endl;
			return -1;
		}
		PerIoData = (PER_IO_OPERATEION_DATA*) CONTAINING_RECORD(IpOverlapped ,PER_IO_OPERATEION_DATA ,overlapped);
		//这个宏的作用是：根据一个结构体实例中的成员的地址，取到整个结构体实例的地址
		//PER_IO_DATA的成员overlapped的地址为&IpOverlapped，结果就可以获得PER_IO_DATA的地址

		// 检查在套接字上是否有错误发生
		if ( 0 == BytesTransferred )
		{
			closesocket(PerHandleData->socket);
			GlobalFree(PerHandleData);
			GlobalFree(PerIoData);
			continue;
		}

		// 开始数据处理，接收来自客户端的数据
		//WaitForSingleObject(hMutex ,INFINITE);
		cout <<"threadId="<<CThread::threadId()<<",socket="<<PerHandleData->socket<<","<<"A Client getSize: " << strlen(PerIoData->databuff.buf) <<std::endl;
		
// 		std::cout << "receive size =" << ToStr(strlen(PerIoData->databuff.buf)) << std::endl;
// 		std::cout << "enter r for searize" << std::endl;
// 		char ch;
// 		ch = cin.get();
// 		if ( 'r' == ch )
// 		{
// 			CSerializeStream getOs;
// 			getOs.append(PerIoData->databuff.buf ,strlen(PerIoData->databuff.buf));
// 			SeriaTestAll getAll;
// 			getOs.prepareToRead();
// 			getAll.read(getOs);
// 		} else
// 		{
// 			std::cout << "you have skip!" << std::endl;
// 		}
		//ReleaseMutex(hMutex);

		// 为下一个重叠调用建立单I/O操作数据
		ZeroMemory(&( PerIoData->overlapped ) ,sizeof(OVERLAPPED)); // 清空内存
		PerIoData->databuff.len = 1024;
		PerIoData->databuff.buf = PerIoData->buffer;//buf是个指针，这一过程会清空buffer的内容
		PerIoData->operationType = 0;   // read
		WSARecv(PerHandleData->socket ,&( PerIoData->databuff ) ,1 ,&RecvBytes ,&Flags ,&( PerIoData->overlapped ) ,NULL);
	}

	return 0;
}

int startClient()
{

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

	SOCKET clientfd = socket(AF_INET ,SOCK_STREAM ,IPPROTO_TCP);
	if ( INVALID_SOCKET == clientfd )
	{
		CSG_LOG_DEBUG("winsocket socket failed,invalid socket");
	}

	struct sockaddr_in servaddr;
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(DefaultPort);
	servaddr.sin_addr.S_un.S_addr = inet_addr(localAddr);

	SeriaTestAll src;
	src.bByte = 1;
	src.iInt = 2;
	src.bBool = true;
	src.sShort = 4;
	src.lLong = 5;
	src.str = "6";

	CSerializeStream srcOS;
	srcOS.setUseBitMark(true);
	src.write(srcOS);
	srcOS.prepareToAppend();

	CSerializeStream tmpOS;
	tmpOS.writeSize(srcOS.getFlagDataSize());
	tmpOS.append(srcOS.getFlagData() ,srcOS.getFlagDataSize());
	tmpOS.append(srcOS.getData() ,srcOS.getDataSize());

	CSerializeStream restultOS;
	restultOS.append(tmpOS.getData() ,tmpOS.getDataSize());
	
	/*
	restultOS.prepareToRead();

	int resultDataLength = restultOS.getDataSize();
	int resultFlagLength = restultOS.getFlagDataSize();

	restultOS.setUseBitMark(true);

	SeriaTestAll result;
	result.read(restultOS);
	**/

	std::string s = "hello";
	std::string b = "hh";
	
	if ( SOCKET_ERROR == connect(clientfd ,( struct sockaddr * )&servaddr ,sizeof(servaddr)) )
	{
		CSG_LOG_DEBUG("winsocket socket failed,invalid socket""hello");
		return -1;
	}

	char testsend[10] = "hello\0";

	do
	{
		//std::cout << "enter key for send" << std::endl;;
		//cin.get();
		int sResult = send(clientfd ,testsend ,strlen(testsend),0);
		std::cout << "send size=" << ToStr(sizeof(testsend)) << ",result=" << sResult << std::endl;;
		CThread::sleep_for(100);
	} while ( true );

	return -1;

}

void testSocketExample()
{
	char ch;
	do
	{
		std::cout << "enter s fro server,c fro client,other for nothing" << std::endl;
		ch=cin.get();
		if ( 's' == ch )
		{
			startIOCP();
			break;
		} else if ( 'c' == ch )
		{
			startClient();
			break;
		}
	} while ( true );	

}


void testSocket()
{

}

#endif


#endif