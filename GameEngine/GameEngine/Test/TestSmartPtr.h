
#ifndef _TEST_SMART_PTR_H_
#define _TEST_SMART_PTR_H_

#include <memory>
#include "framework/log/Log.h"
#include "framework/SmartPoint/refObject.h"
#include "framework/counter/counterHandler.h"
#include "framework/counter/counterHandlerManager.h"
#include "engine/net/session/session.h"
#include "engine/net/session/sessionManager.h"

using namespace  csg;
class TestPoint
{
public:
	std::string _name;
	int a;
};

void testFirst()
{
	shared_ptr<TestPoint> a(new TestPoint());
	//CSG_LOG_DEBUG("count1=" + ToStr(a->use_count()));

	std::cout << "count=" << a.use_count() << std::endl;

	shared_ptr<TestPoint> b(a);
	std::cout << "acount=" << a.use_count() << std::endl;
	std::cout << "bcount=" << a.use_count() << std::endl;
} 

void testObject2(CRefObject &obj)
{
	int k = 0;
	
}

void testObject1()
{
	CRefObject* obj=new CRefObject();
	CRefObject* obj2 = new CRefObject();;
	obj2 = obj;

	CRefObject* obj3 = obj;

	testObject2(*obj2);
}

class CTestSmartP :public virtual CRefObject
{
public: 
	CTestSmartP(int a ,std::string s) :_a(a) ,_str(s)
	{
	}
	std::string print()
	{
		return "a="+ToStr(_a) + ",str=" + _str+",ref="+ToStr(getRef());
	}
private:
	int _a;
	std::string _str;
};
typedef CSmartPointShare<CTestSmartP> CTestSmartPtr;

class CTestEx2 :public virtual CTestSmartP
{
public:
	CTestEx2() :CTestSmartP(111,"tes2")
	{
	}
};
typedef CSmartPointShare<CTestEx2> CTestEx2Ptr;

class CTestEx :public virtual CRefObject
{

};
typedef CSmartPointShare<CTestEx> CTestExPtr;

void testSp(){
	CTestSmartPtr t1 = new CTestSmartP(10 ,"t1");
	CSG_LOG_DEBUG(t1->print());
	CTestSmartPtr t2 = t1;
	CSG_LOG_DEBUG("after t2"+t1->print());
	CSG_LOG_DEBUG("t2=" + t2->print());
	{
		CTestSmartPtr t3;
		try
		{
			CSG_LOG_DEBUG("t3=" + t3->print());
		} catch ( const CException& ex )
		{

		}
		t3 = CTestSmartPtr::dynamicCast(t2);
		CSG_LOG_DEBUG("t3=" + t3->print()); 
	}
	CSG_LOG_DEBUG("after t1" + t1->print());
}

void testSp2()
{
	CTestSmartPtr t1 = new CTestSmartP(10 ,"t1");
	CSG_LOG_DEBUG(t1->print());
	{
		CTestSmartPtr t2 = t1;
		CSG_LOG_DEBUG("after t2" + t1->print());
		CSG_LOG_DEBUG("t2=" + t2->print());
		{
			CTestSmartPtr t3;
			try
			{
				CSG_LOG_DEBUG("t3=" + t3->print());
			} catch ( const CException& ex )
			{

			}
			t3 = CTestSmartPtr::dynamicCast(t2);
			CSG_LOG_DEBUG("t3: " + t3->print());
		}
	}
	CSG_LOG_DEBUG("after t1" + t1->print());
}

void testSmart3()
{
	CTestSmartPtr t1 = new CTestSmartP(10 ,"t1");
	CSG_LOG_DEBUG(t1->print());
	{
		CTestSmartPtr t2 = t1;
		CSG_LOG_DEBUG("after t2" + t1->print());
		CSG_LOG_DEBUG("t2=" + t2->print());
		{
			CTestSmartPtr t3;
			try
			{
				CSG_LOG_DEBUG("t3=" + t3->print());
			} catch ( const CException& ex )
			{

			}
			t3 = CTestSmartPtr::dynamicCast(t2);
			CSG_LOG_DEBUG("t3: " + t3->print());
		}
	}
	CSG_LOG_DEBUG("after t1" + t1->print());

	CTestExPtr t4 = CTestExPtr::dynamicCast(t1);

	CTestEx2Ptr t5 = CTestEx2Ptr::dynamicCast(t1);

	CTestEx2Ptr t6 = new CTestEx2();

	CSG_LOG_DEBUG("after t1" + t6->print());

	CTestSmartPtr t7 = CTestSmartPtr::dynamicCast(t6);

	CSG_LOG_DEBUG("after t7" + t7->print());

	int k = 0;

	CCounterHandlerManager::printCounterHandler(NULL);

}

class CTestSessionMgr :public virtual Singleton<CTestSessionMgr>
{
public:
	void addSession(CSession& se)
	{
		//_map[1] = se;
	};
	void addSessionEx(CSessionPtr& see)
	{
		_mapEx[see->getSocketfd()] = see;
	}
private:
	std::map<int ,CSession> _map;
	MapSession _mapEx;

};


void testSession()
{
	std::string ip = "127.0.0.1";
	CSessionPtr sessionEx = new CSession(10 ,ip ,1002 ,true);

	CSession session(10 ,ip ,1002 ,true);

	CTestSessionMgr::instance()->addSession(session);

	CTestSessionMgr::instance()->addSessionEx(sessionEx);

	CSessionManager::instance()->addSession(sessionEx);

	CSG_LOG_DEBUG("ok");

}

void testSmartPtr()
{
	testSession();

	//testSmart3();

	CCounterHandlerManager::printCounterHandler(NULL);

	//testSp2();

	//testSp();

	//testObject1();

	//testFirst();
}


#endif