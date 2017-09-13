#ifndef _ENGINE_THREAD_THREAD_MANAGER_H_
#define _ENGINE_THREAD_THREAD_MANAGER_H_

#include "framework/DesignPattern/Singleton.h"
#include "task.h"
#include <vector>
#include "thread.h"
#include "../baseServer/updateDtTask.h"
#include "../baseServer/loggerWritter.h"
#include <string>
#include "../baseServer/mainLogicTask.h"
#include "../baseServer/SocketSendTask.h"

namespace csg
{
	class CThreadManager :public virtual Singleton<CThreadManager>
	{
	public:
		void activeBaseServer(EBaseServerDef type,std::string dir="");

		void stopTaskByType(EBaseServerDef type);

		void runTaskFromStopByType(EBaseServerDef type);

		void killTaskByType(EBaseServerDef type);

		void stopAllTask();

		void runAllTaskFromStop();

		void killAllTask();

		void addTask(CTask& task);
	private:

		void activeUpdateDtServer();

		CUpdateDtTask _updateDateServer;

		void activeLogServer(std::string& dir);

		CLoggerWritterTask _logServer;

		void activeMainLogiceServer();

		CMainLogicTask _mainLogiceServer;

		void activeSocketSendServer();

		CSocketSendTask _socketSendServer;

	private:
		std::vector<CTask*> _taskList;


	};


}
#endif