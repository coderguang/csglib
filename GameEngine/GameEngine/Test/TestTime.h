
#ifndef _TEST_TEST_TIME_H_
#define _TEST_TEST_TIME_H_

#include <iostream>
#include "engine/def/csg_def.h"
#ifdef CSG_WIN
#include <Windows.h>
#endif
#include "framework/datetime/datetime.h"
#include <chrono>

using namespace std::chrono;

void getWindowsTime()
{
#ifdef CSG_WIN
	//声明变量
	SYSTEMTIME sys_time;

	//将变量值设置为本地时间
	GetLocalTime(&sys_time);

	//输出时间
	printf("%4d/%02d/%02d %02d:%02d:%02d.%03d 星期%1d\n" ,sys_time.wYear ,
		   sys_time.wMonth ,
		   sys_time.wDay ,
		   sys_time.wHour ,
		   sys_time.wMinute ,
		   sys_time.wSecond ,
		   sys_time.wMilliseconds ,
		   sys_time.wDayOfWeek);

	system("time");

	system("pause");
#endif

}

void getCppTime()
{
	struct tm* lockTime;
	time_t tmt;
	lockTime=gmtime(&tmt);
	

	system("time");

	system("pause");
}

void testCppExample()
{
#define MST (-7)
#define UTC (0)
#define CCT (+8)

	time_t rawtime;
	struct tm  *ptm;

	time(&rawtime);

	ptm=gmtime(&rawtime);

	puts("Current time around the World:");
	printf("Phoenix, AZ (U.S.) :  %2d:%02d\n" ,( ptm->tm_hour + MST ) % 24 ,ptm->tm_min);
	printf("Reykjavik (Iceland) : %2d:%02d\n" ,( ptm->tm_hour + UTC ) % 24 ,ptm->tm_min);
	printf("Beijing (China) :     %2d:%02d\n" ,( ptm->tm_hour + CCT ) % 24 ,ptm->tm_min);

	printf("Current local time and date tm: %s" ,asctime(ptm));

	time_t nowT = mktime(ptm);

	printf("Current local time and date time_t: %s" ,ctime(&nowT));

	time_t rawtimeEx;
	struct tm  *ptmEx;

	time(&rawtimeEx);

	ptmEx=localtime(&rawtimeEx);

	printf("Current local time and date tm: %s" ,asctime(ptmEx));

	time_t nowTEx = mktime(ptmEx);

	printf("Current local time and date time_t: %s" ,ctime(&nowTEx));

}

struct STimeTest
{
	int k;
	int i;
};

void testMyTime()
{
	
	std::chrono::system_clock::time_point today = std::chrono::system_clock::now();

	CDateTime dt;

	printf("Current year: %d\n" ,dt.getYear());
	printf("Current mon: %d\n" ,dt.getMonth());
	printf("Current day: %d\n" ,dt.getDay());
	printf("Current hour: %d\n" ,dt.getHour());
	printf("Current min: %d\n" ,dt.getMinute());
	printf("Current sec: %d\n" ,dt.getSecond());
	printf("Current mil: %d\n" ,dt.getMillSecond());
	printf("Current wday: %d\n" ,dt.getDayOfWeek());
	
	CDateTime dtt;
	dtt.init(dt.getYear() ,dt.getMonth() ,dt.getDay() ,dt.getHour() ,dt.getMinute() ,dt.getSecond() ,dt.getMillSecond());

	printf("Current year: %d\n" ,dtt.getYear());
	printf("Current mon: %d\n" ,dtt.getMonth());
	printf("Current day: %d\n" ,dtt.getDay());
	printf("Current hour: %d\n" ,dtt.getHour());
	printf("Current min: %d\n" ,dtt.getMinute());
	printf("Current sec: %d\n" ,dtt.getSecond());
	printf("Current mil: %d\n" ,dtt.getMillSecond());
	printf("Current wday: %d\n" ,dtt.getDayOfWeek());

	system("pause");
}


void testTime11()
{
	using std::chrono::system_clock;
	
	 std::chrono::duration<int ,std::ratio<60 * 60 * 24> > one_day(1);
	
	 system_clock::time_point today = system_clock::now();
	 system_clock::time_point tomorrow = today + one_day;
	
	 time_t tt;
	
		  tt = system_clock::to_time_t(today);
	  std::cout << "today is: " << ctime(&tt);
	
		  tt = system_clock::to_time_t(tomorrow);
	  std::cout << "tomorrow will be: " << ctime(&tt);

	  using namespace std::chrono;
	 
	 steady_clock::time_point t1 = steady_clock::now();
	 
		   std::cout << "printing out 1000 stars...\n";
	    for ( int i = 0; i < 1000; ++i ) std::cout << "*";
	   std::cout << std::endl;
	  
		   steady_clock::time_point t2 = steady_clock::now();
	  
		   duration<double> time_span = duration_cast<duration<double>>( t2 - t1 );
	
		    std::cout << "It took me " << time_span.count() << " seconds.";
	    std::cout << std::endl;
		

		typedef duration<int ,std::ratio<60 * 60 * 24>> days_type;
	
		time_point<system_clock ,days_type> todayEx = time_point_cast<days_type>( system_clock::now() );
		
		std::cout << todayEx.time_since_epoch().count() << " days since epoch" << std::endl;
		
		
		system("pause");
	
}

void testTimeval()
{
	 struct timeval* tm;

	 clock_t start,end;
	 
	 start = clock();

	 //Sleep(500);
	 int k=0;
	 for ( int i = 0; i < 9999999; i++ )
		 k++;

	 end = clock();

	 std::cout << "start=" << start << ",end=" << end << ",result=" << end - start << std::endl;

// 	 struct timeb tb;
// 	 ftime(&tb);
// 	 printf(".%03d" ,tb.millitm);

	 system("pause");

}

