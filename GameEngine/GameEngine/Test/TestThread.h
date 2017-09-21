
#ifndef _TEST_TEST_THREAD_H_
#define _TEST_TEST_THREAD_H_

#include <thread>
#include "framework/counter/counterHandler.h"
#include "framework/log/Log.h"
#include <thread>
#include "framework/io/PrintIOHandler.h"
#include "framework/datetime/datetime.h"
#include "engine/lock/lock.h"
#include "engine/lock/lockObjectManager.h"
#include <set>
#include "engine/thread/thread.h"
#include <mutex>
#include "engine/thread/task.h"
#include "engine/baseServer/updateDtTask.h"
#include "engine/thread/threadManager.h"

using namespace csg;

std::atomic<bool> ready;
void printObject(int v)
{
	static int counter = 0;
	for ( int i = 0; i < v; i++ )
	{
		counter++;
		std::cout << ",count=" << counter << std::endl;
	}
	/*
	static CCounterHandler t("test");
	CPrintIOHandler* log = new CPrintIOHandler();
	t.print(log);
	*/
}

void testCounter(int num)
{
	CCounterHandler t("test");
	for ( int i = 0; i <= num; i++ )
	{
		std::cout << ",thread=" << i << std::endl;
		std::thread t2(printObject,100);
		t2.detach();
	}
}

void updateSysDt(int loop)
{
	int i = 0;
	while ( true )
	{
		if ( loop == i++ )
		{
			i = 0;
			CDateTime::updateThreadDt();
		}
	}
}

int updateSysDtByThread(void *args)
{
	CDateTime now;
	std::cout << "updateDt thread_id=" << CThread::threadId()<<"now is "<<now.asString()<<std::endl;

	int *uDt = (int*) args;
	while ( true )
	{
		if ( CDateTime().getTotalSecond() - now.getTotalSecond() > 10 )
		{
			if ( !ready )
			{
				ready = true;
			}
		}
		CThread::sleep_for(*uDt);
		CDateTime::updateThreadDt();
		//std::cout << "updateDt thread_id=" << std::this_thread::threadId();
	}
	return 0;
}

void testUpdateDt()
{
	std::thread dtTask(updateSysDt ,1000);

	char ch;
	do
	{
		std::cout << "n for continue,b for break:" << std::endl;
		ch = cin.get();
		if ( ch == 'b' )
		{
			break;
		}
		CDateTime dt;
		std::cout << "now my data is " << dt.asString() << std::endl;
		using std::chrono::system_clock;
		system_clock::time_point today = system_clock::now();
		time_t tt = system_clock::to_time_t(today);
		std::cout << "system date is: " << ctime(&tt);
	} while ( true );
}

int printHello(void *arg)
{
	int *pDt = (int*) arg;
	do
	{
		CDateTime now;
		CThread::sleep_for(*pDt);
		std::cout << "hello,now is "<<now.asString() << std::endl;
		
	} while ( true );
}

int printHelloEx(void *arg)
{
	int *pDt = (int*) arg;
	while ( !ready )
	{
		CThread::yeild();
	}
	CDateTime now;
	do
	{
		CThread::sleep_until(now);
		std::cout << "hello,now is " << CDateTime().asString() << std::endl;
		now += CInterval(*pDt ,0);
	} while ( true );
}


static int threadAddSum = 0;
static std::set<int> numset;

void addSum(int flag)
{
	while ( true )
	{
		//CAutoLock l(CLockObjectManager::getSingletonLock());
		threadAddSum++;
		//std::cout <<"flag:"<<flag<< "threadSum=" << threadAddSum << std::endl;
		std::cout<< "threadSum=" << threadAddSum << std::endl;
		std::cout.flush();
		if ( numset.count(threadAddSum) > 1 )
			assert(false);
		numset.insert(threadAddSum);
// 		if ( threadAddSum>10000 ) {
// 			break;
// 		}
	}
}

void testNormalThread()
{
	std::thread dtTask(updateSysDt ,1000);

	std::cout << "main start" << std::endl;
	std::thread t(addSum ,111);
	std::thread t2(addSum ,222);
	std::thread t3(addSum ,333);
	//std::thread t3(addSum);
	//t.detach();
	dtTask.join();
	t.join();
	t2.detach();
	t3.detach();
	std::cout << "main end" << std::endl;
}

void printTime()
{
	char ch;
	do
	{
		std::cout << "n for continue,b for break:" << std::endl;
		ch = cin.get();
		if ( ch == 'b' )
		{
			break;
		}
		CDateTime dt;
		std::cout << "now my data is " << dt.asString() << std::endl;
		using std::chrono::system_clock;
		system_clock::time_point today = system_clock::now();
		time_t tt = system_clock::to_time_t(today);
		std::cout << "system date is: " << ctime(&tt);
	} while ( true );
}

void testMyThread()
{
	/*
	int uDt= 500;
	int pDt = 2;
	ready = false;
	std::cout << "pid is " << CThread::pid() << std::endl;
	csg_thread thr;
		CThread::InitTask(updateSysDtByThread ,(void*)&uDt,thr);
		csg_thread thrEx;
		CThread::InitTask(printHelloEx ,(void*) &pDt,thrEx);
	std::cout << "main run ,thread="<<std::this_thread::threadId()<<std::endl;
	thr.detach();
	thrEx.join();
	printTime();
	*/
}

std::mutex lockEE;

int counterLock = 0;

