
#ifndef _ENGINE_BASE_SERVER_LOGGER_WRITTER_H_
#define _ENGINE_BASE_SERVER_LOGGER_WRITTER_H_

#include "../thread/task.h"
#include "framework/log/Log.h"
#include <map>
#include <fstream>

namespace csg
{
	class CLoggerWritterTask :public virtual CTask
	{
	public:
		CLoggerWritterTask() :CTask() ,_consoleShow(false) ,_lastWriteTotalDay(0),_dir("")
		{
			setTaskType(EBaseServerLoggerWritter);
			_logRecord.clear();
			_logMsgMap.clear();
		}
		virtual ~CLoggerWritterTask();

		void setConsoleInfo(bool show);

		virtual void setFileDir(const std::string& dir);

	protected:
		virtual int run();

	private:
		void flush();

		void initLogMsgMap();

		virtual void write();

		virtual void write(const CDateTime& logDt,const std::string& msg);
	private:
		SeqLogRecord _logRecord;
		std::map<int ,std::string> _logMsgMap;

		std::string _dir;
		std::ofstream _ofs;
		bool _consoleShow;
		int _lastWriteTotalDay;
	};
}
#endif