#include "TestSerialize.h"
#include "../../Engine/engine/thread/thread.h"
#include "framework/DesignPattern/objectPool.h"
#include "engine/net/session/session.h"
#include "engine/net/socket/socket.h"
#include "engine/mq/msgQueue.h"
#include "engine/net/session/sessionManager.h"
#include "engine/net/socket/socketManager.h"
#include "engine/rpc/rmidef.h"
#include "../Message/Test.h"
#ifndef _TEST_TEST_SERIALIZE_EX_H_
#define _TEST_TEST_SERIALIZE_EX_H_

using namespace csg;
void testSend()
{
	//std::string host = "123.207.87.135";
	std::string host = "127.0.0.1";

	int socketfd = CSocketHelper::getClientSocket(AF_INET ,SOCK_STREAM ,0 ,DefaultPort ,host);
	if ( -1 == socketfd )
	{
		CSG_LOG_DEBUG("testSend get socket fail");
		return;
	}

	char ch;
	while ( true )
	{
		ch = cin.get();
		if ( 'b' == ch )
		{
			break;
		}

// 		SeriaInt msg;
// 		msg.aInt = rand() % 1000;
// 		msg.print();
// 
// 		CSerializeStream srcOS;
// 		srcOS.setUseBitMark(true);
// 		msg.write(srcOS);
// 		srcOS.prepareToAppend();
// 
// 		CSerializeStream tmpOS;
// 		tmpOS.writeSize(srcOS.getFlagDataSize());
// 		tmpOS.append(srcOS.getFlagData() ,srcOS.getFlagDataSize());
// 		tmpOS.append(srcOS.getData() ,srcOS.getDataSize());
// 
// 		int sendSize = CSocketHelper::sendMsg(socketfd ,tmpOS.getData() ,tmpOS.getDataSize());
// 		CSG_LOG_DEBUG("send size="<<ToStr(sendSize));
	}

}

void testRecv()
{
	CSocketManager::instance()->startListen(AF_INET ,SOCK_STREAM ,0 ,DefaultPort);
	CSG_LOG_DEBUG("listen start");
	cin.get();
	cin.get();
}

void testSendAll()
{
	std::string host = "123.207.87.135";
	//std::string host = "127.0.0.1";

	int socketfd = CSocketHelper::getClientSocket(AF_INET ,SOCK_STREAM ,0 ,DefaultPort ,host);
	if ( -1 == socketfd )
	{
		CSG_LOG_DEBUG("testSend get socket fail");
		return;
	}

	char ch;
	while ( true )
	{
		//ch = cin.get();
// 		if ( 'b' == ch )
// 		{
// 			break;
// 		}

// 		SeriaTestAllTest msg;
// 		msg.randValue();
// 		msg.print();
// 
// 		CSerializeStream srcOS;
// 		srcOS.setUseBitMark(true);
// 		msg.write(srcOS);
// 		srcOS.prepareToAppend();
// 
// 		CSerializeStream tmpOS;
// 		tmpOS.writeSize(srcOS.getFlagDataSize());
// 		tmpOS.append(srcOS.getFlagData() ,srcOS.getFlagDataSize());
// 		tmpOS.append(srcOS.getData() ,srcOS.getDataSize());
// 
// 		SProtocolHead head;
// 		CSerializeStream addHeadOs;
// 		head.msgSize = tmpOS.getDataSize();
// 		addHeadOs.append(&head ,SIZE_OF_PROTOCOL_HEAD);
// 		addHeadOs.append(tmpOS.getData() ,tmpOS.getDataSize());
// 
// 		int sendSize = CSocketHelper::sendMsg(socketfd ,addHeadOs.getData() ,addHeadOs.getDataSize());
// 
// 		CSG_LOG_DEBUG("send size=" << ToStr(sendSize)<<",head.size="<<ToStr(head.msgSize));

		CThread::sleep_for(1000);
	}

	cin.get();
}



