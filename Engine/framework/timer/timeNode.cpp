#include "framework/timer/timer.h"
#include "framework/timer/timeNode.h"

using namespace csg;
csg::CTimeNode::CTimeNode() :_timer(NULL) ,_canDelete(false) ,_isCancel(false)
{

}

csg::CTimeNode::~CTimeNode()
{

}

void csg::CTimeNode::setTimer(CTimerBasePtr& timer ,CDateTime runTime ,CInterval loopTime)
{
	_timer = timer;
	setNextRunTime(runTime);
	setLoopTime(loopTime);
}

void csg::CTimeNode::setTimer(CTimerBasePtr& timer ,CInterval afterTime ,CInterval loopTime)
{
	_timer = timer;
	CDateTime runTime = CDateTime() + afterTime;
	setNextRunTime(runTime);
	setLoopTime(loopTime);
}

CTimerBasePtr& csg::CTimeNode::getTimer()
{
	return _timer;
}

void csg::CTimeNode::setCanDelete(bool canDelete)
{
	_canDelete = canDelete;
}

void csg::CTimeNode::setNextRunTime(CDateTime dt)
{
	_runTime = dt;
}

CDateTime& csg::CTimeNode::getRunTime()
{
	return _runTime;
}

void csg::CTimeNode::setLoopTime(CInterval dt)
{
	_loopTime = dt;
}

CInterval& csg::CTimeNode::getLoopTime()
{
	return _loopTime;
}

bool csg::CTimeNode::canDelete() const
{
	return _canDelete;
}

bool csg::CTimeNode::isCancel() const
{
	return _isCancel;
}

void csg::CTimeNode::setCancel(bool cancel)
{
	_isCancel = cancel;
}

