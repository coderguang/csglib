
#ifndef _FRAMEWORK_LOG_LOG_H_
#define _FRAMEWORK_LOG_LOG_H_

#include <iostream>
#include "../datetime/datetime.h"
#include "../DesignPattern/Singleton.h"
#include <vector>
#include "engine/thread/thread.h"
#include "../util/sysUtil.h"

namespace csg
{

	enum ELogLevel{
		ELogLevelUnknow=0,
		ELogSystem=1,//系统log
		ELogError =2, //严重错误
		ELogWarnning =4, //警告
		ELogInfo = 8,  //常规信息
		ELogDebug = 16, //调试信息
	};


	// log数据管理
	class CLogRecord
	{
	public:
		//默认参数构造效率更高
		CLogRecord(const std::string& msg ,ELogLevel lv) :logLv(lv) ,logStr(msg) ,logDt(CDateTime()) ,threadId(CThread::threadId()) ,pid(CThread::pid())
		{
		
		}
		void appendLog(const std::string &str);

		ELogLevel logLv;
		CDateTime logDt;
		std::string logStr;
		int threadId;
		int pid;
	};

	typedef std::vector<CLogRecord> SeqLogRecord;

	CLogRecord& operator<< ( CLogRecord& out ,const std::string& str );

	CLogRecord& operator<< ( CLogRecord& out ,const int v );

	CLogRecord& operator<< ( CLogRecord& out ,const double v );

	CLogRecord& operator<< ( CLogRecord& out ,const long64_t v );

	//这个bool类型会导致 字符串变量被优先匹配
	//CLogRecord& operator<< ( CLogRecord& out ,const bool v );


	class CLogManager :public Singleton<CLogManager>
	{
	public:
		void add(const std::string& logStr ,ELogLevel level=ELogInfo);

		void getLogRecord(SeqLogRecord& records);

		void add(const CLogRecord& newRecord);
	protected:
	
	private:
		SeqLogRecord _logRecord;
	};
}

#define CSG_LOG_SYS(X)\
	{\
	CLogRecord record("",csg::ELogSystem);\
	record<<X;\
	csg::CLogManager::instance()->add(record);\
	}
#define CSG_LOG_ERR(X) \
	{\
	CLogRecord record("",csg::ELogError);\
	record<<X;\
	csg::CLogManager::instance()->add(record);\
	}

#define CSG_LOG_WARN(X)\
	{\
	CLogRecord record("",csg::ELogWarnning);\
	record<<X;\
	csg::CLogManager::instance()->add(record);\
	}
#define CSG_LOG_INFO(X) \
	{\
	CLogRecord record("",csg::ELogInfo);\
	record<<X;\
	csg::CLogManager::instance()->add(record);\
	}

#define CSG_LOG_DEBUG(X) \
	{\
	CLogRecord record("",csg::ELogDebug);\
	record<<X;\
	csg::CLogManager::instance()->add(record);\
	}

#endif