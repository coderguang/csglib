#include "engine/lock/lockObjectManager.h"


csg::CLock singleLockLock;
csg::CLock atomicLockLock;
csg::CLock counterLockLock;
csg::CLock logLockLock;

csg::CLockObjectManager::CLockObjectManager()
{
	CLockObjectManager::getSingletonLock();
	CLockObjectManager::getAtomicLock();
	CLockObjectManager::getCounterHandlerLock();
	CLockObjectManager::getLogLock();
}

csg::CLock& csg::CLockObjectManager::getSingletonLock()
{
	CAutoLock lock(singleLockLock);
	static csg::CLock singleLock;
	return singleLock;
}

csg::CLock& csg::CLockObjectManager::getAtomicLock()
{
	CAutoLock lock(atomicLockLock);
	static CLock atomicLock;
	return atomicLock;
}

csg::CLock& csg::CLockObjectManager::getCounterHandlerLock(){

	CAutoLock lock(counterLockLock);
	static CLock counterHandlerLock;
	return counterHandlerLock;
}

csg::CLock& csg::CLockObjectManager::getLogLock()
{
	CAutoLock lock(logLockLock);
	static csg::CLock logLock;
	return logLock;
}
