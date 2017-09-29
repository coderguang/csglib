#include "framework/datetime/datetime.h"
#include <chrono>
#include "../exception/exception.h"
#include <string.h>
#include <iostream>

using namespace csg;

int CDateTime::_localTimeZone = -8;
const CInterval CInterval::_zero(0,0);

const int SECONDS_TO_MILLSECONDS=1000;
const int TIME_ZERO_YEAR = 1900;

CDateTime CDateTime::_lastLocalTime(0);

csg::CDateTime::CDateTime() :_tm(NULL)
{
	*this = getThreadDt();
}

csg::CDateTime::~CDateTime()
{
	_tm = NULL;
}

csg::CDateTime::CDateTime(const CDateTime& other) :_tm(NULL)
{
	_timeSpan = other._timeSpan;
}

int csg::CDateTime::getYear()const 
{
	return getLocalTime()->tm_year + TIME_ZERO_YEAR;
}

int csg::CDateTime::getMonth()const
{
	return getLocalTime()->tm_mon + 1;
}

int csg::CDateTime::getDay() const
{
	return getLocalTime()->tm_mday;
}

int csg::CDateTime::getHour() const
{
	return getLocalTime()->tm_hour;
}

int csg::CDateTime::getMinute() const
{
	return getLocalTime()->tm_min;
}

int csg::CDateTime::getSecond() const
{
	return getLocalTime()->tm_sec;
}

int csg::CDateTime::getDayOfWeek() const
{
	return getLocalTime()->tm_wday;
}

int csg::CDateTime::getMillSecond() const
{
	return _timeSpan%SECONDS_TO_MILLSECONDS;
}

void csg::CDateTime::clearMillSecond()
{
	_timeSpan -= getMillSecond();
}

int csg::CDateTime::getTotalDay() const
{
	return ( _timeSpan - _localTimeZone* 60 * 60 * SECONDS_TO_MILLSECONDS ) / ( 24 * 60 * 60 * SECONDS_TO_MILLSECONDS );
}

long64_t csg::CDateTime::getTotalMill() const
{
	return _timeSpan;
}

const CDateTime& csg::CDateTime::operator+=( const CInterval& interval )
{
	clearTm();
	_timeSpan += interval._timeSpan;
	return *this;
}

const CDateTime& csg::CDateTime::operator-=( const CInterval& interval )
{
	clearTm();
	_timeSpan -= interval._timeSpan;
	return *this;
}

CDateTime& csg::CDateTime::operator=( const CDateTime& other )
{
	clearTm();
	_timeSpan = other._timeSpan;
	return *this;
}

csg::CDateTime csg::CDateTime::operator+( const CInterval& interval ) const
{
	CDateTime datetime(*this);
	datetime += interval;
	return datetime;
}

csg::CDateTime csg::CDateTime::operator-( const CInterval& other ) const
{
	CDateTime datetime(*this);
	datetime -= other;
	return datetime;
}

csg::CInterval csg::CDateTime::operator-( const CDateTime& other ) const
{
	long64_t interval;
	interval = _timeSpan - other._timeSpan;
	return CInterval(interval);
}

long64_t csg::CDateTime::getTotalSecond()const
{
	return _timeSpan / SECONDS_TO_MILLSECONDS;
}

csg::CDateTime::CDateTime(long64_t millseconds) :_tm(NULL)
{
	_timeSpan = millseconds;
}

CLock& csg::CDateTime::getLock()
{
	static CLock lock;
	return lock;
}

bool csg::CDateTime::operator==( const CDateTime& other ) const
{
	return _timeSpan == other._timeSpan;
}

bool csg::CDateTime::operator!=( const CDateTime& other ) const
{
	return _timeSpan != other._timeSpan;
}

bool csg::CDateTime::operator<=( const CDateTime& other ) const
{
	return _timeSpan <= other._timeSpan;
}

bool csg::CDateTime::operator<( const CDateTime& other ) const
{
	return _timeSpan < other._timeSpan;
}

bool csg::CDateTime::operator>(const CDateTime& other) const
{
	return _timeSpan > other._timeSpan;
}

