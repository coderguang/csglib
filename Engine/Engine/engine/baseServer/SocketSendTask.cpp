#include "engine/baseServer/SocketSendTask.h"
#include "framework/log/Log.h"
#include "../mq/msgQueue.h"
#include "../thread/thread.h"

using namespace csg;
int csg::CSocketSendTask::svn_run(void* task)
{
	CSocketSendTask *t = (CSocketSendTask*) task;
	if ( !t )
	{
		assert(false);
		return CSG_RETURN_ERROR;
	}
	t->run();
	return CSG_RETURN_OK;
}

int csg::CSocketSendTask::run()
{
	CSG_LOG_SYS("CSocketSendTask::start ");

	std::unique_lock<CLock> lock(getLock());
	while ( !isExit() )
	{
		if ( isStop() )
		{
			CSG_LOG_SYS("CSocketSendTask::run waiting");
			_cv.wait(lock);
		}
		CMsgQueue::instance()->runLoop();
		CThread::sleep_for(100);
	}
	CMsgQueue::instance()->runLoop();
	CSG_LOG_SYS("CSocketSendTask::run exit");
	return 0;
}

