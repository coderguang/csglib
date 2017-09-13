
#include "engine/net/session/session.h"
#include "framework/log/Log.h"
#include "framework/util/typetransform.h"

using namespace csg;
csg::CSession::CSession(int socketfd ,std::string& addr ,int port ,bool isInner) :_socketfd(socketfd) ,_remoteAddr(addr) ,_remotePort(port) ,_isClient(isInner)
{

}

int csg::CSession::getSocketfd() const
{
	return _socketfd;
}

void csg::CSession::print(std::string info)
{
	CSG_LOG_DEBUG("CCSession::print:" << info << ",remoteaddr="<<_remoteAddr<<",port="<<ToStr(_remotePort));
}

csg::CSession::~CSession()
{

}

void csg::CSession::setProtocol(CProtocolPtr& protocol)
{
	_protocol = protocol;
}

bool csg::CSession::onRecvData(const void* buf ,int len)
{
	CAutoLock l(getNetRecvIoLock());
	assert(_protocol);
	if ( len > 0 )
	{
		setGetMsgFlag(true);
	}
	return _protocol->handleRecvData(buf ,len);
}

void csg::CSession::handlePacket()
{
	if ( isGetMsg() )
	{
		CAutoLock l(getNetRecvIoLock());
		assert(_protocol);
		_protocol->handlePacket();
		setGetMsgFlag(false);
	}
}

bool csg::CSession::pushMessage(const CMsgBlockPtr& mb)
{
	CAutoLock l(getNetSendIoLock());
	assert(_protocol);
	if ( _socketfd > 0 )
		return _protocol->pushMessage(_socketfd ,mb);
	else
		return false;
}

CLock& csg::CSession::getNetRecvIoLock()
{
	return _netRecvIoLock;
}

CLock& csg::CSession::getNetSendIoLock()
{
	return _netSendIoLock;
}

void csg::CSession::setGetMsgFlag(bool b)
{
	_isGetMsg = b;
}

bool csg::CSession::isGetMsg()
{
	return _isGetMsg;
}

