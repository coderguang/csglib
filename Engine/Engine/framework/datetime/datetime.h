#ifndef _FRAMEWORK_DATETIME_H_
#define _FRAMEWORK_DATETIME_H_
#include "engine/def/csg_def.h"
#include "engine/lock/lock.h"
#include <chrono>
#include <string>



namespace csg
{
	using namespace std;

	typedef std::chrono::system_clock::time_point csg_time;
	typedef struct tm csg_struct_time;

#define CDF_DEFAULT_DATETIME_FORMAT "YYYY-MM-DD hh:mm:ss.nnn"
#define CDF_NORMAL_DATETIME_FORMAT "YYYY-MM-DD hh:mm:ss"
#define CDF_DEFAULT_INTERVAL_FORMAT "DD hh:mm:ss.nnn"

	class CInterval;

	class CDateTime
	{
	public:

		CDateTime();

		CDateTime(const CDateTime& other);

		CDateTime(long64_t millseconds);

		virtual ~CDateTime();

	public:
		
		int getYear()const;

		int getMonth()const;

		int getDay()const;

		int getHour()const;

		int getMinute()const;

		int getSecond()const;

		int getDayOfWeek()const;

		int getMillSecond()const;

		void clearMillSecond();

		int getTotalDay()const;

		long64_t getTotalSecond()const;
		
		long64_t getTotalMill()const;

		CInterval operator -( const CDateTime& other ) const;

		CDateTime operator -( const CInterval& interval ) const;

		CDateTime operator +( const CInterval& interval ) const;

		CDateTime& operator =( const CDateTime& other );

		const CDateTime& operator -=( const CInterval& interval );

		const CDateTime& operator +=( const CInterval& interval );

		bool operator==( const CDateTime& other ) const;

		bool operator !=( const CDateTime& other ) const;

		bool operator <=( const CDateTime& other ) const;

		bool operator <( const CDateTime& other ) const;

		bool operator >( const CDateTime& other ) const;

		bool operator >=( const CDateTime& other ) const;
	public:
		std::string asString(const char* format = CDF_DEFAULT_DATETIME_FORMAT) const;

		void parse(const char* value ,const char* format = CDF_DEFAULT_DATETIME_FORMAT);

	private:
		bool asYMD(csg_struct_time* time ,unsigned long mills ,
				   const char ch ,char* buf ,int index ,
				   int count ,int bufsize) const;
		bool parseYMD(csg_struct_time* time ,
					  unsigned long* millis ,
					  const char ch ,
					  const char* value ,
					  int index ,
					  int length ,
					  int count
					  );
	public:

		static void updateThreadDt();

		bool init(long64_t millseconds);

		bool init(int year ,int month ,int day ,int hour ,int minute ,int second ,int millseconds);

		const csg_struct_time* getLocalTime()const;

		static void setTimeZone(int timeZone);
	private:

		//线程时间操控函数
		static CLock& getLock();

		static CDateTime getThreadDt();

		void update();

		void clearTm();
	private:
		time_t _timeSpan; //毫秒级
		csg_struct_time* _tm;
		static int _localTimeZone; //时区
	private: //系统时间设置
		static CDateTime _lastLocalTime; //真实系统时间,由时间线程更新
	};


	class CInterval
	{
	public:
		const static CInterval _zero;
	private:
		friend class CDateTime;
		long64_t _timeSpan;

		bool asYMD(const char ch ,char* buf ,int index ,int count ,int bufsize) const;

		bool parseIntervalYMD(
			long64_t* time ,const char ch ,const char* value ,
			int index ,int length ,int count
			);
	public:

		CInterval(int days ,int hours ,int minutes ,int seconds ,int millseconds);

		CInterval(const CInterval& other);

		CInterval(const int second,const int millseconds);

		CInterval(const long64_t millseconds);

		CInterval();

		int getDays() const;

		int getTotalSeconds() const;

		int getTotalMinutes() const;

		int getTotalHours() const;
		long64_t getTotalMills() const;

		int getSecondsOfDay() const;

		int getMinutesOfDay() const;

		int getHoursOfDay() const;

		int getMillsOfDay() const;

		CInterval operator -( const CInterval& other ) const;

		CInterval operator +( const CInterval& other ) const;

		const CInterval& operator -=( const CInterval& other );

		const CInterval& operator +=( const CInterval& other );

		bool operator ==( const CInterval& other ) const;

		bool operator !=( const CInterval& other ) const;

		bool operator <( const CInterval& other ) const;

		bool operator <=( const CInterval& other ) const;

		bool operator >( const CInterval& other ) const;

		bool operator >=( const CInterval& other ) const;

		void parse(const char* value ,const char* format = CDF_DEFAULT_INTERVAL_FORMAT);

		std::string asString(const char* format = CDF_DEFAULT_INTERVAL_FORMAT) const;

	};

}
#endif