int printLockHello(void *dt)
{
	std::cout << "thread_id= " << CThread::threadId() << " start " << std::endl;
	//CAutoLock lock(CLockObjectManager::getSingletonLock());
	//std::lock_guard<std::mutex> l(lockEE);
	do
	{
		{
			CThread::sleep_for(1000);
			CAutoLock lock(CLockObjectManager::getSingletonLock());
			counterLock++;
			CDateTime dt;
			std::cout << "thread_id=" << CThread::threadId() << ",now my data is " << dt.asString() << ",num=" << counterLock << std::endl;
		}
	} while ( true );
}

int printLockHelloByMain()
{
	char ch;
	do
	{

		ch = cin.get();
		if ( 'n' == ch )
		{
			CDateTime dt;
			std::cout << "thread_id=" << CThread::threadId() << ",now my data is " << dt.asString() << ",num=" << counterLock << std::endl;
		} else if ( 'b' == ch )
		{
			break;
		}
	} while ( true );

	return 0;
}


void testLock()
{
	int uDt = 500;
	std::cout << "pid is " << CThread::pid() << std::endl;
// 	csg_thread thr = CThread::InitTask(updateSysDtByThread ,(void*) &uDt);
// 	csg_thread thrEx= CThread::InitTask(printLockHello ,(void*) &uDt);
// 	csg_thread thrEx2 = CThread::InitTask(printLockHello ,(void*) &uDt);
// 	csg_thread thrEx3 = CThread::InitTask(printLockHello ,(void*) &uDt);
// 	csg_thread thrEx4 = CThread::InitTask(printLockHello ,(void*) &uDt);
// 	csg_thread thrEx5 = CThread::InitTask(printLockHello ,(void*) &uDt);
// 	csg_thread thrEx6 = CThread::InitTask(printLockHello ,(void*) &uDt);
// 
// 	thr.join();
}

void testTask()
{
	int uDt = 500;
// 	CTask task(updateSysDtByThread ,(void*) &uDt);
// 	{
// 		CTask taskEx(printLockHello ,(void*) &uDt);
// 		task.active();
// 	}
	//taskEx.active(5);
	//task.join();
}

void testTaskEx()
{
	int threadCount = 3;
	int uDt = 500;
	CSG_THREAD_FUN _func = printLockHello;
	void *_args = (void*) &uDt;
	
	std::cout << "pid is " << CThread::pid() << std::endl;
	return;
}

void testTask2()
{
	int uDt = 500;
	//CTask task(updateSysDtByThread ,(void*) &uDt);
	//task.active(1);



}

void testUpdateDtTask()
{
	CUpdateDtTask dtTask;
	//dtTask.active(1);

	char ch;
	do
	{

		ch = cin.get();
		if ( 'b' == ch )
		{
			break;
		} else if ( 's' == ch )
		{
			//dtTask.setExit();
		}else
		{
			CDateTime dt;
			std::cout << "thread_id=" << CThread::threadId() << ",now my data is " << dt.asString() << ",num=" << counterLock << std::endl;
		} 
	} while ( true );
}

void testNotify()
{
	CUpdateDtTask dtTask;
	//dtTask.active(1);

	char ch;
	do
	{

		ch = cin.get();
		if ( 'b' == ch )
		{
			break;
		} else if ( 's' == ch )
		{
			//dtTask.setStop();
		} else if ( 't' == ch )
		{
			//dtTask.runFromStop();
		} else if ( 'e' == ch )
		{
			//dtTask.setExit();
		}
		else
		{
			CDateTime dt;
			std::cout << "thread_id=" << CThread::threadId() << ",now my data is " << dt.asString() << ",num=" << counterLock << std::endl;
		}
	} while ( true );

}


void testTaskMgr()
{
	CUpdateDtTask dtTask;
	//dtTask.active(3);

	//CThreadManager::instance()->addTask(dtTask);

	std::string taskName = "CUpdateDtTask";

	char ch;
	do
	{
		ch = cin.get();
		if ( 'b' == ch )
		{
			break;
		} else if ( 's' == ch )
		{
			CThreadManager::instance()->stopTaskByType(EBaseServerUpdateDt);

		} else if ( 't' == ch )
		{
			CThreadManager::instance()->runTaskFromStopByType(EBaseServerUpdateDt);
		} else if ( 'e' == ch )
		{
			CThreadManager::instance()->killTaskByType(EBaseServerUpdateDt);
		} else
		{
			CDateTime dt;
			std::cout << "thread_id=" << CThread::threadId() << ",now my data is " << dt.asString() << ",num=" << counterLock << std::endl;
		}
	} while ( true );

}

void testTaskMgrEx()
{
	//CUpdateDtTask dtTask;
	//dtTask.active(1);

	//CThreadManager::instance()->addTask(dtTask);

	CThreadManager::instance()->activeBaseServer(EBaseServerUpdateDt);

	std::string taskName = "CUpdateDtTask";

	char ch;
	do
	{
		ch = cin.get();
		if ( 'b' == ch )
		{
			break;
		} else if ( 's' == ch )
		{
			CThreadManager::instance()->stopAllTask();

		} else if ( 't' == ch )
		{
			CThreadManager::instance()->runAllTaskFromStop();
		} else if ( 'e' == ch )
		{
			CThreadManager::instance()->killAllTask();
		} else
		{
			CDateTime dt;
			std::string printLog = " now my data is "+dt.asString();;
			CLogManager::instance()->add(printLog);
		}
	} while ( true );

	CThreadManager::instance()->killAllTask();
}


void testThread()
{

	testTaskMgrEx();

	//testNotify();

	//testUpdateDtTask();

	//testTaskEx();

	//testTask();

	//testLock();
	//testMyThread();
}


#endif