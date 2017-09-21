#include "framework/SmartPoint/refObject.h"
#include "../log/Log.h"
#include "../util/typetransform.h"

using namespace csg;

CCounterHandler CRefObject::_handlerCount("CRefObjec");

csg::CRefObject::CRefObject() :_noDelete(false) ,_refCount(0)
{
	//CSG_LOG_DEBUG("CRefObjec::CRefObjec create");
	_handlerCount.increaseCount();
}


CRefObject& csg::CRefObject::operator=( const CRefObject& other )
{
	return *this;
}

csg::CRefObject::~CRefObject()
{
	//CSG_LOG_DEBUG("CRefObjec::CRefObjec destroy");
	_handlerCount.decreaseCount();
}

bool csg::CRefObject::decRef()
{
	_refCount -= 1;
	if ( 0 == _refCount )
	{
		if ( !_noDelete )
		{
			delete this;
		}
		return true;
	} else
		return false;
}

void csg::CRefObject::incRef()
{
	_refCount += 1;
}

int csg::CRefObject::getRef() const
{
	return _refCount;
}

void csg::CRefObject::setNoDelete(bool b)
{
	_noDelete = b;
}

bool csg::CRefObject::getNoDelete() const
{
	return _noDelete;
}

