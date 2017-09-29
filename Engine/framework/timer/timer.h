#ifndef _FRAMEWORK_TIMER_TIMER_H_
#define _FRAMEWORK_TIMER_TIMER_H_

#include "../SmartPoint/refObject.h"
#include "../SmartPoint/smartPoint.h"
#include <string>


namespace csg
{

	class CTimerBase :public virtual CRefObject
	{
	public:
		CTimerBase(std::string name);

		virtual ~CTimerBase();

		virtual int handleTimeOut(CDateTime& current);

		virtual void handleException(const CException& ex);

		bool operator <( const CTimerBase& other );

		std::string getTimerName()const;

	private:
		std::string _timerName;
	};

	typedef CSmartPointShare<CTimerBase> CTimerBasePtr;

}
#endif