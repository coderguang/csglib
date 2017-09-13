
#ifndef _TEST_TEST_CHAT_H_
#define _TEST_TEST_CHAT_H_

#include "chat/client.h"
#include "TestSocket.h"
#include "engine/thread/thread.h"
#include "chat/server.h"
#include "engine/net/socket/LinuxSocket.h"
#include "engine/baseServer/SocketRecvTask.h"
#include "engine/net/socket/socketManager.h"


using namespace csg;

#ifdef CSG_WIN
void testChatClient(int num,int port)
{
	num = 1;
// 	for ( int i = 0; i < num;i++ )
// 	{
// 		//const char *host = "123.207.87.135";
// 		const char *host = "127.0.0.1";
// 		CChatClientPtr ptr = new  CChatClient(std::string(host) ,port);
// 		ptr->startRun();
// 		list.push_back(ptr);
// 	}

	const char *host = "123.207.87.135";
	//const char *host = "127.0.0.1";
	
	std::vector<CChatClientPtr> list;
	for ( int i = 0; i < 1; i++ )
	{
		CChatClientPtr ptr = new  CChatClient(std::string(host) ,9001);
		ptr->startRun();
		CThread::sleep_for(1000);
		list.push_back(ptr);
	}
	while ( true )
	{

	}
}

// 开始服务工作线程函数
DWORD WINAPI ChatServerWorkerThread(LPVOID IpParam)
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

		PerIoData->buffer[PerIoData->overlapped.InternalHigh] = '\0';
		CSG_LOG_DEBUG("socket=" << ToStr(PerHandleData->socketfd) << ", it say:" << std::string(PerIoData->buffer));
		std::string resend = "from server,I'm thread " + ToStr(CThread::threadId());
		CWinSocket::send(PerHandleData->socketfd ,resend.c_str() ,resend.length() ,0);

		// 为下一个重叠调用建立单I/O操作数据
		ZeroMemory(&( PerIoData->overlapped ) ,sizeof(OVERLAPPED)); // 清空内存
		PerIoData->databuff.len = 1024;
		PerIoData->databuff.buf = PerIoData->buffer;//buf是个指针，这一过程会清空buffer的内容
		PerIoData->operationType = 0;   // read
		CWinSocket::recv(PerHandleData->socketfd ,&( PerIoData->databuff ) ,1 ,&RecvBytes ,&Flags ,&( PerIoData->overlapped ) ,NULL);
	}

	return 0;
}

void testChatServer()
{
	HANDLE completionPort = CWinSocket::CreateIOCPPortInit();
	if ( NULL == completionPort )
	{   // 创建IO内核对象失败
	
	}
	//创建服务进程
	std::vector<HANDLE> handleVec;
	CWinSocket::createIOCPThread(handleVec ,completionPort ,ChatServerWorkerThread);


	CChatServer sServer(AF_INET ,SOCK_STREAM ,0 ,DefaultPort,completionPort);
	sServer.startRun();
	
	CChatServer sServerEx(AF_INET ,SOCK_STREAM ,0 ,DefaultPort+1 ,completionPort);
	sServerEx.startRun();

	while ( true )
	{
		;
	}
}
#endif

#endif


#ifdef CSG_LINUX

#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/epoll.h>

const int DefaultPort = 9001;
#define localAddr  "127.0.0.1"

static struct epoll_event *events;

