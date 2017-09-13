#ifndef _ENGINE_NET_SOCKET_SOCKET_MANAGER_H_
#define _ENGINE_NET_SOCKET_SOCKET_MANAGER_H_

#include "engine/def/environment_def.h"
#include "engine/net/socket/WinSocket.h"
#include <vector>
#include "../../baseServer/SocketRecvTask.h"
#include <string>

namespace csg
{

	class CSocketManager
	{
	public:
		static CSocketManager* instance();
		// windows 下用于初始化IOCP
		int init();

#ifdef CSG_WIN
		HANDLE getIOCPPort();
#endif

		void startListen(int family ,int type ,int protocol ,int port);

		int startConnect(int family ,int type ,int protocol ,int port ,std::string host);

	private:
		CSocketManager();

		virtual ~CSocketManager();
	private:
		bool _isInit;
		bool _coreError;
		std::vector<CSocketRecvTask*> _recvTaskList;
#ifdef CSG_WIN
		HANDLE _completionPort;
		std::vector<HANDLE> _workerThreadVec;
#endif

	};



}
#endif