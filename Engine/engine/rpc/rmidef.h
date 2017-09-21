#ifndef _ENGINE_RPC_RMI_DEF_H_
#define _ENGINE_RPC_RMI_DEF_H_

#include "../serialize/serializestream.h"
#include <map>

namespace csg
{
	enum ERMIMessageType
	{
		ERMIMessageTypeMQ = 1 ,
		ERMIMessageTypeCall = 2 ,
		ERMIMessageTypeCallRet = 3 ,
	};

	void _csg_write(CSerializeStream& __os ,ERMIMessageType __messageType);
	void _csg_read(CSerializeStream& __is ,ERMIMessageType& __messageType);

	
	enum ERMIDispatchResult
	{
		ERMIDispatchResultOk = 1 ,
		ERMIDispatchResultTimeout = 2 ,
		ERMIDispatchResultException = 3 ,
		ERMIDispatchObjectNotExist = 4 ,
	};

	void _csg_write(CSerializeStream& __os ,ERMIDispatchResult __result);
	void _csg_read(CSerializeStream& __is ,ERMIDispatchResult& __result);


	struct SRMIInfo
	{
		std::string identify;
		std::string operation;

		SRMIInfo();
		bool operator==( const SRMIInfo& ) const;
		bool operator!=( const SRMIInfo& ) const;
		bool operator<( const SRMIInfo& ) const;

		void _csg_init();
		void _csg_write(CSerializeStream&) const;
		void _csg_read(CSerializeStream&);
	};


	struct SRMICall
	{
		int messageId; //for call return
		int rpcId;

		SRMICall();
		bool operator==( const SRMICall& ) const;
		bool operator!=( const SRMICall& ) const;
		bool operator<( const SRMICall& ) const;

		void _csg_init();
		void _csg_write(CSerializeStream&) const;
		void _csg_read(CSerializeStream&);

	};
	
	struct SRMIReturn
	{
		int messageId;
		ERMIDispatchResult dispatchStatus;

		SRMIReturn();
		bool operator==( const SRMIReturn& ) const;
		bool operator!=( const SRMIReturn& ) const;
		bool operator<( const SRMIReturn& ) const;

		void _csg_init();
		void _csg_write(CSerializeStream&) const;
		void _csg_read(CSerializeStream&);
	};

}
#endif