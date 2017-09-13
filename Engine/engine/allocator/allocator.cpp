#include "engine/allocator/allocator.h"
#include <stdlib.h>
#include "engine/lock/lockObjectManager.h"
#include "engine/def/csg_def.h"

csg::CAllocator* csg::CAllocator::_alloc = NULL;

void *csg::CAllocator::malloc(size_t size)
{
	void *p = ::malloc(size);  //use stdlib.h
	if ( p )
	{
		//_count++;
	}
	return p;
}

void *csg::CAllocator::realloc(void *tmp ,size_t newSize)
{
	void *p = ::realloc(tmp ,newSize);
	return p;
}

void csg::CAllocator::free(void *buf)
{
	::free(buf);
}


void csg::CAllocator::gc()
{
}

csg::CAllocator* csg::CAllocator::instance()
{
	if ( NULL == _alloc )
	{
		CAutoLock lock(CLockObjectManager::getSingletonLock());
		if ( NULL == _alloc )
		{
			_alloc = new CAllocator();
		}

	}
	return _alloc;
}

csg::CAllocator* csg::CAllocator::instance(CAllocator* alloc)
{
	CAutoLock lock(CLockObjectManager::getSingletonLock());
	if ( NULL != _alloc )
	{
		delete _alloc;
		_alloc = alloc;
	}
	return _alloc;
}