#include "framework/counter/counterHandler.h"
#include "framework/counter/counterHandlerManager.h"
#include "engine/lock/lock.h"
#include "engine/lock/lockObjectManager.h"
#include "../timer/timerManager.h"

using namespace csg;

void CCounterHandlerManager::registCounterHandler(CCounterHandler* handler)
{
	CAutoLock lock(CLockObjectManager::getCounterHandlerLock());
	CCounterHandlerManager::getCounterHandler().push_back(handler);
}

void CCounterHandlerManager::removeCounterHandler( CCounterHandler* handler)
{
	CAutoLock lock(CLockObjectManager::getCounterHandlerLock());
	CCounterHandlerManager::getCounterHandler().remove(handler);
}

std::list<CCounterHandler*>& CCounterHandlerManager::getCounterHandler()
{
	static std::list<CCounterHandler*> counterHandler;
	return counterHandler;
}

void CCounterHandlerManager::printCounterHandler()
{
	CAutoLock lock(CLockObjectManager::getCounterHandlerLock());
	for ( std::list<CCounterHandler*>::iterator iter = CCounterHandlerManager::getCounterHandler().begin(); iter != CCounterHandlerManager::getCounterHandler().end(); iter++)
 	{
		( *iter )->print();
	}
}

void csg::CCounterHandlerManager::initPrintCounterTimer()
{
	CCounterHandlerTimerPtr timer = new CCounterHandlerTimer();

	CTimerManager::instance()->addTimer(timer ,CInterval(60 ,0) ,CInterval(60 ,0));

}

int csg::CCounterHandlerTimer::handleTimeOut(CDateTime& current)
{
	CCounterHandlerManager::printCounterHandler();
	return 0;
}