void testSendAllAutoStream()
{
	std::string host = "123.207.87.135";
	//std::string host = "127.0.0.1";

	int socketfd = CSocketHelper::getClientSocket(AF_INET ,SOCK_STREAM ,0 ,DefaultPort ,host);
	if ( -1 == socketfd )
	{
		CSG_LOG_DEBUG("testSend get socket fail");
		return;
	}

	char ch;
	while ( true )
	{
		//ch = cin.get();
		// 		if ( 'b' == ch )
		// 		{
		// 			break;
		// 		}

// 		SeriaTestAllTest msg;
// 		msg.randValue();
// 		msg.print();
// 
// 		CAutoSerializeStream srcOS(CSerializeStreamPool::instance()->newObject());
// 		srcOS->setUseBitMark(true);
// 		msg.write(*srcOS);
// 		srcOS->prepareToAppend();
// 
// 		CAutoSerializeStream  tmpOS(CSerializeStreamPool::instance()->newObject());
// 		tmpOS->writeSize(srcOS->getFlagDataSize());
// 		tmpOS->append(srcOS->getFlagData() ,srcOS->getFlagDataSize());
// 		tmpOS->append(srcOS->getData() ,srcOS->getDataSize());
// 
// 		SProtocolHead head;
// 		CAutoSerializeStream addHeadOs(CSerializeStreamPool::instance()->newObject());
// 		head.msgSize = tmpOS->getDataSize();
// 		addHeadOs->append(&head ,SIZE_OF_PROTOCOL_HEAD);
// 		addHeadOs->append(tmpOS->getData() ,tmpOS->getDataSize());
// 
// 		int totalSize = addHeadOs->getDataSize();
// 		int haveSend = 0;
// 		while ( totalSize > 0 )
// 		{
// 			int realSend = 0;
// 			if ( totalSize > 100 )
// 				realSend = 100;
// 			else
// 				realSend = totalSize;
// 			int sendSize = CSocketHelper::sendMsg(socketfd ,addHeadOs->getData() + haveSend ,realSend);
// 			haveSend += sendSize;
// 			totalSize -= sendSize;
// 			CSG_LOG_DEBUG("send size=" << ToStr(sendSize) << ",head.size=" << ToStr(head.msgSize));
// 			CThread::sleep_for(100);
// 		}

		CThread::sleep_for(1000);
	}

	cin.get();
}



void testSendAllAutoStreamEx()
{
	//std::string host = "123.207.87.135";
	std::string host = "127.0.0.1";

	int socketfd = CSocketHelper::getClientSocket(AF_INET ,SOCK_STREAM ,0 ,DefaultPort ,host);
	if ( -1 == socketfd )
	{
		CSG_LOG_DEBUG("testSend get socket fail");
		return;
	}

	int times = 0;
	CAutoSerializeStream addHeadOs(CSerializeStreamPool::instance()->newObject());
	while ( true )
	{
		//ch = cin.get();
		// 		if ( 'b' == ch )
		// 		{
		// 			break;
		// 		}
// 		SeriaTestAllTest msg;
// 		msg.randValue();
// 		//msg.print();
// 
// 		CAutoSerializeStream srcOS(CSerializeStreamPool::instance()->newObject());
// 		srcOS->setUseBitMark(true);
// 		msg.write(*srcOS);
// 		srcOS->prepareToAppend();
// 
// 		CAutoSerializeStream  tmpOS(CSerializeStreamPool::instance()->newObject());
// 		tmpOS->writeSize(srcOS->getFlagDataSize());
// 		tmpOS->append(srcOS->getFlagData() ,srcOS->getFlagDataSize());
// 		tmpOS->append(srcOS->getData() ,srcOS->getDataSize());
// 
// 		SProtocolHead head;
// 		head.msgSize = tmpOS->getDataSize();
// 
// 		addHeadOs->append(&head ,SIZE_OF_PROTOCOL_HEAD);
// 		addHeadOs->append(tmpOS->getData() ,tmpOS->getDataSize());
// 		int sendSize = 0;
// 		if ( times == 1 )
// 		{
// 			sendSize = CSocketHelper::sendMsg(socketfd ,addHeadOs->getData() ,addHeadOs->getDataSize());
// 			CSG_LOG_DEBUG("send size=" << ToStr(sendSize) << ",head.size=" << ToStr(head.msgSize));
// 		}
// 		times++;
// 
// 		if ( times == 2 )
// 		{
// 			addHeadOs->reset();
// 			times = 0;
// 		}

		CThread::sleep_for(100);
	}

	cin.get();
}


