#ifndef _TEST_TEST_TIMER_H_
#define _TEST_TEST_TIMER_H_

#include "framework/timer/timer.h"
#include "framework/SmartPoint/smartPoint.h"
#include "framework/timer/timerManager.h"
#include "framework/exception/exception.h"


using namespace csg;

class CTestTimer :public virtual CTimerBase
{
public:
	CTestTimer() :CTimerBase("CTestTimer") 
	{
	}
	virtual int handleTimeOut(CDateTime& current);

	virtual void handleException(const CException& ex);

	int flag;
	std::string _name;
};

typedef CSmartPointShare<CTestTimer> CTestTimerPtr;

int CTestTimer::handleTimeOut(CDateTime& current)
{
	CSG_LOG_INFO("CTestTimer::handleTimeOut,flag="<<flag<<",name="<<_name<<",dt="<<current.asString());
	if ( _name == "3333" )
	{
		flag++;
		if ( 5 == flag )
		{
			CTimerManager::instance()->cancelTimer(this);
			CSG_LOG_INFO("CTestTimer::handleTimeOut,cancel flag=" << flag << ",name=" << _name << ",dt=" << current.asString());
			throw CException("test timer");
		}
	}
	return 0;
}

void CTestTimer::handleException(const CException& ex)
{
	CSG_LOG_INFO(" CTestTimer::handleException,flag=" << flag << ",name=" << _name << ",dt=" << CDateTime().asString() << ",ex.what=" << ex.what());;
}

void testTimer()
{
	CTestTimerPtr timer = new CTestTimer();
	timer->flag = 1;
	timer->_name = "11111";
	CTimerManager::instance()->addTimer(timer ,CInterval(2 ,0) ,CInterval::_zero);


	CTestTimerPtr timer2 = new CTestTimer();
	timer2->flag = 2;
	timer2->_name = "222222";
	CTimerManager::instance()->addTimer(timer2 ,CInterval(5 ,0) ,CInterval(2,0));

	CTestTimerPtr timer3 = new CTestTimer();
	timer3->flag = 3;
	timer3->_name = "3333";
	CTimerManager::instance()->addTimer(timer3 ,CInterval(10 ,0) ,CInterval(5,0));



}

#endif