#ifndef _ENGINE_NET_SOCKET_WIN_SOCKET_H_
#define _ENGINE_NET_SOCKET_WIN_SOCKET_H_

#include "engine/def/environment_def.h"

#ifdef CSG_WIN
#include <WinSock2.h>
#include <ws2def.h>
#include <vector>
#include <winsmcrd.h>

#endif
#include "socket.h"



namespace csg
{
#ifdef CSG_WIN
	class CWinSocket
	{
	public:
		//创建IOCP 完成端口
		static HANDLE CreateIOCPPortInit();
		//创建iocp工作线程
		static int createIOCPThread(std::vector<HANDLE>& ,HANDLE completionPort ,SOCKET_SERVER_WORKER_THREAD_FUNCTION workerThreadFunction,int threadNum = 0);
		//将接受套接字和完成端口关联
		//原型：WINBASEAPI __out_opt HANDLE WINAPI CreateIoCompletionPort( __in HANDLE FileHandle, __in_opt HANDLE ExistingCompletionPort, __in ULONG_PTR CompletionKey, __in DWORD NumberOfConcurrentThreads )
		static HANDLE createIOCPPort(HANDLE FileHandle ,HANDLE ExistingCompletionPort ,ULONG_PTR completionKey ,DWORD NumberOfConcurrentThread);

		static int getSocket(int af ,int type ,int protocol);

		static int bind(int socketfd,int af ,int port,int saddr);

		static int listen(int socketfd ,int backlog);

		static int accept(int socketfd ,sockaddr* addr ,int *addrlen);

		//WINSOCK_API_LINKAGE int WSAAPI WSARecv( _In_ SOCKET s, _In_reads_(dwBufferCount) __out_data_source(NETWORK) LPWSABUF lpBuffers, _In_ DWORD dwBufferCount, _Out_opt_ LPDWORD lpNumberOfBytesRecvd, _Inout_ LPDWORD lpFlags, _Inout_opt_ LPWSAOVERLAPPED lpOverlapped
		static int recv(int socketfd ,LPWSABUF lpBuffers ,DWORD dwBufferCount ,LPDWORD lpNumberOfBytesRecvd ,LPDWORD lpFlags ,LPWSAOVERLAPPED lpOverlapped,LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRout);

		//WINBASEAPI BOOL WINAPI GetQueuedCompletionStatus( __in HANDLE CompletionPort, __out LPDWORD lpNumberOfBytesTransferred, __out PULONG_PTR lpCompletionKey, __out LPOVERLAPPED *lpOverlapped, __in DWORD dwMilliseconds )
		static bool getQueuedCompletionStatus(HANDLE completionPort ,LPDWORD lpNumberOfBytesTransferred ,PULONG_PTR lpCompletionKey ,LPOVERLAPPED* lpOverlapped ,DWORD dwMilliseconds);


		static int close(int socketfd);

		static int connect(int socketfd ,struct sockaddr* name ,int nameLen);

		static int send(int clientfd ,const char* buf ,int len ,int flags);

		static int InitSocket();

		static void clearWinSocket();
	};


#endif
}
#endif