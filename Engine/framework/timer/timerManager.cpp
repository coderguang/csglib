#include "framework/timer/timerManager.h"
#include "../DesignPattern/objectPool.h"
#include "../datetime/datetime.h"

using namespace csg;

void csg::CTimerManager::addTimer(CTimerBasePtr timer ,CDateTime runTime ,CInterval loopTime)
{
	CAutoRecursiveLock l(getLock());
	if ( !timer )
	{
		assert(false);
		return;
	}

	CTimeNodePtr timerNode = CObjectPool<CTimeNode>::instance()->newObject();
	timerNode->setTimer(timer ,runTime ,loopTime);
	addTimeNode(timerNode);
}

void csg::CTimerManager::addTimer(CTimerBasePtr timer ,CInterval afterTime ,CInterval loopTime)
{
	CAutoRecursiveLock l(getLock());
	CDateTime runTime = CDateTime() + afterTime;
	addTimer(timer ,runTime,loopTime);
}

void csg::CTimerManager::triggerTimer(CDateTime& currentDt)
{
	CAutoRecursiveLock l(getLock());
	if ( _mapTimer.size() <= 0 )
	{
		return;
	}
	if ( currentDt < _mapTimer.begin()->first )
	{
		return;
	}

	ListTimer listTimer;
	std::swap(_mapTimer.begin()->second,listTimer);
	_mapTimer.erase(_mapTimer.begin());

	for ( ListTimer::iterator it = listTimer.begin(); it != listTimer.end(); ++it )
	{
		CTimeNodePtr timeNode = *it;
		if ( !timeNode )
		{
			continue;
		}
		if ( !timeNode->getTimer() )
		{
			continue;
		}
		if ( timeNode->isCancel() )
		{
			continue;
		}

		CTimerBasePtr timer = timeNode->getTimer();
		if ( !timer )
		{
			continue;
		}
		try
		{
			timer->handleTimeOut(currentDt);
		}
		catch ( const CException& ex)
		{
			timer->handleException(ex);
		}
		catch ( ... )
		{
			CSG_LOG_ERR("CTimerManager::triggerTimer" << " unknow error,name="<<timer->getTimerName());
		}
		if ( CInterval::_zero != timeNode->getLoopTime() )
		{
			timeNode->setNextRunTime(currentDt + timeNode->getLoopTime());
			addTimeQueue(timeNode);
		} else
		{
			//ÒÆ³ý¶¨Ê±Æ÷
			removeTimer(timer);
		}
	}
	listTimer.clear();
}

void csg::CTimerManager::cancelTimer(CTimerBasePtr timer)
{
	CAutoRecursiveLock l(getLock());
	MapTimerHandler::iterator it = _mapHandler.find(timer);
	if ( it != _mapHandler.end() )
	{
		it->second->setCancel(true);
		removeTimer(timer);
	}

}

void csg::CTimerManager::addTimeNode(CTimeNodePtr& timerNode)
{

	assert(_mapHandler.find(timerNode->getTimer()) == _mapHandler.end());
	_mapHandler[timerNode->getTimer()] = timerNode;
	addTimeQueue(timerNode);
}

void csg::CTimerManager::addTimeQueue(CTimeNodePtr& timerNode)
{
	MapTimer::iterator it = _mapTimer.find(timerNode->getRunTime());
	if ( it == _mapTimer.end() )
	{
		ListTimer listTimer;
		listTimer.push_back(timerNode);
		_mapTimer[timerNode->getRunTime()] = listTimer;
	} else
	{
		it->second.push_back(timerNode);
	}
}

void csg::CTimerManager::removeTimer(CTimerBasePtr timer)
{
	MapTimerHandler::iterator it = _mapHandler.find(timer);
	if ( it != _mapHandler.end() )
	{
		_mapHandler.erase(it);
	}
}

CRecursiveLock& csg::CTimerManager::getLock()
{
	return _lock;
}

