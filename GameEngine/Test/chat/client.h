#ifndef _TEST_CHAT_CLIENT_H_
#define _TEST_CHAT_CLIENT_H_
#include "engine/def/environment_def.h"
#include <string>
#include "framework/SmartPoint/smartPoint.h"
#include "framework/SmartPoint/refObject.h"

#ifdef CSG_WIN

namespace csg
{

	class CChatClient:public virtual CRefObject
	{
	public:
		CChatClient(std::string ipAddr,int port);

		virtual ~CChatClient();

		void startRun();

	private:

		static void startSend(CChatClient*);

		static void startRecv(CChatClient*);

		bool init();

		void doRecvChat();

		int doSend();

		int getSocketfd()
		{
			return _socketfd;
		}

		int _socketfd;
		std::string _ipAddr;
		int _port;
		std::atomic<bool> _stop;
	};

	typedef CSmartPointShare<CChatClient> CChatClientPtr;
}


#endif

#endif