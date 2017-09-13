
#include "engine/baseServer/loggerWritter.h"
#include <iostream>
#include "framework/util/typetransform.h"
#include "engine/def/csg_def.h"

#ifdef CSG_WIN
#include <direct.h>
#include <errno.h>
#define mkdir(x,y) _mkdir(x)
#endif
#ifdef CSG_LINUX
#include <sys/stat.h>
#endif


csg::CLoggerWritterTask::~CLoggerWritterTask()
{
	_ofs.flush();
	_ofs.close();
}

void csg::CLoggerWritterTask::setConsoleInfo(bool show)
{
	_consoleShow = show;
}

void csg::CLoggerWritterTask::setFileDir(const std::string& dir)
{
	_dir = dir;
	if ( -1 == mkdir(_dir.c_str(),0777) )
	{
		if ( EEXIST != errno )
		{
			CSG_LOG_ERR("CLoggerWritterTask::setFileDir,mkdir "<<_dir<<" error!,errno="+ToStr(errno));
			return ;
		}
	}
	CSG_LOG_SYS("CLoggerWritterTask::setFileDir,mkdir "<<_dir<<" complete!");
}

int csg::CLoggerWritterTask::run()
{
	CSG_LOG_SYS("CLoggerWritterTask::run start");
	std::unique_lock<CLock> lock(getLock());
	initLogMsgMap();
	while ( !isExit() )
	{
		if ( isStop() )
		{
			CSG_LOG_SYS("CLoggerWritterTask::run watting");
			_cv.wait(lock);
		}
		flush();
		CThread::sleep_for(500); //500ms 刷一次log
	}
	CSG_LOG_SYS("CLoggerWritterTask::run exit");
	flush();//刷完最后的log
	return CSG_RETURN_OK;
}

void csg::CLoggerWritterTask::flush()
{
	CLogManager::instance()->getLogRecord(_logRecord);
	write();
}


void csg::CLoggerWritterTask::initLogMsgMap()
{
	_logMsgMap[ELogLevelUnknow] = "LogUnkonw";
	_logMsgMap[ELogSystem] = "LogSys";
	_logMsgMap[ELogError] = "LogErr";
	_logMsgMap[ELogWarnning] = "LogWarn";
	_logMsgMap[ELogInfo] = "LogInfo";
	_logMsgMap[ELogDebug] = "LogDebug";
}

void csg::CLoggerWritterTask::write()
{
	for ( SeqLogRecord::const_iterator it = _logRecord.begin(); it != _logRecord.end(); ++it )
	{

#ifndef _DEBUG
		if(ELogDebug==it->logLv)
			continue;
#endif

		std::string logMsg = "[" + ToStr(it->pid) + "][" + ToStr(it->threadId) + "] " + it->logDt.asString() + ": " + _logMsgMap[it->logLv] + " : " + it->logStr;
		write(it->logDt,logMsg);
	}
	_logRecord.clear(); 
}

void csg::CLoggerWritterTask::write(const CDateTime& logDt ,const std::string& msg)
{
	if ( _consoleShow )
		std::cout <<msg << std::endl;

	std::string logFileName = logDt.asString("YYYY-MM-DD");
	int totalDay = logDt.getTotalDay();
	if ( _lastWriteTotalDay != logDt.getTotalDay() )
	{
		if ( _ofs.is_open() )
			_ofs.close();
		_ofs.open(_dir + "/" + logFileName + ".log",std::ios::app);
		if ( !_ofs.is_open() )
		{
			CSG_LOG_ERR("csg::CLoggerWritterTask::write,cant open "<<logFileName);
			return;
		}
		_lastWriteTotalDay = logDt.getTotalDay();
	}
	_ofs << msg<<std::endl;
}

