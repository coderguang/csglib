#include "engine/thread/threadManager.h"
#include "thread.h"

using namespace csg;

void csg::CThreadManager::addTask(CTask& task)
{
	_taskList.push_back(&task);
}

void csg::CThreadManager::activeBaseServer(EBaseServerDef type,std::string dir)
{
	if ( EBaseServerUpdateDt&type )
	{
		activeUpdateDtServer();
	}
	if ( EBaseServerLoggerWritter&type )
	{
		activeLogServer(dir);
	}
	if ( EBaseServerMainLogic&type)
	{
		activeMainLogiceServer();
	}
	if ( EBaseServerSocketSend&type )
	{
		activeSocketSendServer();
	}

}

void csg::CThreadManager::stopTaskByType(EBaseServerDef type)
{
	for ( std::vector<CTask*>::iterator it = _taskList.begin(); it != _taskList.end(); ++it )
	{
		if ( (*it)->getTaskType()&type )
		{
			( *it )->setStop();
		}
	}
}

void csg::CThreadManager::runTaskFromStopByType(EBaseServerDef type)
{
	for ( std::vector<CTask*>::iterator it = _taskList.begin(); it != _taskList.end(); ++it )
	{
		if ( ( *it )->getTaskType()&type)
		{
			( *it )->runFromStop();
		}
	}
}

void csg::CThreadManager::killTaskByType(EBaseServerDef type)
{
	for ( std::vector<CTask*>::iterator it = _taskList.begin(); it != _taskList.end(); )
	{
		if ( ( *it )->getTaskType()&type)
		{
			( *it )->setExit();
			it = _taskList.erase(it);
		} else
		{
			++it;
		}
	}
}

void csg::CThreadManager::stopAllTask()
{
	for ( std::vector<CTask*>::iterator it = _taskList.begin(); it != _taskList.end(); ++it )
	{
		( *it )->setStop();
	}
}

void csg::CThreadManager::runAllTaskFromStop()
{
	for ( std::vector<CTask*>::iterator it = _taskList.begin(); it != _taskList.end(); ++it )
	{
		( *it )->runFromStop();
	}
}

void csg::CThreadManager::killAllTask()
{
	for ( std::vector<CTask*>::iterator it = _taskList.begin(); it != _taskList.end(); ++it )
	{
		( *it )->setExit();
	}
	_taskList.clear();
}

void csg::CThreadManager::activeUpdateDtServer()
{
	if ( _updateDateServer.isStart() )
		return;
	_updateDateServer.active(CSERVER_UPDATE_DATE_AMOUNT);
	addTask(_updateDateServer);
}

void csg::CThreadManager::activeLogServer(std::string& dir)
{
	if ( _logServer.isStart() )
		return;
#ifdef _DEBUG
	_logServer.setConsoleInfo(true);
#endif
	_logServer.setFileDir(dir);
	_logServer.active(CSERVER_LOG_WRITTER_AMOUNT);
	addTask(_logServer);
}

void csg::CThreadManager::activeMainLogiceServer()
{
	if ( _mainLogiceServer.isStart() )
		return;
	_mainLogiceServer.active(CSERVER_MAIN_LOGIC_AMOUNT);
	addTask(_mainLogiceServer);
}

void csg::CThreadManager::activeSocketSendServer()
{
	if ( _socketSendServer.isStart() )
		return;
	_socketSendServer.active(CSERVER_SOCKET_SEND_AMOUNT);
	addTask(_socketSendServer);

}
