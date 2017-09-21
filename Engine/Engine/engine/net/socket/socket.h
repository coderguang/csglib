#ifndef _ENGINE_NET_SOCKET_SOCKET_H_
#define _ENGINE_NET_SOCKET_SOCKET_H_

#include "engine/def/environment_def.h"


#ifdef CSG_WIN
#include <WinSock2.h>

#elif defined(CSG_LINUX)
#include "engine/net/socket/LinuxSocket.h"

#endif

#include "framework/SmartPoint/refObject.h"

namespace csg
{
	const int DATA_BUFF_SOCKET_SIZE = 1024 * 2;

#ifdef CSG_WIN
	struct PER_IO_SOCKET_DATA{
		OVERLAPPED overlapped;
		WSABUF databuff;
		char buffer[DATA_BUFF_SOCKET_SIZE];
		int operationType;
	};

	struct PER_HANDLER_DATA
	{
		SOCKET socketfd;
		SOCKADDR_IN clientAddr;
	};
	typedef LPTHREAD_START_ROUTINE  SOCKET_SERVER_WORKER_THREAD_FUNCTION;

	typedef SOCKET CSG_SOCKET_FD;

	typedef PER_IO_SOCKET_DATA CSG_SOCKET_IO_DATA;

	typedef PER_HANDLER_DATA CSG_SOCKET_DATA;


#elif defined(CSG_LINUX)

#endif

	class CSocketHelper
	{
	public:

		static int getClientSocket(int family,int type,int protocol,int port,std::string ip);

		static int sendMsg(int socketfd ,const char* buf ,int len);

		static void onConnected(int clientfd ,std::string& remoteAddr ,int remotePort,bool isInner);

		static void onDisconnected(int clientfd,int errorCode);

		static void onRecvMsg(int clientfd ,const char* buf ,int len);
	};


}
#endif