bool csg::CDateTime::operator>=( const CDateTime& other ) const
{
	return _timeSpan >= other._timeSpan;
}

std::string csg::CDateTime::asString(const char* format) const
{
	char buf[100];
	sprintf(buf ,"%s" ,format);
	int len = (int) strlen(format);
	int index = 0;
	time_t showTime = getTotalSecond();
	csg_struct_time time = *localtime(&showTime);
	unsigned long mills = (long) ( _timeSpan % 1000 );
	for ( index = 0; index < len; )
	{
		char ch = format[index];
		switch ( ch )
		{
			case 'Y':
			case 'M':
			case 'D':
			case 'h':
			case 'm':
			case 's':
			case 'n':
			{
						int count = 1;
						while ( format[++index] == ch )
							count++;

						if ( !asYMD(&time ,mills ,ch ,buf ,index - count ,count ,sizeof( buf )) )
							throw CException("CDateTime Format Error!" ,ExceptionCodeDatetime);
			};
			break;
			default:
			index++;
		}
	}
	return std::string(buf);
}

void csg::CDateTime::parse(const char* value ,const char* format )
{
	int len = (int) strlen(format);
	int vallen = (int) strlen(value);
	int index = 0;
	struct tm ltm;
	unsigned long millis = 0;
	for ( index = 0; index < len; )
	{
		char ch = format[index];
		switch ( ch )
		{
			case 'Y':
			case 'M':
			case 'D':
			case 'h':
			case 'm':
			case 's':
			case 'n':
			{
						int count = 1;
						while ( format[++index] == ch )
							count++;

						if ( !parseYMD(&ltm ,&millis ,ch ,value ,index - count ,vallen ,count) )
						{
							throw CException("CDateTime Format Error!" ,ExceptionCodeDatetime);
						}
			};
			break;
			default:
			index++;
		}
	}
	time_t timeV = mktime(&ltm);
	if ( timeV == -1 )
	{
		throw CException("CDateTime Format Error!" ,ExceptionCodeDatetime);
	}
	long64_t mill = timeV;
	mill = mill * 1000 + millis;
	init(mill);
}

bool CDateTime::parseYMD(csg_struct_time* time ,
			  unsigned long* millis ,
			  const char ch ,
			  const char* value ,
			  int index ,
			  int length ,
			  int count
			  )
{
	char buf[5];
	int nvalue = 0;
	if ( index + count > length )
		return false;
	switch ( ch )
	{
		case 'Y':
		{
					if ( count == 2 || count == 4 )
					{
						memcpy(buf ,value + index ,count);
						buf[count] = 0;
						nvalue = atoi(buf);
					} else
						return false;

					if ( count == 2 )
						nvalue += 2000;
					if ( nvalue >= 1970 )
						nvalue -= 1900;
					else
						return false;
					time->tm_year = nvalue;
		}
		break;
		case 'M':
		{
					if ( count > 2 )
						return false;
					memcpy(buf ,value + index ,count);
					buf[count] = 0;
					nvalue = atoi(buf);
					if ( nvalue > 12 || nvalue <= 0 )
						return false;
					time->tm_mon = nvalue - 1;
		}
		break;
		case 'D':
		{
					if ( count > 2 )
						return false;
					memcpy(buf ,value + index ,count);
					buf[count] = 0;
					nvalue = atoi(buf);
					if ( nvalue > 31 || nvalue <= 0 )
						return false;
					time->tm_mday = nvalue;
		}
		break;
		case 'h':
		{
					if ( count > 2 )
						return false;
					memcpy(buf ,value + index ,count);
					buf[count] = 0;
					nvalue = atoi(buf);
					if ( nvalue > 23 || nvalue < 0 )
						return false;
					time->tm_hour = nvalue;
		}
		break;
		case 'm':
		{
					if ( count > 2 )
						return false;
					memcpy(buf ,value + index ,count);
					buf[count] = 0;
					nvalue = atoi(buf);
					if ( nvalue > 59 || nvalue < 0 )
						return false;
					time->tm_min = nvalue;
		}
		break;
		case 's':
		{
					if ( count > 2 )
						return false;
					memcpy(buf ,value + index ,count);
					buf[count] = 0;
					nvalue = atoi(buf);
					if ( nvalue > 59 || nvalue < 0 )
						return false;
					time->tm_sec = nvalue;
		}
		break;
		case 'n':
		{
					if ( count > 3 )
						return false;
					memcpy(buf ,value + index ,count);
					buf[count] = 0;
					nvalue = atoi(buf);
					if ( nvalue < 0 )
						return false;
					*millis = nvalue;
		}
		break;
		default:
		return false;
	}
	return true;
}


