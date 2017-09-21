
#ifndef _ENGINE_THREAD_TASK_H_
#define _ENGINE_THREAD_TASK_H_

#include "thread.h"
#include <condition_variable>
#include "../baseServer/baseServerDef.h"
#include "framework/SmartPoint/smartPoint.h"

namespace csg
{
	// 所有线程,均由 threadManager 管理
	class CTask
	{
		friend class CThreadManager;
	protected:

		virtual void active(int threadCount = 1);
		
		//结束线程
		virtual int setExit();

		//停止线程
		virtual int setStop();

		//从停止中恢复
		virtual int runFromStop();

		virtual EBaseServerDef getTaskType()const;
	protected:

		CTask() :_exit(false) ,_stop(false) ,_isStart(false)
		{
		};

		virtual bool isExit()const;

		virtual bool isStop()const;

		virtual bool isStart()const;

		CLock& getLock();

		virtual void setTaskType(EBaseServerDef type);

	protected:
		std::condition_variable _cv; //条件变量

	private:
		//线程执行函数
		static int svc_run(void* task);

		//实际逻辑执行函数
		virtual int run();
	private:
		CTask(const CTask&)=delete;

		CTask& operator =( const CTask& ) = delete;

	private:
		csg_atomic_boolean _exit; //退出标志位
		csg_atomic_boolean _stop; //暂停标志位

		csg_atomic_boolean _isStart; //启动标志
		csg_atomic_int _threadCount; //线程数

		CLock _lock;
		
		EBaseServerDef _serverType;
	};
}
#endif