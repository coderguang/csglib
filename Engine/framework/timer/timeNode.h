#ifndef _FRAMEWORK_TIMER_H_
#define _FRAMEWORK_TIMER_H_

#include "../SmartPoint/refObject.h"
#include "timer.h"

namespace csg
{
	class CTimeNode :public virtual CRefObject
	{
	public:
		CTimeNode();

		virtual ~CTimeNode();

		void setTimer(CTimerBasePtr& timer,CDateTime runTime,CInterval loopTime);

		void setTimer(CTimerBasePtr& timer ,CInterval afterTime ,CInterval loopTime);

		
		CTimerBasePtr& getTimer();

		void setCanDelete(bool canDelete);

		void setNextRunTime(CDateTime dt);

		CDateTime& getRunTime();

		void setLoopTime(CInterval dt);

		CInterval& getLoopTime();

		bool canDelete()const;

		bool isCancel()const;

		void setCancel(bool cancel);

	private:
		CTimerBasePtr _timer;
		CDateTime _runTime;
		CInterval _loopTime;
		bool _canDelete;
		bool _isCancel;
	};

	typedef CSmartPointShare<CTimeNode> CTimeNodePtr;
}
#endif