void testSendAllAutoStreamExWithType()
{
	//std::string host = "123.207.87.135";
	std::string host = "127.0.0.1";

	int socketfd = CSocketHelper::getClientSocket(AF_INET ,SOCK_STREAM ,0 ,DefaultPort ,host);
	if ( -1 == socketfd )
	{
		CSG_LOG_DEBUG("testSend get socket fail");
		return;
	}

	int times = 0;
	CAutoSerializeStream addHeadOs(CSerializeStreamPool::instance()->newObject());
	while ( true )
	{
		char ch;
		CSG_LOG_DEBUG("enter o for 1");
		ch= cin.get();
		cin.get();
		
// 		SeriaTestAllTestExPtr msg = new SeriaTestAllTestEx();
// 		msg->randValue();
// 
// 		SeriaIntExPtr ai = new SeriaIntEx();
// 		ai->aInt = 9998;
// 
// 		CAutoSerializeStream srcOS(CSerializeStreamPool::instance()->newObject());
// 		srcOS->setUseBitMark(true);
// 
// 		int structType = 0;
// 		if ( 'o' == ch )
// 		{
// 			msg->_csg_write(*srcOS);
// 			structType = msg->getType();
// 			msg->print();
// 		} else
// 		{
// 			ai->_csg_write(*srcOS);
// 			structType = ai->getType();
// 			ai->print();
// 		}
// 
// 		srcOS->prepareToAppend();
// 		CAutoSerializeStream  tmpOS(CSerializeStreamPool::instance()->newObject());
// 	//写入type
// 		int mqType = EMessageTypeMQ;
// 		tmpOS->write(mqType);
// 		tmpOS->write(structType);
// 
// 
// 		tmpOS->writeSize(srcOS->getFlagDataSize());
// 		tmpOS->append(srcOS->getFlagData() ,srcOS->getFlagDataSize());
// 		tmpOS->append(srcOS->getData() ,srcOS->getDataSize());
// 
// 		SProtocolHead head;
// 		head.msgSize = tmpOS->getDataSize();
// 
// 		addHeadOs->append(&head ,SIZE_OF_PROTOCOL_HEAD);
// 		addHeadOs->append(tmpOS->getData() ,tmpOS->getDataSize());
// 		int sendSize = CSocketHelper::sendMsg(socketfd ,addHeadOs->getData() ,addHeadOs->getDataSize());
// 		CSG_LOG_DEBUG("send size=" << ToStr(sendSize) << ",head.size=" << ToStr(head.msgSize));
		
		CThread::sleep_for(100);
	}

	cin.get();
}


void testSendAllAutoStreamExWithTypeByPush()
{
	std::string host = "123.207.87.135";
	//std::string host = "127.0.0.1";

	int socketfd = CSocketHelper::getClientSocket(AF_INET ,SOCK_STREAM ,0 ,DefaultPort ,host);
	if ( -1 == socketfd )
	{
		CSG_LOG_DEBUG("testSend get socket fail");
		return;
	}

	int times = 0;
	CAutoSerializeStream addHeadOs(CSerializeStreamPool::instance()->newObject());
	while ( true )
	{
		char ch;
		CSG_LOG_DEBUG("enter o for 1");
		//ch = cin.get();
		cin.get();

// 		SeriaTestAllTestExPtr msg = new SeriaTestAllTestEx();
// 		msg->randValue();
// 
// 		SeriaIntExPtr ai = new SeriaIntEx();
// 		ai->aInt = 9998;
// 
// 
// 		CMsgBlockPtr mb = new CMsgBlock();
// 		mb->_msgHead.command = 988;
// 		mb->_msgHead.fromId.id = 1992;
// 		if ( times%2==0)
// 		{
// 			mb->_msgBase = msg;
// 		} else
// 		{
// 			mb->_msgBase = ai;
// 		}
// 
// 		mb->_msgBase->print();
// 
// 		CAutoSerializeStream  tmpOS(CSerializeStreamPool::instance()->newObject());
// 		//写入type
// 		int mqType = EMessageTypeMQ;
// 		tmpOS->write(mqType);		
// 		mb->_csg_write(*tmpOS);// _writeBody的时候写structType
// 
// 		//整合发送前的数据
// 		tmpOS->prepareToAppend();
// 
// 		CAutoSerializeStream  sendOs(CSerializeStreamPool::instance()->newObject());
// 		sendOs->writeSize(tmpOS->getFlagDataSize());
// 		sendOs->append(tmpOS->getFlagData() ,tmpOS->getFlagDataSize());
// 		sendOs->append(tmpOS->getData() ,tmpOS->getDataSize());
// 
// 
// 		//加协议头
// 		SProtocolHead head;
// 		head.msgSize = sendOs->getDataSize();
// 		addHeadOs->append(&head ,SIZE_OF_PROTOCOL_HEAD);
// 		addHeadOs->append(sendOs->getData() ,sendOs->getDataSize());
// 
// 		int sendSize = CSocketHelper::sendMsg(socketfd ,addHeadOs->getData() ,addHeadOs->getDataSize());
// 		CSG_LOG_DEBUG("send size=" << ToStr(sendSize) << ",head.size=" << ToStr(head.msgSize));
// 
// 		CThread::sleep_for(100);

		times++;
	}

	cin.get();
}


