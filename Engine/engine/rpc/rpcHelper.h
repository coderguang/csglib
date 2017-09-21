#ifndef _ENGINE_RPC_RPC_HELPER_H_
#define _ENGINE_RPC_RPC_HELPER_H_

#include "../serialize/serializestream.h"
#include "rmidef.h"
#include "rmiObject.h"
#include "../net/session/session.h"
#include "framework/DesignPattern/objectPool.h"


namespace csg
{
	class CRpcHelper
	{
	public:
		static void prepareToCall(const CSessionPtr& session ,CAutoSerializeStream& os ,SRMICall& call ,const CRMIProxyCallBackObjectPtr& backObject ,CRMIObjectBindPtr& objectBind);

		static void toCall(const CSessionPtr& session ,CAutoSerializeStream& os ,CRMIObjectBindPtr& objectBind);

		static void prepareToReturn(const CSessionPtr& session ,CAutoSerializeStream& os ,SRMIReturn& rmiReturn);

		static void toReturn(const CSessionPtr& session ,CAutoSerializeStream& os);

	};
}
#endif