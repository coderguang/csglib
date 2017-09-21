#include "engine/baseServer/mainLogicTask.h"
#include "../def/csg_def.h"
#include "../net/session/sessionManager.h"

int csg::CMainLogicTask::run()
{
	CSG_LOG_SYS("CMainLogicTask::run start");
	std::unique_lock<CLock> lock(getLock());
	while ( !isExit() )
	{
		if ( isStop() )
		{
			CSG_LOG_SYS("CMainLogicTask::run waiting");
			_cv.wait(lock);
		}
		//网络消息处理
		CSessionManager::instance()->runNetMsgLoop();


		CThread::sleep_for(100);
	}
	CSG_LOG_SYS("CMainLogicTask::run exit");
	return CSG_RETURN_OK;
}

