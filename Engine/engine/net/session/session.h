#ifndef _ENGINE_NET_SESSION_SESSION_H_
#define _ENGINE_NET_SESSION_SESSION_H_

#include "framework/SmartPoint/refObject.h"
#include "framework/SmartPoint/smartPoint.h"
#include "../protocol/protocol.h"
#include "../../mq/msgBlock.h"

namespace csg
{
	class CSession :public virtual CRefObject
	{
	public:
		CSession(int socketfd ,std::string& addr ,int port,bool isInner);

		int getSocketfd()const;

		void print(std::string info);

		virtual ~CSession();

		void setProtocol(CProtocolPtr& protocol);

		bool onRecvData(const void* buf ,int len);

	public:
		void handlePacket();

		bool pushMessage(const CMsgBlockPtr& mb);

	private:
		CLock& getNetRecvIoLock();

		CLock& getNetSendIoLock();

		void setGetMsgFlag(bool b);

		bool isGetMsg();

	private:
#ifdef _DEBUG
	public:
#endif
		int _socketfd;
		std::string _remoteAddr;
		int _remotePort;
		bool _isClient;
		CProtocolPtr _protocol;
		CLock _netRecvIoLock;
		CLock _netSendIoLock;
		bool _isGetMsg; //标记是否接收到新的留
	};
	typedef CSmartPointShare<CSession> CSessionPtr;
}
#endif