#ifndef _TEST_CHAT_SERVER_H_
#define _TEST_CHAT_SERVER_H_
#include "engine/def/environment_def.h"

#ifdef CSG_WIN
#include "engine/net/socket/WinSocket.h"
namespace csg
{
	class CChatServer
	{
	public:
		CChatServer(int af ,int type ,int _protocol ,int port ,HANDLE completionPort);

		virtual ~CChatServer();

		void startRun();

	protected:

		static void run(CChatServer*);

		void doRun();

	private:

		HANDLE _completionPort;
		int _af;
		int _type;
		int _protocol;
		int _port;
		int _listenfd;
	};

}

#endif

#endif