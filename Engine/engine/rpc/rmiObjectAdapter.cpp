#include "engine/rpc/rmiObjectAdapter.h"

using namespace csg;

bool csg::CRMIObjectAdapter::addRmiObject(const std::string& endPoint,const CRMIObjectPtr& rmiObject)
{
	CAutoLock l(getLock());
	assert(rmiObject);
	MapEndPointRMIObject::iterator endPointIter = _endPointRmiObjectMap.find(endPoint);

	const SetRpcId& setRpcId = rmiObject->getRpcIds();

	if ( endPointIter == _endPointRmiObjectMap.end() )
	{
		MapRMIObject tmpMap;
		_endPointRmiObjectMap[endPoint] = tmpMap;
	} 

	endPointIter = _endPointRmiObjectMap.find(endPoint);
	
	assert(endPointIter != _endPointRmiObjectMap.end());

	for ( SetRpcId::iterator it = setRpcId.begin(); it != setRpcId.end(); it++ )
	{
		MapRMIObject::iterator idIter = endPointIter->second.find(*it);
		if ( idIter != endPointIter->second.end() )
		{
			CSG_LOG_ERR("CRMIObjectAdapter::addRmiObject,conflict proxy,name="<<endPoint<<",rpcId="<<*it);
			assert(false);
			return false;
		}
		endPointIter->second[*it] = rmiObject;
	}
	return true;
}

bool csg::CRMIObjectAdapter::findRmiObject(const std::string& endPoint ,MapRMIObject& mapRmiObject)
{
	CAutoLock l(getLock());
	MapEndPointRMIObject::iterator it = _endPointRmiObjectMap.find(endPoint);
	if ( it == _endPointRmiObjectMap.end() )
	{
		return false;
	}
	mapRmiObject = it->second;
	return true;
}

CLock& csg::CRMIObjectAdapter::getLock()
{
	return _lock;
}

