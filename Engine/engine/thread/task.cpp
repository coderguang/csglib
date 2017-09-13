#include "engine/thread/task.h"
#include "../lock/lockObjectManager.h"

using namespace csg;

void csg::CTask::active(int threadCount /*= 1*/)
{
	for ( int i = 0; i < threadCount; i++ )
	{		
		csg_thread* _thread = new csg_thread(CTask::svc_run ,this);
		_thread->detach();
	}
	_isStart = true;
	_threadCount = threadCount;
}

int csg::CTask::setExit()
{
	if ( _exit )
	{
		assert(false);
		return CSG_RETURN_ERROR;
	}

	_exit = true;
	//Ç¿ÖÆ»½ÐÑ
	_cv.notify_all();
	return CSG_RETURN_OK;
}

int csg::CTask::setStop()
{
	if ( _stop )
		return CSG_RETURN_ERROR;
	_stop = true;
	return CSG_RETURN_OK;
}

int csg::CTask::run()
{
	return CSG_RETURN_OK;
}


bool csg::CTask::isExit()const
{
	return _exit;
}

bool csg::CTask::isStop()const
{
	return _stop;
}

bool csg::CTask::isStart() const
{
	return _isStart;
}

int csg::CTask::runFromStop()
{
	if ( _stop )
	{
		_stop = false;
		_cv.notify_all();
		return CSG_RETURN_OK;
	}
	return CSG_RETURN_ERROR;
}

EBaseServerDef csg::CTask::getTaskType() const
{
	return _serverType;
}

CLock& csg::CTask::getLock()
{
	return _lock;
}

void csg::CTask::setTaskType(EBaseServerDef type)
{
	_serverType = type;
}

int csg::CTask::svc_run(void* task)
{
	CTask *t = (CTask*) task;
	if ( !t )
	{
		assert(false);
		return CSG_RETURN_ERROR;
	}
	t->run();
	return CSG_RETURN_OK;
}


