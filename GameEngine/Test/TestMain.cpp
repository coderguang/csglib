#ifndef _TEST_TEST_MAIN_H_
#define _TEST_TEST_MAIN_H_

#include "framework/counter/counterHandler.h"
#include "engine/def/csg_def.h"
#include "engine/serialize/serializestream.h"
#include "TestSerialize.h"
#include "TestThread.h"


#include <iostream>
#include "TestTime.h"
#include "TestLog.h"
#include "TestSmartPtr.h"
#include "TestSocket.h"
#include "TestBaseWinSocket.h"
#include "framework/log/Log.h"
#include "TestUtil.h"
#include "TestChat.h"
#include "engine/net/socket/socketManager.h"
#include <exception>
#include "TestSerializeEx.h"
#include "TestRpc.h"
#include "TestTimer.h"
#include "framework/counter/counterHandlerManager.h"


void startMain()
{
	CSG_LOG_DEBUG("Program run in below environment !!!");
#ifdef _DEBUG
	CSG_LOG_DEBUG("_DEBUG set");
#endif

#ifdef CSG_WIN
	CSG_LOG_DEBUG("CSG_WIN set");
#endif

#ifdef CSG_LINUX
	CSG_LOG_DEBUG("CSG_LINUX set");
#endif
	/*
	long64_t t = 3124124235423;
	double d = 0.254;
	bool b = false;
	short s = 135;
	CSG_LOG_DEBUG("test log t=" << t << ",d=" << d<<" b="<<b<<",s="<<s);
	*/
	//启动时间更新线程
	CThreadManager::instance()->activeBaseServer(EBaseServerUpdateDt);
	//log线程
	std::string dir = "TestApp";
	CThreadManager::instance()->activeBaseServer(EBaseServerLoggerWritter ,dir);

	CThreadManager::instance()->activeBaseServer(EBaseServerMainLogic);

	//启动网络IO发送线程

	CThreadManager::instance()->activeBaseServer(EBaseServerSocketSend);

	//启动网络IOCP
	if ( -1 == CSocketManager::instance()->init() )
	{
		CSG_LOG_DEBUG("socket init fail");
		cin.get();
		return;
	}
	//监控对象数量
	CCounterHandlerManager::initPrintCounterTimer();


	//testTimer();

	//testRPC();

	//testSerializeEx();

	//testCounter(5);

	//testSerialize();

	//testTime();

	//testThread();

	//testLog();

#ifdef CSG_WIN
	//testSocket();

	//testBaseWinSocket();

	//testChat();

	//testSmartPtr();
#endif

#ifdef CSG_LINUX
	//testSmartPtr();
#endif

	//testUtil();
	//#ifdef CSG_WIN
	//testChat();
	//#endif

	//#ifdef CSG_LINUX
	//testLinuxChat();
	// #endif

	char ch;
	//std::cout << "enter b for exit!" << std::endl;
	CSG_LOG_DEBUG("enter anykey for exit!");
	do
	{
		ch = cin.get();
		// 		if ( 'b' == ch )
		// 			break;
		break;
		CThread::sleep_for(100);
	} while ( true );

	CThreadManager::instance()->killAllTask();

	//等待所有线程执行退出完成
	CThread::sleep_for(2000);
}


int main(int argc, char** argv){
	
	try
	{
		startMain();
	} catch ( exception &ex )
	{
		std::cerr << "exception ,code=" << ex.what() << std::endl;
	}

	return 0;
}

#endif