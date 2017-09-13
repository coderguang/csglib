#ifndef _ENGINE_NET_PROTOCOL_PROTOCOL_H_
#define _ENGINE_NET_PROTOCOL_PROTOCOL_H_

#include "../../lock/lock.h"
#include "../../serialize/serializestream.h"
#include "protocoldef.h"
#include "framework/SmartPoint/smartPoint.h"
#include "framework/SmartPoint/refObject.h"
#include "framework/DesignPattern/objectPool.h"
#include "../../mq/msgBlock.h"

namespace csg
{
	//每个Session都有一个protocol
	class CProtocol:public virtual CRefObject
	{
	public:
		CProtocol();

		virtual ~CProtocol();

		virtual int handleRecvData(const void* inData ,const int len);

		virtual int handleSendData(int socketfd,const void* data ,const int len);

		virtual bool handlePacket();

		virtual int pushMessage(int socketfd ,const CMsgBlockPtr& mb);

	protected:
		virtual bool handlePacket(const void *inData ,const int len);

	protected:
		CRecursiveLock& getLock();

	private:
		CRecursiveLock _lock;
		CAutoSerializeStream _buffer; //网络IO中接收的数据存放在这里
		SProtocolHead _protocolHead;
		bool _receiveHead;
	};

	typedef CSmartPointShare<CProtocol> CProtocolPtr;
}

#endif