bool csg::CDateTime::asYMD(csg_struct_time* time ,unsigned long mills ,const char ch ,char* buf ,int index ,int count ,int bufSize) const
{
	if ( index + count >= bufSize )
		return false;
	char change;
	char format[5];
	sprintf(format ,"%%0%dd" ,count);
	change = buf[index + count];
	switch ( ch )
	{
		case 'Y':
		if ( count == 2 )
			sprintf(buf + index ,format ,time->tm_year);
		else if ( count == 4 )
			sprintf(buf + index ,format ,time->tm_year + TIME_ZERO_YEAR);
		else
			return false;
		break;
		case 'M':
		if ( count > 2 )
			return false;
		sprintf(buf + index ,format ,time->tm_mon + 1);
		break;
		case 'D':
		if ( count > 2 )
			return false;
		sprintf(buf + index ,format ,time->tm_mday);
		break;
		case 'h':
		if ( count > 2 )
			return false;
		sprintf(buf + index ,format ,time->tm_hour);
		break;
		case 'm':
		if ( count > 2 )
			return false;
		sprintf(buf + index ,format ,time->tm_min);
		break;
		case 's':
		if ( count > 2 )
			return false;
		sprintf(buf + index ,format ,time->tm_sec);
		break;
		case 'n':
		if ( count > 3 )
			return false;
		sprintf(buf + index ,format ,mills);
		break;
		default:
		return false;
	}
	buf[index + count] = change;
	return true;
}

void csg::CDateTime::updateThreadDt()
{
	CAutoLock l(getLock());
	_lastLocalTime.update();
}

csg::CDateTime csg::CDateTime::getThreadDt()
{
	CAutoLock l(getLock());
	if ( 0 == _lastLocalTime.getTotalMill() )
	{
		_lastLocalTime.update();
	}
	return _lastLocalTime;
}

void csg::CDateTime::update()
{
	using namespace std::chrono;
	time_point<system_clock ,milliseconds> timepoint = time_point_cast<milliseconds>(system_clock::now());
	milliseconds ms = duration_cast<milliseconds>( timepoint.time_since_epoch());
	init(ms.count());
}

const csg_struct_time* csg::CDateTime::getLocalTime()const
{
	if ( _tm )
	{
		return _tm;
	}
	time_t timeSec = getTotalSecond();
	*const_cast<csg_struct_time**>( &_tm ) = localtime(&timeSec);
	assert(_tm);
	return _tm;
}

void csg::CDateTime::setTimeZone(int timeZone)
{
	_localTimeZone = timeZone;
}

void csg::CDateTime::clearTm()
{
 	_tm = NULL;
}

bool csg::CDateTime::init(long64_t millseconds)
{
	clearTm();
	_timeSpan = millseconds;
	return true;
}

bool csg::CDateTime::init(int year ,int month ,int day ,int hour ,int minute ,int second ,int millseconds )
{
	clearTm();
	csg_struct_time ltm;
	ltm.tm_year = year - TIME_ZERO_YEAR;
	ltm.tm_mon = month-1;
	ltm.tm_mday = day;
	ltm.tm_hour = hour;
	ltm.tm_min = minute;
	ltm.tm_sec = second;
	
	time_t tmV = mktime(&ltm);
	if ( millseconds < 0 || millseconds>999 || -1 == tmV )
	{
		_timeSpan = 0;
		return false;
	}
	_timeSpan = tmV*SECONDS_TO_MILLSECONDS + millseconds;
	return true;
}



