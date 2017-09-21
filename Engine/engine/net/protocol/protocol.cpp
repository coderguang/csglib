
#include "engine/net/protocol/protocol.h"
#include "framework/log/Log.h"
#include "framework/util/endian.h"
#include "framework/exception/exception.h"
#include <string.h>
#include "framework/DesignPattern/objectPool.h"
#include "../socket/socket.h"
#include "../../rpc/rmidef.h"
#include "../../rpc/rmiObjectAdapter.h"
#include "../../rpc/rmiObject.h"

using namespace csg;

csg::CProtocol::CProtocol() :_buffer(CSerializeStreamPool::instance()->newObject())
{
	_receiveHead = false;
}

csg::CProtocol::~CProtocol()
{

}

int csg::CProtocol::handleRecvData(const void* inData ,const int len)
{
	CAutoRecursiveLock l(getLock());
	//CSG_LOG_INFO("CProtocol::handleRecvData,len="<<len);
	_buffer->append(inData ,len);

	return len;
}

int csg::CProtocol::handleSendData(const CSessionPtr& session ,const void* data ,const int len)
{
	return CSocketHelper::sendMsg(session->getSocketfd() ,(const char*)data ,len);
	
}

bool csg::CProtocol::handlePacket(const CSessionPtr& session,const void *inData ,const int len)
{
	CAutoRecursiveLock l(getLock());


	CAutoSerializeStream is(CSerializeStreamPool::instance()->newObject());
	is->append(inData ,len);

	is->setUseBitMark(false);

	is->prepareToRead();

	ERMIMessageType mqType;
	_csg_read(*is ,mqType);

	switch ( mqType )
	{
		case csg::ERMIMessageTypeMQ:
		{
			CMsgBlockPtr msg = new CMsgBlock();
			msg->_csg_read(*is);

			if ( !msg )
			{
				assert(false);
				return false;
			}
			msg->_msgBase->print();
		}
		break;
		case csg::ERMIMessageTypeCall:
		{
			SRMICall rmiCall;
			rmiCall._csg_read(*is);

			MapRMIObject mapObject;
			if (!CRMIObjectAdapter::instance()->findRmiObject("Test" ,mapObject) )
			{
				return false;
			}
			MapRMIObject::iterator it = mapObject.find(rmiCall.rpcId);
			if ( it == mapObject.end() )
			{
				return false;
			}
			is->setUseBitMark(true);
			it->second->__onCall(session,rmiCall ,*is);			
		}
		break;
		case csg::ERMIMessageTypeCallRet:
		{
			SRMIReturn rmiReturn;
			rmiReturn._csg_read(*is);

			is->setUseBitMark(true);

			CSG_LOG_INFO("CProtocol::handlePacket"<<",messageId="<<rmiReturn.messageId);

			if ( rmiReturn.messageId <= 0 )
				return true;

			CRMIObjectBindPtr backObject;
			if ( !session->getCallBackObject(rmiReturn.messageId ,backObject) )
			{
				return false;
			}
			if ( backObject->_callBack )
			{
				backObject->_callBack->__response(*is);
			}

		}
		break;
		default:
		break;
	}

	return true;

	try
	{
#ifdef _DEBUG
		int times = 0;
#endif
		while ( is->getBytesLeft() > 0 )
		{
#ifdef _DEBUG
			assert(times < 1);
#endif
			uint_t size;
			is->readSize(size);
			if ( size < 0 )
			{
				return false;
			}
			if ( is->getBytesLeft() < size )
			{
				return false;
			}
// 			if ( !handlePacketEx(is->getReadPoint() ,size) )
// 			{
// 				return false;
// 			}
			is->addReadIndex(size);
#ifdef _DEBUG
			times++;
#endif
		}

	} catch ( const CException& ex )
	{
		CSG_LOG_INFO("CProtocol::handlePacket code="<<ex.code()<<",what="<<ex.what());
	} catch ( ... )
	{
		CSG_LOG_INFO("CProtocol::handlePacket unkonw exception");
	}

	return true;
}

int csg::CProtocol::pushMessage(const CSessionPtr& session ,const CMsgBlockPtr& mb)
{
	CAutoSerializeStream  tmpOS(CSerializeStreamPool::instance()->newObject());

	_csg_write(*tmpOS ,ERMIMessageTypeMQ);
	mb->_csg_write(*tmpOS);// _writeBody的时候写structType

	//整合发送前的flag数据
	tmpOS->prepareToAppend();

	CAutoSerializeStream  sendOs(CSerializeStreamPool::instance()->newObject());
	sendOs->writeSize(tmpOS->getFlagDataSize());
	sendOs->append(tmpOS->getFlagData() ,tmpOS->getFlagDataSize());
	sendOs->append(tmpOS->getData() ,tmpOS->getDataSize());

	//加协议头
	SProtocolHead head;
	head.msgSize = sendOs->getDataSize();
	CAutoSerializeStream addHeadOs(CSerializeStreamPool::instance()->newObject());
	addHeadOs->append(&head ,SIZE_OF_PROTOCOL_HEAD);
	addHeadOs->append(sendOs->getData() ,sendOs->getDataSize());

	return handleSendData(session,addHeadOs->getData() ,addHeadOs->getDataSize());

}

bool csg::CProtocol::handlePacket(const CSessionPtr& session)
{
	do
	{
		if ( _receiveHead )
		{
			if ( _buffer->getDataSize() < _protocolHead.msgSize )
			{
				//data not enough for a packet
				return true;
			} else
			{
				//data enough for a packet
				if ( !handlePacket(session,_buffer->getData() + SIZE_OF_PROTOCOL_HEAD ,_protocolHead.msgSize) )
				{
					CSG_LOG_INFO("CProtocol::handlePacket error");
					return false;
				}
				_buffer->popData(_protocolHead.msgSize + SIZE_OF_PROTOCOL_HEAD);
				_receiveHead = false;
			}
		} else
		{
			if ( _buffer->getDataSize() >= SIZE_OF_PROTOCOL_HEAD )
			{
				memcpy(&_protocolHead ,_buffer->getData() ,SIZE_OF_PROTOCOL_HEAD);
				_protocolHead.msgSize = endian(_protocolHead.msgSize);
				_receiveHead = true;
			} else
			{
				return true;
			}
		}
	} while ( true );

	return false;
}

CRecursiveLock& csg::CProtocol::getLock()
{
	return _lock;
}

