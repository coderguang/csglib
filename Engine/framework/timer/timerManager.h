#ifndef _FRAMEWORK_TIMER_TIMER_MANAGER_H_
#define _FRAMEWORK_TIMER_TIMER_MANAGER_H_

#include "../DesignPattern/Singleton.h"
#include <vector>
#include "timer.h"
#include <map>
#include "timeNode.h"
#include "../datetime/datetime.h"

namespace csg
{

	typedef std::vector<CTimeNodePtr> ListTimer;
	typedef std::map<CDateTime ,ListTimer> MapTimer;
	typedef std::map<CTimerBasePtr ,CTimeNodePtr> MapTimerHandler;

	class CTimerManager :public virtual Singleton<CTimerManager>
	{
	public:
		void addTimer(CTimerBasePtr timer ,CDateTime runTime ,CInterval loopTime);

		void addTimer(CTimerBasePtr timer ,CInterval afterTime ,CInterval loopTime);

		void triggerTimer(CDateTime& currentDt);

		void cancelTimer(CTimerBasePtr timer);

	protected:
		void addTimeNode(CTimeNodePtr& timerNode);

		void addTimeQueue(CTimeNodePtr& timerNode);

		void removeTimer(CTimerBasePtr timer);

		CRecursiveLock& getLock();
	private:
		MapTimer _mapTimer;
		MapTimerHandler _mapHandler;
		CRecursiveLock _lock;
	};



}
#endif