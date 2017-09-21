
#ifndef _ENGINE_LOCK_LOCK_OBJECT_MANAGER_H_
#define _ENGINE_LOCK_LOCK_OBJECT_MANAGER_H_

#include "engine/lock/lock.h"


namespace csg
{
	class CLockObjectManager
	{

	public:
		static CLock& getSingletonLock();

		static CLock& getAtomicLock();

		static CLock& getCounterHandlerLock();

		static CLock& getLogLock();

	private:
		CLockObjectManager();
		virtual ~CLockObjectManager()
		{
		};
	};

}
#endif