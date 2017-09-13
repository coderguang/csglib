#include "framework/counter/counterHandler.h"
#include "framework/counter/counterHandlerManager.h"
#include "engine/lock/lock.h"
#include "engine/lock/lockObjectManager.h"

using namespace csg;

void CCounterHandlerManager::registCounterHandler(CCounterHandler* handler)
{
	CAutoLock lock(CLockObjectManager::getCounterHandlerLock());
	CCounterHandlerManager::getCounterHandler().push_back(handler);
}

void CCounterHandlerManager::removeCounterHandler( CCounterHandler* handler)
{
	CAutoLock lock(CLockObjectManager::getCounterHandlerLock());
	CCounterHandlerManager::getCounterHandler().remove(handler);
}

std::list<CCounterHandler*>& CCounterHandlerManager::getCounterHandler()
{
	static std::list<CCounterHandler*> counterHandler;
	return counterHandler;
}

void CCounterHandlerManager::printCounterHandler(IIOHandler* handle)
{
	CAutoLock lock(CLockObjectManager::getCounterHandlerLock());
	for ( std::list<CCounterHandler*>::iterator iter = CCounterHandlerManager::getCounterHandler().begin(); iter != CCounterHandlerManager::getCounterHandler().end(); iter++)
 	{
		( *iter )->print(handle);
	}
}