bool csg::CInterval::asYMD(const char ch ,char* buf ,int index ,int count ,int bufsize) const
{
	if ( index + count >= bufsize )
		return false;
	char change;
	char format[5];
	sprintf(format ,"%%0%dd" ,count);
	change = buf[index + count];
	switch ( ch )
	{
		case 'D':
		if ( count > 2 )
			return false;
		sprintf(buf + index ,format ,this->getDays());
		break;
		case 'h':
		if ( count > 2 )
			return false;
		sprintf(buf + index ,format ,this->getHoursOfDay());
		break;
		case 'm':
		if ( count > 2 )
			return false;
		sprintf(buf + index ,format ,this->getMinutesOfDay());
		break;
		case 's':
		if ( count > 2 )
			return false;
		sprintf(buf + index ,format ,this->getSecondsOfDay());
		break;
		case 'n':
		if ( count > 3 )
			return false;
		sprintf(buf + index ,format ,this->getMillsOfDay());
		break;
		default:
		return false;
	}
	buf[index + count] = change;
	return true;
}

csg::CInterval::CInterval(int days ,int hours ,int minutes ,int seconds ,int millseconds)
{
	_timeSpan = millseconds;
	_timeSpan += long64_t (seconds) * SECONDS_TO_MILLSECONDS;
	_timeSpan += long64_t (minutes) * 60 * SECONDS_TO_MILLSECONDS;
	_timeSpan += long64_t (hours) * 60 * 60 * SECONDS_TO_MILLSECONDS;
	_timeSpan += long64_t (days) * 24 * 60 * 60 * SECONDS_TO_MILLSECONDS;
}

int csg::CInterval::getDays() const
{
	return _timeSpan / ( 24 * 60 * 60 * SECONDS_TO_MILLSECONDS );
}

int csg::CInterval::getTotalSeconds() const
{
	return _timeSpan / SECONDS_TO_MILLSECONDS;
}

int csg::CInterval::getTotalMinutes() const
{
	return _timeSpan / ( 60 * SECONDS_TO_MILLSECONDS );
}

int csg::CInterval::getTotalHours() const
{
	return _timeSpan / ( 60 * 60 * SECONDS_TO_MILLSECONDS );
}

long64_t csg::CInterval::getTotalMills() const
{
	return _timeSpan;
}

int csg::CInterval::getSecondsOfDay() const
{
	return getTotalSeconds() - getTotalMinutes() * 60;
}

int csg::CInterval::getMinutesOfDay() const
{
	return getTotalMinutes() - getTotalHours() * 60;
}

int csg::CInterval::getHoursOfDay() const
{
	return getTotalHours() - getDays() * 24;
}

int csg::CInterval::getMillsOfDay() const
{
	return _timeSpan -getTotalSeconds()*SECONDS_TO_MILLSECONDS;
}

const CInterval& csg::CInterval::operator+=( const CInterval& other )
{
	_timeSpan += other._timeSpan;
	return CInterval(_timeSpan);
}

void csg::CInterval::parse(const char* value ,const char* format /*= CDF_DEFAULT_INTERVAL_FORMAT*/)
{
	_timeSpan = 0;
	int len = (int) strlen(format);
	int vallen = (int) strlen(format);
	int index = 0;
	for ( index = 0; index < len; )
	{
		char ch = format[index];
		switch ( ch )
		{
			case 'D':
			case 'h':
			case 'm':
			case 's':
			case 'n':
			{
						int count = 1;
						while ( format[++index] == ch )
							count++;

						if ( !parseIntervalYMD(&_timeSpan ,ch ,value ,index - count ,vallen ,count) )
						{

							throw CException("Interval Format Error!" ,ExceptionCodeDatetime);
						}
			};
			break;
			default:
			index++;
		}
	}
}

