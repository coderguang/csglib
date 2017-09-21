#include "engine/rpc/rmiObject.h"
#include "framework/log/Log.h"


using namespace csg;

CCounterHandler CRMIObject::_count("CRMIObject");
CCounterHandler CRMIProxyObject::_count("CRMIProxyObject");
CCounterHandler CRMIProxyCallBackObject::_count("CRMIProxyCallBackObject");
CCounterHandler CRMIObjectBind::_count("CRMIProxyCallBackBindObject");

csg::CRMIObject::CRMIObject()
{
	_count.increaseCount(1);
}

csg::CRMIObject::~CRMIObject()
{
	_count.decreaseCount(1);
}

csg::ERMIDispatchResult csg::CRMIObject::__onCall(const CSessionPtr& session ,const SRMICall& rmiCall ,CSerializeStream& inStream)
{
	return ERMIDispatchResultOk;
}

void csg::CRMIObject::__response(CSerializeStream& stream)
{

}

void csg::CRMIObject::__responseException(const CException& ex)
{

}

const MapRMIInfo& csg::CRMIObject::getRMIInfos()const
{
	return _mapRMIInfo;
}



const SetRpcId& csg::CRMIObject::getRpcIds() const
{
	return _rpcIdset;
}


void csg::CRMIObject::__responseException()
{

}

void csg::CRMIObject::__responseException(const std::exception& ex)
{

}

bool csg::CRMIObject::addRMIInfo(int rpcId ,SRMIInfo info)
{
	if ( isHaveRMIInfo(rpcId) )
	{
		CSG_LOG_ERR("csg::CRMIObject::addRMIInfo"<<",conflict rpcId,id="<<rpcId);
		assert(false);
		return false;
	}
	if ( isHaveRMIInfo(info) )
	{
		CSG_LOG_ERR("csg::CRMIObject::addRMIInfo" << ",conflict rmiInfo,identify=" <<info.identify<<",operator="<<info.operation);
		assert(false);
		return false;
	}
	_checkRMIInfo.insert(info);
	_mapRMIInfo[rpcId] = info;
	return true;
}

bool csg::CRMIObject::isHaveRMIInfo(SRMIInfo info)
{
	return _checkRMIInfo.count(info) >0;
}

bool csg::CRMIObject::addRpcId(int rpcId)
{
	if ( _rpcIdset.count(rpcId) > 0 )
	{
		CSG_LOG_ERR("CRMIObject::addRpcId conflict rpcId="<<rpcId);
		assert(false);
		return false;
	}
	_rpcIdset.insert(rpcId);
	return true;
}

bool csg::CRMIObject::isHaveRMIInfo(int rpcId)
{
	MapRMIInfo::iterator it = _mapRMIInfo.find(rpcId);
	if ( it != _mapRMIInfo.end() )
	{
		return true;
	}
	return false;
}


csg::CRMIProxyObject::CRMIProxyObject()
{
	_count.increaseCount(1);
}

csg::CRMIProxyObject::~CRMIProxyObject()
{
	_count.decreaseCount(1);
}


void csg::CRMIProxyObject::setSession(const CSessionPtr& session ,const SRMICall& rmiCall)
{
	_session = session;
	_rmiCall = rmiCall;
}

CSessionPtr& csg::CRMIProxyObject::getSession()
{
	return _session;
}

int csg::CRMIProxyObject::getMessageId() const
{
	return _rmiCall.messageId;
}

csg::CRMIProxyCallBackObject::CRMIProxyCallBackObject()
{
	_count.increaseCount(1);
}

csg::CRMIProxyCallBackObject::~CRMIProxyCallBackObject()
{
	_count.decreaseCount(1);
}

bool csg::CRMIObjectBind::bindProxyCallBackObject(const CRMIProxyCallBackObjectPtr& object)
{
	if ( _callBack )
	{
		assert(false);
		return false;
	}
	_callBack = object;
	return true;
}

void csg::CRMIObjectCallBackObject::setSession(const CSessionPtr& session ,const SRMICall& rmiCall)
{
	_session = session;
	_rmiCall = rmiCall;
}
