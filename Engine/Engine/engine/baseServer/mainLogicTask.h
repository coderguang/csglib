
#ifndef _ENGINE_BASE_SERVER_MAIN_LOGIC_H_
#define _ENGINE_BASE_SERVER_MAIN_LOGIC_H_
#include "../thread/task.h"
#include "baseServerDef.h"

namespace csg
{
	class CMainLogicTask :public virtual CTask
	{
	public:
		CMainLogicTask() :CTask()
		{
			setTaskType(EBaseServerMainLogic);
		}
		virtual ~CMainLogicTask()
		{
		}
		virtual int run();
	};
}
#endif