void testSendAllAutoStreamExWithTypeBySession()
{
	//std::string host = "123.207.87.135";
	std::string host = "127.0.0.1";

	int socketfd = CSocketHelper::getClientSocket(AF_INET ,SOCK_STREAM ,0 ,DefaultPort ,host);
	if ( -1 == socketfd )
	{
		CSG_LOG_DEBUG("testSend get socket fail");
		return;
	}

	CSessionPtr session = new CSession(socketfd ,host ,DefaultPort ,false);
	CProtocolPtr protocol = new CProtocol();
	session->setProtocol(protocol);
	session->setSocketfd(socketfd);

	int times = 0;
	while ( true )
	{
		char ch;
		CSG_LOG_DEBUG("enter o for 1");
		//ch = cin.get();
		cin.get();
// 
// 		SeriaTestAllTestExPtr msg = new SeriaTestAllTestEx();
// 		msg->randValue();
// 
// 		SeriaIntExPtr ai = new SeriaIntEx();
// 		ai->aInt = 9998;
// 
// 
// 		CMsgBlockPtr mb = new CMsgBlock();
// 		mb->_msgHead.command = 988;
// 		mb->_msgHead.fromId.id = 1992;
// 		if ( times % 2 == 0 )
// 		{
// 			mb->_msgBase = msg;
// 		} else
// 		{
// 			mb->_msgBase = ai;
// 		}
// 
// 		mb->_msgBase->print();
// 
// 
// 
// 		CAutoSerializeStream  tmpOS(CSerializeStreamPool::instance()->newObject());
// 		//写入type
// 		int mqType = EMessageTypeMQ;
// 		tmpOS->write(mqType);
// 		mb->_csg_write(*tmpOS);// _writeBody的时候写structType
// 
// 		//整合发送前的数据
// 		tmpOS->prepareToAppend();
// 
// 		CAutoSerializeStream  sendOs(CSerializeStreamPool::instance()->newObject());
// 		sendOs->writeSize(tmpOS->getFlagDataSize());
// 		sendOs->append(tmpOS->getFlagData() ,tmpOS->getFlagDataSize());
// 		sendOs->append(tmpOS->getData() ,tmpOS->getDataSize());
// 
// 
// 		//加协议头
// 		SProtocolHead head;
// 		head.msgSize = sendOs->getDataSize();
// 		CAutoSerializeStream addHeadOs(CSerializeStreamPool::instance()->newObject());
// 		addHeadOs->append(&head ,SIZE_OF_PROTOCOL_HEAD);
// 		addHeadOs->append(sendOs->getData() ,sendOs->getDataSize());
// 
// 		int sendSize=CSocketHelper::sendMsg(session->getSocketfd(),addHeadOs->getData() ,addHeadOs->getDataSize());
// 
// 		CSG_LOG_DEBUG("send size=" << ToStr(sendSize) << ",head.size=" << ToStr(head.msgSize));

		CThread::sleep_for(100);

		times++;
	}

	cin.get();
}


