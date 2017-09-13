#ifndef _ENGINE_RPC_RMI_DEF_H_
#define _ENGINE_RPC_RMI_DEF_H_

#include "../serialize/serializestream.h"
#include <map>

namespace csg
{
	enum EMessageType
	{
		EMessageTypeMQ = 1 ,
		EMessageTypeCall = 2 ,
		EMessageTypeCallRet = 3 ,
	};

	void _csg_write(CSerializeStream& __os ,EMessageType __messageType);
	void _csg_read(CSerializeStream& __is ,EMessageType& __messageType);

	
	enum ERMIDispatchResult
	{
		DispatchResultOk=1,
		DispatchResultTimeout=2,
		DispatchResultException=3,
		DispatchObjectNotExist=4,
	};

	void _csg_write(CSerializeStream& __os ,ERMIDispatchResult __result);
	void _csg_read(CSerializeStream& __is ,ERMIDispatchResult& __result);


	struct SRMIInfo
	{
		std::string identity;
		std::string operation;
	};
	
	typedef std::map<int ,SRMIInfo> MapRMIInfo;


}
#endif