void testMyTimeEx()
{
	CDateTime dt;

	std::chrono::system_clock::time_point today = std::chrono::system_clock::now();
	time_t tt = std::chrono::system_clock::to_time_t(today);
	std::cout << "tomorrow will be: " << ctime(&tt);
	system("pause");
}

void testInteral()
{
// 	CDateTime dt;
// 
// 	std::chrono::system_clock::time_point today = std::chrono::system_clock::now();
// 	auto tp=today.time_since_epoch().count();
// 
// 	time_t cur_time = std::chrono::duration_cast<std::chrono::milliseconds>( today.time_since_epoch() ).count();
// 
// 	std::string str_time = ctime(&cur_time);
// 	cout << str_time << endl;


	CInterval it1(1800 ,0);
	CInterval it2(2100 ,0);

	std::string testStrInter = it1.asString();

	std::cout << "it<<" << it1.asString()<<",it2="<<it2.asString()<<",sum="<<(it1+it2).asString()<<",del="<<(it2-it1).asString()<<std::endl;

	CInterval it3;
	it3.parse(testStrInter.c_str());

	std::cout << "it3=" << it3.asString() << std::endl;;
	system("pause");
}

void testAllTime()
{
	CDateTime dF ,dS;
	CInterval dI(10 * 60 ,0);
	//std::chrono::system_clock::time_point today = std::chrono::system_clock::now();
	//std::cout << "df" << dF.asString() <<",ds:"<<dF.asString()<< std::endl;
	//std::cout << "dI" << dI.asString() << std::endl;;
	CDateTime result = dF + dI;
	//std::cout << "add" << result.asString() << std::endl;;
	{
		CDateTime result = dF + dI;
		printf("Current year: %d\n" ,result.getYear());
		printf("Current mon: %d\n" ,result.getMonth());
		printf("Current day: %d\n" ,result.getDay());
		printf("Current hour: %d\n" ,result.getHour());
		printf("Current min: %d\n" ,result.getMinute());
		printf("Current sec: %d\n" ,result.getSecond());
		printf("Current mil: %d\n" ,result.getMillSecond());
		printf("Current wday: %d\n" ,result.getDayOfWeek());
		result = dS - dI;
	}
	//std::cout << "del" << result.asString() << std::endl;;
	/*
	printf("Current year: %d\n" ,dF.getYear());
	printf("Current mon: %d\n" ,dF.getMonth());
	printf("Current day: %d\n" ,dF.getDay());
	printf("Current hour: %d\n" ,dF.getHour());
	printf("Current min: %d\n" ,dF.getMinute());
	printf("Current sec: %d\n" ,dF.getSecond());
	printf("Current mil: %d\n" ,dF.getMillSecond());
	printf("Current wday: %d\n" ,dF.getDayOfWeek());

	printf("Current year: %d\n" ,result.getYear());
	printf("Current mon: %d\n" ,result.getMonth());
	printf("Current day: %d\n" ,result.getDay());
	printf("Current hour: %d\n" ,result.getHour());
	printf("Current min: %d\n" ,result.getMinute());
	printf("Current sec: %d\n" ,result.getSecond());
	printf("Current mil: %d\n" ,result.getMillSecond());
	printf("Current wday: %d\n" ,result.getDayOfWeek());
	*/
}

void testFinaTime()
{
	CDateTime dt;
	std::cout <<"totalMils="<<dt.getTotalMill()<<std::endl<<",str:"<< dt.asString() << std::endl;

	std::cout << "year=" << dt.getYear() << std::endl;
	std::cout << "month=" << dt.getMonth() << std::endl;
	std::cout << "day=" << dt.getDay() << std::endl;
	std::cout << "hour=" << dt.getHour() << std::endl;
	std::cout << "minute=" << dt.getMinute() << std::endl;
	std::cout << "second=" << dt.getSecond() << std::endl;
	std::cout << "millseconds=" << dt.getMillSecond() << std::endl;
	std::cout << "wday=" << dt.getDayOfWeek() << std::endl;

	CInterval di(1800 ,0);
	CDateTime result = dt + di;

	std::cout << di.asString() << std::endl;
	std::cout << result.asString() << std::endl;
	std::cout << "year=" << result.getYear() << std::endl;
	result = dt - di;

	std::cout << di.asString() << std::endl;
	std::cout << result.asString() << std::endl;
	std::cout << "year=" << result.getYear() << std::endl;

	CDateTime pt;
	pt.parse(result.asString().c_str());

	std::cout << "year=" << pt.asString() << std::endl;
}

void testException()
{
	CDateTime dt;
	std::cout << dt.asString() << std::endl;

	CInterval di(1800 ,0);
	CDateTime result = dt + di;

//  	std::cout << di.asString() << std::endl;
//  	std::cout << result.asString() << std::endl;
  	std::cout << "year=" << result.getYear() << std::endl;
	result = dt - di;

	//std::cout << di.asString() << std::endl;
	//std::cout << result.asString() << std::endl;
	//std::cout << "year=" << result.getYear() << std::endl;
}

void whyThisException()
{
	//CDateTime dt;
	//CInterval di(1800 ,0);
	CDateTime result;// = dt + di;
	result.getYear();
	//std::cout << "year=" << result.getYear() << std::endl;
	//result = dt - di;

}

void testTime()
{
	//whyThisException();

	//testException();

	testFinaTime();

	//testAllTime();

	//testInteral();

	//testMyTimeEx();

	//testTime11();

	//testMyTime();

	//testTimeval();

	//testCppExample();

	//getWindowsTime();

	//getCppTime();
}



#endif