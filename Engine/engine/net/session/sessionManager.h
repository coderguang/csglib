
#ifndef _ENGINE_NET_SESSION_SESSION_MANAGER_H_
#define _ENGINE_NET_SESSION_SESSION_MANAGER_H_

#include "framework/DesignPattern/Singleton.h"
#include "session.h"
#include <map>
#include "../../lock/lock.h"

namespace csg
{

	typedef std::map<int ,CSessionPtr> MapSession;

	class CSessionManager :public virtual Singleton<CSessionManager>
	{
	public:
		bool addSession(CSessionPtr& session);

		bool removeSession(int socketfd);

		CSessionPtr getSession(int socketfd);

		bool onRecvData(int socketfd ,const void* buf ,int len);

		void runNetMsgLoop();

	protected:
		CRecursiveLock& getLock();

		bool isHaveSession(int socketfd);
	private: 


	private:
		MapSession _sessionMap; // <socketfd,data>
	};

}
#endif