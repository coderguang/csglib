
#include "engine/net/session/session.h"
#include "framework/log/Log.h"
#include "framework/util/typetransform.h"
#include <limits.h>

using namespace csg;
csg::CSession::CSession(int socketfd ,std::string& addr ,int port ,bool isInner) :_socketfd(socketfd) ,_remoteAddr(addr) ,_remotePort(port) ,_isClient(isInner) ,_callBackId(1)
{

}

void csg::CSession::setSocketfd(int fd)
{
	_socketfd = fd;
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

bool csg::CSession::addRMIObject(const MapRMIObject& objects)
{
	for ( MapRMIObject::const_iterator it = objects.begin(); it != objects.end(); it++ )
	{
		assert(_rmiObjectMap.find(it->first) == _rmiObjectMap.end());
		_rmiObjectMap[it->first] = it->second;
	}
	return true;
}

int csg::CSession::getCallBackId()
{
	if ( _callBackId < INT_MAX )
	{
		return _callBackId++;
	} else
	{
		_callBackId = 1;
		return _callBackId;
	}
}

void csg::CSession::addCallBackObject(int callBackId ,CRMIObjectBindPtr& object)
{
	MapRMIObjectBind::const_iterator it = _callBackMap.find(callBackId);
	assert(it == _callBackMap.cend());
	_callBackMap[callBackId] = object;
}

bool csg::CSession::getCallBackObject(int callBackId ,CRMIObjectBindPtr& backObject)
{
	MapRMIObjectBind::const_iterator it = _callBackMap.find(callBackId);
	if ( it != _callBackMap.cend() )
	{
		backObject = it->second;
		return true;
	}
	backObject = NULL;
	return false;
}

void csg::CSession::handlePacket()
{
	if ( isGetMsg() )
	{
		CAutoLock l(getNetRecvIoLock());
		assert(_protocol);
		_protocol->handlePacket(this);
		setGetMsgFlag(false);
	}
}

bool csg::CSession::pushMessage(const CMsgBlockPtr& mb)
{
	CAutoLock l(getNetSendIoLock());
	assert(_protocol);
	if ( _socketfd > 0 )
		return _protocol->pushMessage(this ,mb);
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

