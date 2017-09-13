
#ifndef _ENGINE_BASE_SERVER_UPDATE_DT_TASK_H_
#define _ENGINE_BASE_SERVER_UPDATE_DT_TASK_H_

#include "../thread/task.h"
#include "baseServerDef.h"

namespace csg
{
	class CUpdateDtTask :public virtual CTask
	{
	public:
		CUpdateDtTask() :CTask()
		{
			setTaskType(EBaseServerUpdateDt);
		}
		virtual ~CUpdateDtTask()
		{
		};
	protected:
		virtual int run();
	};


}
#endif
