
#ifndef _ENGINE_BASE_SERVER_SOCKET_RECV_TASK_H_
#define _ENGINE_BASE_SERVER_SOCKET_RECV_TASK_H_

#include "../thread/task.h"
#include "baseServerDef.h"

#ifdef CSG_LINUX

#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#elif defined CSG_WIN
#include <winbase.h>
#endif

namespace csg
{
	//该线程只负责接收IO数据,不负责解包操作
	class CSocketRecvTask :public virtual CTask
	{
	public:
		CSocketRecvTask() :CTask() ,_isClient(false)
		{
			setTaskType(EBaseServerSocketRecv);
		}

		virtual ~CSocketRecvTask();

		int init(int family ,int type ,int protocol ,int port ,std::string host = "127.0.0.1",bool isInnner = false );

		int startListen();

		int startConnect();
#ifdef CSG_WIN
		static DWORD WINAPI workerThreadRun(LPVOID IpParam);
#endif

		static int svc_run(void* task);

	protected:
		virtual int run();

		virtual void doRecvLoop();

#ifdef CSG_WIN
		int initWinSocket();

		void doWinLoop();
#elif defined(CSG_LINUX)
		int initLinuxSocket();

		void doLinuxLoop();
#endif

	private:
		int _family;
		int _type;
		int _protocol;
		int _port;
		bool _innerPort;
		bool _isClient;
		std::string _host;
	private:
		int _socketfd;
#ifdef CSG_WIN
		HANDLE _completionPort;
#elif defined(CSG_LINUX)
		struct ::epoll_event *_events;
		struct ::epoll_event _ev;
		int _epollfd;
#endif
	};


}

#endif
