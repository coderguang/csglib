
#ifndef _TEST_TEST_LOG_H_
#define _TEST_TEST_LOG_H_

#include "framework/util/typetransform.h"

void baseLogTest()
{
	do
	{
		
		CDateTime dt;
		std::string printLog = " now my data is " + dt.asString();;
		CLogManager::instance()->add(printLog);
		CThread::sleep_for(2000);
	} while ( true );
}

void testTypeTransform()
{
	CDateTime dt;
	std::string printLogEx = " now my data is " + dt.asString();;
	std::cout << "date=" << printLogEx<<"time="<<dt.getTotalMill() << std::endl;

	int i = 5;
	float f = 798.012345678912345678;
	float ff = 798.01;
	double d = 798.012345678912345678;
	double dd = 798.012;
	bool bt = true;
	bool bf = false;
	long64_t l64 = dt.getTotalMill();

// 	std::string is = ToStr(i);
// 	std::cout << "i=" << is << std::endl;
// 	is.clear();
// 	is = ToStr(f);
// 	std::cout << "f=" << is << std::endl;
// 	is.clear();
// 	is = ToStr(d);
// 	std::cout << "d=" << is << std::endl;
// 	std::string ffs = ToStr(ff);
// 	std::cout << "ff=" << ffs << std::endl;
// 	std::string dds = ToStr(dd);
// 	std::cout << "dd=" << dds << std::endl;
// 	is.clear();
// 	is = ToStr(bt);
// 	std::cout << "bt=" << is << std::endl;
// 	is.clear();
// 	is = ToStr(bf);
// 	std::cout << "bf=" << is << std::endl;
// 	is.clear();
// 	is = ToStr(l64);
// 	std::cout << "l64=" << is << std::endl;

	//std::cout << "is:" << is << "  ffs:" << ffs << std::endl;

	//std::cout << ToStr(i) << std::endl;
 	//std::cout << "i:" << ToStr(i) << std::endl;
// 
 	std::string printLog = "i:" + ToStr(i) + "\n" +"f:" + ToStr(f) + "\n" +"d:" + ToStr(d) + "\n" +"bt:" + ToStr(bt) + "\n" +"bf:" + ToStr(bf) + "\n" +"l64:" + ToStr(l64) + "\n";
	CLogManager::instance()->add(printLog);

}

void testTypeTransformEx()
{
	float f = 798.012345678912345678;
	float ff = 798.01;
	double d = 798.012345678912345678;
	double dd = 798.012;
	std::string is;
	is = ToStr(f);
	std::cout << "f=" << is << std::endl;
	is = ToStr(d);
	std::cout << "d=" << is << std::endl;
	std::string ffs = ToStr(ff);
	std::cout << "ff=" << ffs << std::endl;
	std::string dds = ToStr(dd);
	std::cout << "dd=" << dds << std::endl;
}

void testTypeTransform2()
{
	float f = 798.012345678912345678;
	float ff = 798.01;
	double d = 798.012345678912345678;
	double dd = 798.012;

	std::string is;
	
	is = ToStr(f);
	std::cout << "f=" << is << std::endl;
	is.clear();
	is = ToStr(d);
	std::cout << "d=" << is << std::endl;
	std::string ffs = ToStr(ff);
	std::cout << "ff=" << ffs << std::endl;
	std::string dds = ToStr(dd);
	std::cout << "dd=" << dds << std::endl;
}

void testTypePrint()
{
	float f = 798.012345678912345678;
	double d = 798.012345678912345678;

	CTypeTransform tc(f);
	CTypeTransform dc(d);

	std::string strc = ToStr(f) + ToStr(d);
	
}

void testLog()
{
	//testTypePrint();

	//testTypeTransform2();

	//testTypeTransform();

	//testTypeTransformEx();

	baseLogTest();

}


#endif