void testChatServerLinux(){
	struct sockaddr_in serveraddr;
	struct sockaddr_in clientaddr;

	int fdmax;
	int listener;
	int newfd;

	char buf[1024];

	int nbytes;
	socklen_t addrlen;
	int yes;
	int epfd = -1;
	int res = -1;
	struct epoll_event ev;
	int index = 0;
	int client_fd = -1;

	listener = CLinuxSocket::getSocket(AF_INET ,SOCK_STREAM ,0);

// 	if ( setsockopt(listener ,SOL_SOCKET ,SO_REUSEADDR ,&yes ,sizeof(int)) == -1 )
// 	{
// 		std::cout << "setsocketopt faild!" << std::endl;
// 		exit(0);
// 	}

	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = INADDR_ANY;
	serveraddr.sin_port = htons(DefaultPort);

	memset(&( serveraddr.sin_zero ) ,'\0' ,8);

	CLinuxSocket::Bind(listener ,( struct sockaddr * )&serveraddr ,sizeof(serveraddr));

	CLinuxSocket::Listen(listener ,10);

	fdmax = listener;
	int MAX_CONN = 1000;
	events = (epoll_event*) calloc(MAX_CONN ,sizeof(struct epoll_event));

	if ( ( epfd = CLinuxSocket::Epoll_create(MAX_CONN) ) == -1 )
	{
		std::cout << "epoll create faildi!" << std::endl;
		exit(1);
	}

	ev.events = EPOLLIN;
	ev.data.fd = fdmax;

	if ( CLinuxSocket::Epoll_ctl(epfd ,EPOLL_CTL_ADD ,fdmax ,&ev)<0 )
	{
		std::cout << "CSocketBase::Epoll_ctl failed" << std::endl;
		exit(1);
	}

	for ( ;; )
	{
		res = CLinuxSocket::Epoll_wait(epfd ,events ,MAX_CONN ,-1);
		for ( index = 0; index<res; index++ )
		{

			client_fd = events[index].data.fd;//get the first events

			if ( client_fd == listener )
			{//new connections
				addrlen = sizeof(clientaddr);
				newfd =CLinuxSocket::Accept(listener ,( struct sockaddr * )&clientaddr ,&addrlen);
				if ( -1 != newfd )
				{
					ev.events = EPOLLIN;
					ev.data.fd = newfd;
					if ( CLinuxSocket::Epoll_ctl( epfd ,EPOLL_CTL_ADD ,newfd ,&ev )<0 )
					{
						std::cout << "CSocketBase::Epoll_ctl faild when add new sockfd" << std::endl;
						exit(1);
					} else
					{
						std::cout << "CSocketBase::Epoll_ctl add socket " << newfd << std::endl;
					}

				}
				continue;

			} else
			{


				if ( events[index].events&EPOLLHUP )
				{
					if ( CLinuxSocket::Epoll_ctl( epfd ,EPOLL_CTL_DEL ,client_fd ,&ev )<0 )
					{
						std::cout << "del the socket=" << client_fd << std::endl;
					} else
					{
						std::cout << "socket" << client_fd << "disconnec..." << std::endl;
					}
					CLinuxSocket::closeSocket(client_fd);
					continue;
				}



				if ( events[index].events&EPOLLIN )
				{
					memset(buf ,'\0' ,sizeof(buf));
					if ( ( nbytes = CLinuxSocket::Recv( client_fd ,buf ,sizeof(buf) ,0 ) ) <= 0 )
					{
						if ( nbytes == 0 )
						{
							std::cout << "socket " << client_fd << " is hung up" << std::endl;
						} else
						{
							std::cout << "recv error in socket" << client_fd << std::endl;

						
						}
					} else
					{
						std::cout << "get msg=" << buf << endl;
						CLinuxSocket::Write(client_fd,buf,nbytes);
					}

				}
			}



		}


	}


	std::cout << "over" << std::endl;

}

void testChatClientLinux(){

	int socketfd=CLinuxSocket::getSocket(AF_INET,SOCK_STREAM,0);
	
	struct sockaddr_in remote_addr;
	remote_addr.sin_family=AF_INET;
	remote_addr.sin_addr.s_addr=inet_addr("59.41.117.253");
	remote_addr.sin_port=htons(DefaultPort);

	CLinuxSocket::Connect(socketfd,(struct sockaddr*)&remote_addr,sizeof(remote_addr));

	CSG_LOG_DEBUG("start send");

	char buf[1024]="hello\0";

	do{
		CLinuxSocket::Write(socketfd,buf,strlen(buf));

		CThread::sleep_for(1000);
	}while(true);


}

void testLinuxChat()
{
	char ch;
	do
	{
		std::cout << "enter s fro server,c fro client,other for nothing" << std::endl;
		ch = cin.get();
		if ( 's' == ch )
		{
			testChatServerLinux();
			break;
		} else if ( 'c' == ch )
		{
			testChatClientLinux();
			break;

		}
	} while ( true );
}

#endif



void testSocketMgr()
{
	// 	do
	// 	{
	// 		std::cout << "enter port" << std::endl;
	// 		int port = cin.get();
	// 		cin.get();
	CSocketManager::instance()->startListen(AF_INET ,SOCK_STREAM ,0 ,9001);
	std::cout << "listen next" << std::endl;
	cin.get();
	cin.get();
	CSocketManager::instance()->startListen(AF_INET ,SOCK_STREAM ,0 ,9002);

	std::cout << "listen end" << std::endl;

	//} while ( true );

}

void testSocketMgrLinux()
{

	CSocketManager::instance()->startListen(AF_INET ,SOCK_STREAM ,0 ,9001);
}


void testChat()
{
	char ch;
	do
	{
		std::cout << "enter s fro server,c fro client,other for nothing" << std::endl;
		ch = cin.get();
		if ( 's' == ch )
		{
			testSocketMgr();
			break;
		} else if ( 'c' == ch )
		{
#ifdef CSG_WIN
			testChatClient(1 ,1);
#endif
		}
	} while ( true );
	ch = cin.get();
}