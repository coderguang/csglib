#include "engine/mq/msgManager.h"
#include "framework/log/Log.h"
#include "framework/util/typetransform.h"


bool csg::CMsgManager::regist(const IMsgBasePtr& msgBase)
{
	CAutoLock l(getLock());
	MapMsgBase::iterator it = _msgMap.find(msgBase->getType());
	if ( it != _msgMap.end() )
	{
		CSG_LOG_ERR("CMsgManager::regist  duplicate type=" << ToStr(msgBase->getType()));
		return false;
	}
	_msgMap[msgBase->getType()] = msgBase;
	return true;
}

bool csg::CMsgManager::unRegist(int type)
{
	CAutoLock l(getLock());
	MapMsgBase::iterator it = _msgMap.find(type);
	if ( it != _msgMap.end() )
	{
		_msgMap.erase(it);
		return true;
	}
	return false;
}

csg::IMsgBasePtr csg::CMsgManager::getMsg(int type)
{
	CAutoLock l(getLock());
	MapMsgBase::iterator it = _msgMap.find(type);
	if ( it != _msgMap.end() )
	{
		return it->second;
	}
	CSG_LOG_ERR("CMsgManager::getMsg no type=" << ToStr(type));
	return NULL;
}

csg::IMsgBasePtr csg::CMsgManager::createMsg(int type)
{
	CAutoLock l(getLock());
	MapMsgBase::iterator it = _msgMap.find(type);
	if ( it != _msgMap.end() )
	{
		return it->second->clone();
	}
	CSG_LOG_ERR("CMsgManager::createMsg no type=" << ToStr(type));
	return NULL;
}

csg::CLock& csg::CMsgManager::getLock()
{
	return _lock;
}

