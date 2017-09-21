#ifndef _ENGINE_RPC_RMI_OBJECT_ADAPTER_H_
#define _ENGINE_RPC_RMI_OBJECT_ADAPTER_H_

#include <map>
#include "rmiObject.h"
#include "framework/DesignPattern/Singleton.h"
#include "../lock/lock.h"

namespace csg
{
	class CRMIObject;

	typedef CSmartPointShare<CRMIObject> CRMIObjectPtr;

	typedef std::map<int ,CRMIObjectPtr> MapRMIObject;

	typedef std::map<std::string ,MapRMIObject> MapEndPointRMIObject; //区分哪些连接可以使用哪些接口

	class CRMIObjectAdapter :public virtual Singleton<CRMIObjectAdapter>
	{
	public:

		bool addRmiObject(const std::string& endPoint ,const CRMIObjectPtr& rmiObject);

		bool findRmiObject(const std::string& endPoint ,MapRMIObject& mapRmiObject);


	protected:
		CLock& getLock();

	private:
		CLock _lock;
		MapEndPointRMIObject _endPointRmiObjectMap;
	};

}
#endif