void testSendByTask()
{
	std::string host = "123.207.87.135";
	//std::string host = "127.0.0.1";

	int socketfd = CSocketHelper::getClientSocket(AF_INET ,SOCK_STREAM ,0 ,DefaultPort ,host);
	if ( -1 == socketfd )
	{
		CSG_LOG_DEBUG("testSend get socket fail");
		return;
	}

	CSessionPtr session = new CSession(socketfd ,host ,DefaultPort ,false);
	CProtocolPtr protocol = new CProtocol();
	session->setProtocol(protocol);
	session->setSocketfd(socketfd);

	CSessionManager::instance()->addSession(session);

	int times = 0;
	while ( true )
	{
		char ch;
		CSG_LOG_DEBUG("enter o for 1");
		//ch = cin.get();
		cin.get();

// 		SeriaTestAllTestExPtr msg = new SeriaTestAllTestEx();
// 		msg->randValue();
// 
// 		SeriaIntExPtr ai = new SeriaIntEx();
// 		ai->aInt = 9998;


		CMsgBlockPtr mb = new CMsgBlock();
		mb->_msgHead.command = 988;
		mb->_msgHead.fromId.id = 1992;
		if ( times % 2 == 0 )
		{
			//mb->_msgBase = msg;
		} else
		{
			//mb->_msgBase = ai;
		}

		mb->_msgBase->print();

		CMsgQueue::instance()->pushMessage(session->getSocketfd() ,mb);

		CThread::sleep_for(100);

		times++;
	}

	cin.get();
}

void testClientRecv()
{
	//std::string host = "123.207.87.135";
	std::string host = "127.0.0.1";

	
	int socketfd = CSocketManager::instance()->startConnect(AF_INET ,SOCK_STREAM ,0 ,DefaultPort ,host);
	if (-1==socketfd )
	{
		CSG_LOG_DEBUG("testSend get socket fail");
		return;
	}

	CSessionPtr session = new CSession(socketfd ,host ,DefaultPort ,false);
	CProtocolPtr protocol = new CProtocol();
	session->setProtocol(protocol);
	session->setSocketfd(socketfd);

	CSessionManager::instance()->addSession(session);

	int times = 0;
	while ( true )
	{
		char ch;
		CSG_LOG_DEBUG("enter o for 1");
		//ch = cin.get();
		cin.get();


		CMsgBlockPtr mb = new CMsgBlock();
		mb->_msgHead.command = 988;
		mb->_msgHead.fromId.id = 1992;
		
// 		tei_Ptr test = new tei();
// 		test->a = 97987;
// 		test->bt = true;
// 		test->l = 76876886;
// 		test->str = "hi,csgl";
// 		test->ss = "royalchen";
// 		test->vs.push_back("gwe");
// 		test->vs.push_back("gwgwe");
// 		test->vs.push_back("gwe154");
// 		
// 		mb->_msgBase = test;
		

		CMsgQueue::instance()->pushMessage(session->getSocketfd() ,mb);

		CThread::sleep_for(100);

		times++;
	}

	cin.get();
}

void testCsgl()
{
	std::string host = "127.0.0.1";


	int socketfd = CSocketManager::instance()->startConnect(AF_INET ,SOCK_STREAM ,0 ,DefaultPort ,host);
	if ( -1 == socketfd )
	{
		CSG_LOG_DEBUG("testSend get socket fail");
		return;
	}

	CSessionPtr session = new CSession(socketfd ,host ,DefaultPort ,false);
	CProtocolPtr protocol = new CProtocol();
	session->setProtocol(protocol);
	session->setSocketfd(socketfd);

	CSessionManager::instance()->addSession(session);

	int times = 0;
	while ( true )
	{
		char ch;
		CSG_LOG_DEBUG("enter o for 1");
		ch = cin.get();
		cin.get();


		CMsgBlockPtr mb = new CMsgBlock();
		mb->_msgHead.command = 988;
		mb->_msgHead.fromId.id = 1992;

		Message::STest_Ptr test = new Message::STest();
		test->a = 1900;
		test->b = 2312;

		mb->_msgBase = test;

		CMsgQueue::instance()->pushMessage(session->getSocketfd() ,mb);

		CThread::sleep_for(100);

		times++;
	}
}


void testSerializeEx()
{
// 	CGateMsg msg;
// 	Cstruct::regist();
	Message::CTest::regist();
	char ch;
	do
	{
		CSG_LOG_DEBUG("enter s fro server,c fro client,other for nothing");
		ch = cin.get();
		if ( 's' == ch )
		{
			testRecv();
			break;
		} else if ( 'c' == ch )
		{
			testCsgl();
			//testClientRecv();
			//testSendByTask();
			//testSendAllAutoStreamExWithTypeBySession();
			//testSendAllAutoStreamExWithTypeByPush();
			//testSendAllAutoStreamExWithType();
			//testSendAllAutoStreamEx();
			//testSendAllAutoStream();
			//testSendAll();
			//testSend();
			break;
		}
	} while ( true );
}

#endif