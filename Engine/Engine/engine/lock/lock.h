
#ifndef _ENGINE_LOCK_LOCK_H_
#define _ENGINE_LOCK_LOCK_H_

#include "engine/def/csg_def.h"
#include <mutex>



namespace csg
{
	//
	/*
	class CLock
	{
	public:
		
		CLock()
		{

		}
		
		virtual ~CLock()
		{

		}

		void lock()
		{
			_mutex.lock();
		}
		 void unlock()
		{
			_mutex.unlock();
		}
		
	private:
		CLock(const CLock&)
		{
		
		}
		std::mutex _mutex;
	};
	*/

	typedef std::mutex CLock;

	typedef std::recursive_mutex CRecursiveLock;
	/*
	template<typename T>
	class CAutoLockT
	{
	public:
		CAutoLockT(T* lock) :_lock(lock)
		{
			_lock->lock();
		}
		CAutoLockT(T &lock) :_lock(&lock)
		{
			_lock->lock();
		}
		virtual ~CAutoLockT()
		{
			_lock->unlock();
		}	
	private:
		CAutoLockT(const CAutoLockT&)
		{
		
		}
		T* _lock;
	};
	*/

	//typedef CAutoLockT<CLock> CAutoLock;

	typedef std::lock_guard<CLock> CAutoLock;

	typedef std::lock_guard<CRecursiveLock> CAutoRecursiveLock;

}
#endif