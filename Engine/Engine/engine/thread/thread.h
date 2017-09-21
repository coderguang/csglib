#ifndef _ENGINE_THREAD_THREAD_H_
#define _ENGINE_THREAD_THREAD_H_

#include <thread>
#include "framework/datetime/datetime.h"

namespace csg
{

	typedef int(*CSG_THREAD_FUN)( void* );

	typedef std::thread csg_thread;
	typedef std::thread::id csg_thread_id;

	class CThread
	{
	public:
		
		static int threadId();

		static int pid();

		static void sleep_for(long64_t milliseconds);

		static void sleep_until(CDateTime dt);

		static void yeild();

	private:

		static csg_thread_id get_id();

	private:
		//warnning,join 会阻塞主线程
		static void join(csg_thread& thr);

		static void deatch(csg_thread& thr);
	};



}
#endif