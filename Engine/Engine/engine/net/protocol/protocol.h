#ifndef _ENGINE_NET_PROTOCOL_PROTOCOL_H_
#define _ENGINE_NET_PROTOCOL_PROTOCOL_H_

#include "../../lock/lock.h"
#include "../../serialize/serializestream.h"
#include "protocoldef.h"
#include "framework/SmartPoint/smartPoint.h"
#include "framework/SmartPoint/refObject.h"
#include "framework/DesignPattern/objectPool.h"
#include "../../mq/msgBlock.h"
#include "../session/session.h"

namespace csg
{

	class CSession;

	typedef CSmartPointShare<CSession> CSessionPtr;

	//每个Session都有一个protocol
	class CProtocol:public virtual CRefObject
	{
	public:
		CProtocol();

		virtual ~CProtocol();
		// IO 线程接收数据
		virtual int handleRecvData(const void* inData ,const int len);
		// IO 线程发送数据
		virtual int handleSendData(const CSessionPtr& session,const void* data ,const int len);
		//逻辑主线程处理数据
		virtual bool handlePacket(const CSessionPtr& session);

		virtual int pushMessage(const CSessionPtr& session ,const CMsgBlockPtr& mb);

	protected:
		virtual bool handlePacket(const CSessionPtr& session,const void *inData ,const int len);

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