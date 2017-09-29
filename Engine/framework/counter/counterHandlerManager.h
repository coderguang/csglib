#ifndef _FRAMEWORK_UTIL_COUNTERHANDLER_MANAGER_H_
#define _FRAMEWORK_UTIL_COUNTERHANDLER_MANAGER_H_

#include <list>
#include "framework/counter/counterHandler.h"
#include "framework/DesignPattern/Singleton.h"
#include "framework/io/IOHandler.h"
#include "../timer/timer.h"

namespace csg
{

	class CCounterHandlerTimer :public virtual CTimerBase
	{
	public:
		CCounterHandlerTimer() :CTimerBase("CCounterHandlerTimer")
		{
		}

		virtual int handleTimeOut(
			CDateTime& current);

	};
	typedef CSmartPointShare<CCounterHandlerTimer> CCounterHandlerTimerPtr;


	class CCounterHandlerManager
	{
	public:
	
		static void registCounterHandler( CCounterHandler* handler);

		static void removeCounterHandler( CCounterHandler* handler);

		static void printCounterHandler();

		static void initPrintCounterTimer();

	private:
		static std::list<CCounterHandler*>& getCounterHandler();
	};



}
#endif