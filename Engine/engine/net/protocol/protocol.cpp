
#include "engine/net/protocol/protocol.h"
#include "framework/log/Log.h"
#include "framework/util/endian.h"
#include "framework/exception/exception.h"
#include "../GameEngine/Message/GameMsg.h"
#include <string.h>
#include "framework/DesignPattern/objectPool.h"
#include "../socket/socket.h"
#include "../../rpc/rmidef.h"

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

int csg::CProtocol::handleSendData(int socketfd,const void* data ,const int len)
{
	return CSocketHelper::sendMsg(socketfd ,(const char*)data ,len);
	
}

bool csg::CProtocol::handlePacket(const void *inData ,const int len)
{
	CAutoRecursiveLock l(getLock());


	CAutoSerializeStream is(CSerializeStreamPool::instance()->newObject());
	is->append(inData ,len);

	is->prepareToRead();

	int mqType = 0;
	is->read(mqType);
	if ( EMessageTypeMQ != mqType )
	{
		assert(false);
		return false;
	}

	CMsgBlockPtr msg = new CMsgBlock();
	msg->_csg_read(*is);

	if ( !msg )
	{
		assert(false);
		return false;
	}
	msg->_msgBase->print();

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

int csg::CProtocol::pushMessage(int socketfd ,const CMsgBlockPtr& mb)
{
	CAutoSerializeStream  tmpOS(CSerializeStreamPool::instance()->newObject());

	int mqType = EMessageTypeMQ;
	tmpOS->write(mqType);
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

	return handleSendData(socketfd,addHeadOs->getData() ,addHeadOs->getDataSize());

}

bool csg::CProtocol::handlePacket()
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
				if ( !handlePacket(_buffer->getData() + SIZE_OF_PROTOCOL_HEAD ,_protocolHead.msgSize) )
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