std::string csg::CInterval::asString(const char* format /*= CDF_DEFAULT_INTERVAL_FORMAT*/) const
{
	char buf[100];
	sprintf(buf ,"%s" ,format);
	int len = (int) strlen(format);
	int index = 0;
	for ( index = 0; index < len; )
	{
		char ch = format[index];
		switch ( ch )
		{
			case 'D':
			case 'h':
			case 'm':
			case 's':
			case 'n':
			{
						int count = 1;
						while ( format[++index] == ch )
						{
							count++;
						}
						if ( !asYMD(ch ,buf ,index - count ,count ,sizeof( buf )) )
						{
							throw CException("Interval Format Error!" ,ExceptionCodeDatetime);
						}
			}
			break;
			default:
			index++;
		}
	}
	return std::string(buf);
}

bool csg::CInterval::operator>=( const CInterval& other ) const
{
	return _timeSpan >= other._timeSpan;
}

bool csg::CInterval::operator>( const CInterval& other ) const
{
	return _timeSpan > other._timeSpan;
}

bool csg::CInterval::operator<=( const CInterval& other ) const
{
	return _timeSpan <= other._timeSpan;
}

bool csg::CInterval::operator<( const CInterval& other ) const
{
	return _timeSpan < other._timeSpan;
}

bool csg::CInterval::operator!=( const CInterval& other ) const
{
	return _timeSpan != other._timeSpan;
}

bool csg::CInterval::operator==( const CInterval& other ) const
{
	return _timeSpan == other._timeSpan;
}

const CInterval& csg::CInterval::operator-=( const CInterval& other )
{
	_timeSpan -= other._timeSpan;
	return *this;
}

csg::CInterval csg::CInterval::operator+( const CInterval& other ) const
{
	CInterval result(*this);
	result += other;
	return result;
}

csg::CInterval::CInterval()
{
	_timeSpan = 0;
}

csg::CInterval::CInterval(const long64_t millseconds)
{
	_timeSpan = millseconds;
}

csg::CInterval::CInterval(const int second ,const int millseconds)
{
	_timeSpan = long64_t(second)*SECONDS_TO_MILLSECONDS;
	_timeSpan += millseconds;
}

csg::CInterval::CInterval(const CInterval& other)
{
	_timeSpan = other._timeSpan;
}

csg::CInterval csg::CInterval::operator-( const CInterval& other ) const
{
	CInterval result(*this);
	result -= other;
	return result;
}

bool csg::CInterval::parseIntervalYMD(
	long64_t* time ,const char ch ,const char* value ,
	int index ,int length ,int count
	)
{
	char buf[5];
	int nvalue = 0;
	if ( index + count > length )
	{
		return false;
	}
	switch ( ch )
	{
		case 'D':
		{
					if ( count > 2 )
						return false;
					memcpy(buf ,value + index ,count);
					buf[count] = 0;
					nvalue = atoi(buf);
					if ( nvalue < 0 )
						return false;
					*time += nvalue * 24 * 60*60 * SECONDS_TO_MILLSECONDS;
		}
		break;
		case 'h':
		{
					if ( count > 2 )
					{
						return false;
					}
					memcpy(buf ,value + index ,count);
					buf[count] = 0;
					nvalue = atoi(buf);
					if ( nvalue >= 24 || nvalue < 0 )
						return false;
					*time += nvalue * 60*60 * SECONDS_TO_MILLSECONDS;
		}
		break;
		case 'm':
		{
					if ( count > 2 )
					{
						return false;
					}
					memcpy(buf ,value + index ,count);
					buf[count] = 0;
					nvalue = atoi(buf);
					if ( nvalue >= 60 || nvalue < 0 )
						return false;
					*time += nvalue * 60 * SECONDS_TO_MILLSECONDS;
		}
		break;
		case 's':
		{
					if ( count > 2 )
					{
						return false;
					}
					memcpy(buf ,value + index ,count);
					buf[count] = 0;
					nvalue = atoi(buf);
					if ( nvalue >= 60 || nvalue < 0 )
					{
						return false;
					}
					*time += nvalue * SECONDS_TO_MILLSECONDS;
		}
		break;
		case 'n':
		{
					if ( count > 3 )
					{
						return false;
					}
					memcpy(buf ,value + index ,count);
					buf[count] = 0;
					nvalue = atoi(buf);
					if ( nvalue < 0 )
					{
						return false;
					}
					*time += nvalue;
		}
		break;
		default:
		return false;
	}
	return true;
}