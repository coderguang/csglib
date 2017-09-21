#ifndef _TEST_TEST_SERIALIZE_TEST_
#define _TEST_TEST_SERIALIZE_TEST_

#include "engine/serialize/serializestream.h"
#include "engine/def/csg_def.h"
#include <iostream>
#include "framework/counter/counterHandler.h"
#include <thread>
#include "framework/io/PrintIOHandler.h"

using namespace csg;

class SeriaTest
{
public:
	int aInt;
	bool bBool;
	//short sShort;
	byte_t bByte;
	long64_t lLong;
	
	std::string sStr;
	
	std::vector<int> vecInt;
	std::vector<std::string> vecStr;

	void read(CSerializeStream& os)
	{
		os.read(aInt);
		os.read(bBool);
		//os.read(sShort);
		os.read(bByte);
		os.read(lLong);
		os.read(sStr);
		
		os.read(vecInt);
		os.read(vecStr);
	}

	void write(CSerializeStream& os)
	{
		os.write(aInt);
		os.write(bBool);
		//os.write(sShort);
		os.write(bByte);
		os.write(lLong);
		os.write(sStr);
		
		os.write(vecInt);
		os.write(vecStr);
	}

};

class SeriaTestSimple
{
public:
	int aInt;
	bool bBool;

	void read(CSerializeStream& os)
	{
		os.read(aInt);
		os.read(bBool);
	}

	void write(CSerializeStream& os)
	{
		os.write(aInt);
		os.write(bBool);
	}

};

class SeriaTestVectorInt
{
public:
	int a;
	std::vector<int> vecInt;
	std::string str1;

	void read(CSerializeStream& os)
	{
		os.read(a);
		os.read(vecInt);
		os.read(str1);
	}

	void write(CSerializeStream& os)
	{
		os.write(a);
		os.write(vecInt);
		os.write(str1);
	}

};

class SeriaTestStr
{
public:
	std::string str1;
	int aInt;
	byte_t bByte;
	std::string str2;

	void read(CSerializeStream& os)
	{
		os.read(str1);
		os.read(aInt);
		os.read(bByte);
		os.read(str2);
	}

	void write(CSerializeStream& os)
	{
		os.write(str1);
		os.write(aInt);
		os.write(bByte);
		os.write(str2);
	}

};

class SeriaTestVectorStr
{
public:
	int i;
	std::vector<int> vecInt;
	std::vector<std::string> vec;

	void read(CSerializeStream& os)
	{
		os.read(i);
		os.read(vecInt);
		os.read(vec);
	}

	void write(CSerializeStream& os)
	{
		os.write(i);
		os.write(vecInt);
		os.write(vec);
	}

};


void testSerialize1()
{
	//testCounter(5,10);

	SeriaTest a;
	/*
	a.aInt = 0;
	a.bBool = false;
	a.sShort = 6;
	a.bByte = 0;
	a.lLong = 48888999;
	a.sStr = "gool lock";
	*/

	CSerializeStream os;
	os.setUseBitMark(false);
	a.write(os);

	int length = os.getDataSize();

	SeriaTest result;
	result.read(os);

	CSerializeStream ros;
	ros.setUseBitMark(true);
	result.write(ros);

	int flength = ros.getDataSize();

	SeriaTest finaTest;
	finaTest.read(ros);
}

void testSerialize2()
{
	//testCounter(5,10);

 	SeriaTest a;
// 	a.aInt = 0;
// 	a.bBool = false;
// 	a.sShort = 6;
// 	a.bByte = 0;
// 	a.lLong = 48888999;
// 	a.sStr = "gool lock";

	CSerializeStream src;
	src.setUseBitMark(true);
	a.write(src);

	int aLength = src.getDataSize();

	src.prepareToAppend();

	int length = src.getDataSize() + src.getFlagDataSize();

	CSerializeStream tmp;
	tmp.writeSize(src.getFlagDataSize());
	tmp.append(src.getFlagData() ,src.getFlagDataSize());
	tmp.append(src.getData() ,src.getDataSize());

	int tmplength = tmp.getDataSize();

	CSerializeStream getOs;
	getOs.append(tmp.getData() ,tmp.getDataSize());
	int getLength = getOs.getDataSize();
	getOs.prepareToRead();
	getOs.setUseBitMark(true);
	SeriaTest result;
	result.read(getOs);


	int k = 0;

}

void testSerialize3()
{
	//testCounter(5,10);

	SeriaTestSimple a;
	a.aInt = 1;
	a.bBool = false;

	CSerializeStream src;
	src.setUseBitMark(true);
	a.write(src);

	int aLength = src.getDataSize();

	src.prepareToAppend();

	int length = src.getDataSize() + src.getFlagDataSize();

	CSerializeStream tmp;
	tmp.writeSize(src.getFlagDataSize());
	tmp.append(src.getFlagData() ,src.getFlagDataSize());
	tmp.append(src.getData() ,src.getDataSize());

	int tmplength = tmp.getDataSize();

	CSerializeStream getOs;
	getOs.append(tmp.getData() ,tmp.getDataSize());
	int getLength = getOs.getDataSize();
	getOs.prepareToRead();

	SeriaTestSimple result;
	getOs.setUseBitMark(true);
	result.read(getOs);


	int k = 0;

}

void testSerialize4()
{
	//testCounter(5,10);

	SeriaTest sSrc;
	
	sSrc.aInt = 0;
	sSrc.bBool = false;
	
	//sSrc.sShort = 6;
	
	sSrc.bByte = 0;

	sSrc.lLong = 48888999;
	sSrc.sStr = "gool lock";
	
	for ( int i = 0; i < 30; i++ )
	{
		sSrc.vecInt.push_back(i);
	}  
	for ( int i = 0; i < 30; i++ )
	{
		std::string t = "test";
		sSrc.vecStr.push_back(t);
	}

	CSerializeStream src;
	src.setUseBitMark(true);
	sSrc.write(src);

	int aLength = src.getDataSize();

	src.prepareToAppend();

	int length = src.getDataSize() + src.getFlagDataSize();

	CSerializeStream tmp;
	tmp.writeSize(src.getFlagDataSize());
	tmp.append(src.getFlagData() ,src.getFlagDataSize());
	tmp.append(src.getData() ,src.getDataSize());

	int tmplength = tmp.getDataSize();

	CSerializeStream getOs;
	getOs.append(tmp.getData() ,tmp.getDataSize());
	int getLength = getOs.getDataSize();
	getOs.prepareToRead();
	getOs.setUseBitMark(true);
	SeriaTest result;
	result.read(getOs);


	int k = 0;

}

void testSerializeVecInt()
{
	SeriaTestVectorInt a;
	a.a = 0;
	a.str1 = "test o";
	for ( int i = 0; i < 30; i++ )
	{
		a.vecInt.push_back(i);
	}

	CSerializeStream src;
	src.setUseBitMark(true);
	a.write(src);

	int aLength = src.getDataSize();

	src.prepareToAppend();

	int length = src.getDataSize() + src.getFlagDataSize();

	CSerializeStream tmp;
	tmp.writeSize(src.getFlagDataSize());
	tmp.append(src.getFlagData() ,src.getFlagDataSize());
	tmp.append(src.getData() ,src.getDataSize());

	int tmplength = tmp.getDataSize();

	CSerializeStream getOs;
	getOs.append(tmp.getData() ,tmp.getDataSize());
	int getLength = getOs.getDataSize();
	getOs.prepareToRead();
	getOs.setUseBitMark(true);
	SeriaTestVectorInt result;
	result.read(getOs);


	int k = 0;

}

void testSerializeStr()
{
	SeriaTestStr a;
	a.str1 = "str1";
	a.str2 = "str2";
	a.aInt = 0;
	a.bByte = 0;

	CSerializeStream src;
	src.setUseBitMark(true);
	a.write(src);

	int aLength = src.getDataSize();

	src.prepareToAppend();

	int length = src.getDataSize() + src.getFlagDataSize();

	CSerializeStream tmp;
	tmp.writeSize(src.getFlagDataSize());
	tmp.append(src.getFlagData() ,src.getFlagDataSize());
	tmp.append(src.getData() ,src.getDataSize());

	int tmplength = tmp.getDataSize();

	CSerializeStream getOs;
	getOs.append(tmp.getData() ,tmp.getDataSize());
	int getLength = getOs.getDataSize();
	getOs.prepareToRead();
	getOs.setUseBitMark(true);
	SeriaTestStr result;
	result.read(getOs);


	int k = 0;


}

void testSerializeVecStr()
{
	SeriaTestVectorStr sSrc;
	sSrc.i = 0;
	for ( int i = 0; i < 30; i++ )
	{
		sSrc.vecInt.push_back(i % 5);
	}
	for ( int i = 0; i < 30; i++ )
	{
		sSrc.vec.push_back("test");
	}

	CSerializeStream src;
	//src.setUseBitMark(true);
	sSrc.write(src);

	int aLength = src.getDataSize();

	src.prepareToAppend();

	int length = src.getDataSize() + src.getFlagDataSize();

	CSerializeStream tmp;
	tmp.writeSize(src.getFlagDataSize());
	tmp.append(src.getFlagData() ,src.getFlagDataSize());
	tmp.append(src.getData() ,src.getDataSize());

	int tmplength = tmp.getDataSize();

	CSerializeStream getOs;
	getOs.append(tmp.getData() ,tmp.getDataSize());
	int getLength = getOs.getDataSize();
	getOs.prepareToRead();
	//getOs.setUseBitMark(true);
	SeriaTestVectorStr result;
	result.read(getOs);


	int k = 0;

}

class SeriaTestShort
{
public:
	short sShort;
	int aInt;
	void read(CSerializeStream& os)
	{
		os.read(sShort);
		os.read(aInt);
	}

	void write(CSerializeStream& os)
	{
		os.write(sShort);
		os.write(aInt);
	}

};

void testSerializeShort()
{
	SeriaTestShort a;
	a.sShort = 0;
	a.aInt = 5;
	CSerializeStream src;
	src.setUseBitMark(true);
	a.write(src);

	int aLength = src.getDataSize();

	src.prepareToAppend();

	int length = src.getDataSize() + src.getFlagDataSize();

	CSerializeStream tmp;
	tmp.writeSize(src.getFlagDataSize());
	tmp.append(src.getFlagData() ,src.getFlagDataSize());
	tmp.append(src.getData() ,src.getDataSize());

	int tmplength = tmp.getDataSize();

	CSerializeStream getOs;
	getOs.append(tmp.getData() ,tmp.getDataSize());
	int getLength = getOs.getDataSize();
	getOs.prepareToRead();
	getOs.setUseBitMark(true);
	SeriaTestShort result;
	result.read(getOs);


	int k = 0;
}


class SeriaTestShortException
{
public:
	int aInt;
	int bInt;
	short sShort;
	byte_t bByte;
	long64_t lLong;

	std::string sStr;

	std::vector<int> vecInt;
	std::vector<std::string> vecStr;

	void read(CSerializeStream& os)
	{
		os.read(sShort);
		os.read(aInt);
		os.read(bInt);
		os.read(vecStr);
		os.read(bByte);
		os.read(lLong);
		os.read(sStr);

		os.read(vecInt);
	}

	void write(CSerializeStream& os)
	{
		os.write(sShort);
		os.write(aInt);
		os.write(bInt);
		os.write(vecStr);
		os.write(bByte);
		os.write(lLong);
		os.write(sStr);

		os.write(vecInt);
	}

};

void testSerializeShortException()
{
	SeriaTestShortException sSrc;

	sSrc.aInt = 3;
	sSrc.bInt = 5;
	sSrc.sShort = 6;

	sSrc.bByte = 0;

	sSrc.lLong = 48888999;
	sSrc.sStr = "gool lock";
	
	for ( int i = 1; i < 50; i++ )
	{
		sSrc.vecInt.push_back(i);
	}
	
	
	for ( int i = 0; i < 56; i++ )
	{
		std::string t = "test";
		sSrc.vecStr.push_back(t);
	}
	
	
	CSerializeStream src;
	src.setUseBitMark(true);
	sSrc.write(src);

	int aLength = src.getDataSize();
	src.prepareToAppend();

	int flagSize = src.getFlagDataSize();

	int length = src.getDataSize() + src.getFlagDataSize();

	CSerializeStream tmp;
	tmp.writeSize(src.getFlagDataSize());
	tmp.append(src.getFlagData() ,src.getFlagDataSize());
	tmp.append(src.getData() ,src.getDataSize());

	int tmplength = tmp.getDataSize();

	CSerializeStream getOs;
	getOs.append(tmp.getData() ,tmp.getDataSize());
	int getLength = getOs.getDataSize();
	getOs.prepareToRead();
	getOs.setUseBitMark(true);
	SeriaTestShortException result;
	result.read(getOs);


	int k = 0;
}

class SeriaTestShortExceptionEx
{
public:
	int int1;
	int int2;
	int int3;
	int int4;
	int int5;
	int int6;
	int int7;
	int int8;
	int int9;

	void read(CSerializeStream& os)
	{
		os.read(int1);
		os.read(int2);
		os.read(int3);
		os.read(int4);
		os.read(int5);
		os.read(int6);
		os.read(int7);
		os.read(int8);
		os.read(int9);
	}

	void write(CSerializeStream& os)
	{
		os.write(int1);
		os.write(int2);
		os.write(int3);
		os.write(int4);
		os.write(int5);
		os.write(int6);
		os.write(int7);
		os.write(int8);
		os.write(int9);
	}

};

void testSerializeShortExceptionEx()
{
	SeriaTestShortExceptionEx sSrc;

	sSrc.int1 = 1;
	sSrc.int2 = 1;
	sSrc.int3 = 1;
	sSrc.int4 = 1;
	sSrc.int5 = 1;
	sSrc.int6 = 1;
	sSrc.int7 = 1;
	sSrc.int8 = 1;
	sSrc.int9 = 9;


	CSerializeStream src;
	src.setUseBitMark(true);
	sSrc.write(src);

	int aLength = src.getDataSize();
	src.prepareToAppend();

	int flagSize = src.getFlagDataSize();

	int length = src.getDataSize() + src.getFlagDataSize();

	CSerializeStream tmp;
	tmp.writeSize(src.getFlagDataSize());
	tmp.append(src.getFlagData() ,src.getFlagDataSize());
	tmp.append(src.getData() ,src.getDataSize());

	int tmplength = tmp.getDataSize();

	CSerializeStream getOs;
	getOs.append(tmp.getData() ,tmp.getDataSize());
	int getLength = getOs.getDataSize();
	getOs.prepareToRead();
	getOs.setUseBitMark(true);
	SeriaTestShortExceptionEx result;
	result.read(getOs);


	int k = 0;
}


class SeriaTestAll
{
public:
	byte_t bByte;
	std::vector<byte_t> bBytes;
	int iInt;
	std::vector<int> iInts;
	bool bBool;
	std::vector<bool> bBools;
	short sShort;
	std::vector<short> sShorts;
	long64_t lLong;
	std::vector<long64_t> lLongs;
	std::string str;
	std::vector<std::string> strs;

	void read(CSerializeStream& os)
	{
		os.read(bByte);
		os.read(bBytes);
		os.read(iInt);
		os.read(iInts);
		os.read(bBool);
		os.read(bBools);
		os.read(sShort);
		os.read(sShorts);
		os.read(lLong);
		os.read(lLongs);
		os.read(str);
		os.read(strs);
	}

	void write(CSerializeStream& os)
	{
		os.write(bByte);
		os.write(bBytes);
		os.write(iInt);
		os.write(iInts);
		os.write(bBool);
		os.write(bBools);
		os.write(sShort);
		os.write(sShorts);
		os.write(lLong);
		os.write(lLongs);
		os.write(str);
		os.write(strs);
	}

	bool operator == ( const SeriaTestAll& other )
	{
		if ( bByte != other.bByte )
		{
			return false;
		}
		if ( iInt != other.iInt )
		{
			return false;
		}
		if ( bBool != other.bBool )
		{
			return false;
		}
		if ( sShort != other.sShort )
		{
			return false;
		}
		if ( lLong != other.lLong )
		{
			return false;
		}
		if ( str != other.str )
		{
			return false;
		}

		if ( bBytes.size() != other.bBytes.size() )
		{
			return false;
		}
		for ( int i = 0; i < bBytes.size(); i++ )
		{
			if ( bBytes[i] != other.bBytes[i] )
			{
				return false;
			}
		}

		if ( iInts.size() != other.iInts.size() )
		{
			return false;
		}
		for ( int i = 0; i < iInts.size(); i++ )
		{
			if ( iInts[i] != other.iInts[i] )
			{
				return false;
			}
		}

		if ( bBools.size() != other.bBools.size() )
		{
			return false;
		}
		for ( int i = 0; i < bBools.size(); i++ )
		{
			if ( bBools[i] != other.bBools[i] )
			{
				return false;
			}
		}

		if ( sShorts.size() != other.sShorts.size() )
		{
			return false;
		}
		for ( int i = 0; i < sShorts.size(); i++ )
		{
			if ( sShorts[i] != other.sShorts[i] )
			{
				return false;
			}
		}

		if ( lLongs.size() != other.lLongs.size() )
		{
			return false;
		}
		for ( int i = 0; i < lLongs.size(); i++ )
		{
			if ( lLongs[i] != other.lLongs[i] )
			{
				return false;
			}
		}

		if ( strs.size() != other.strs.size() )
		{
			return false;
		}
		for ( int i = 0; i < strs.size(); i++ )
		{
			if ( strs[i] != other.strs[i] )
			{
				return false;
			}
		}

		return true;
	}
};

void testSerializeAll(int mod)
{
	SeriaTestAll src;
	src.bByte = 1;
	src.iInt = 2;
	src.bBool = true;
	src.sShort = 4;
	src.lLong = 5;
	src.str = "6";
	int randNum = rand() % mod;
	//randNum = 41;
	for ( int i = 0; i < randNum; i++ )
	{
		src.bBytes.push_back(i);
	}
	randNum = rand() % mod;
	//randNum = 12;
	for ( int i = 0; i < randNum; i++ )
	{
		src.iInts.push_back(i);
	}
	randNum = rand() % mod;
	//randNum = 12;
	for ( int i = 0; i < randNum; i++ )
	{
		src.bBools.push_back(i%2);
	}
	randNum = rand() % mod;
	//randNum = 21;
	for ( int i = 0; i < randNum; i++ )
	{
		src.sShorts.push_back(i);
	}
	for ( int i = 0; i < 100; i++ )
	{
		randNum = rand();
		src.lLongs.push_back(randNum);
	}
	randNum = rand() % mod;
	//randNum = 50;
	for ( int i = 0; i < randNum; i++ )
	{
		src.strs.push_back("hello");
	}


	CSerializeStream srcOS;
	srcOS.setUseBitMark(true);
	src.write(srcOS);
	srcOS.prepareToAppend();

	int srcDataLengtgh = srcOS.getDataSize();
	int srcFlagSize = srcOS.getFlagDataSize();

	CSerializeStream tmpOS;
	tmpOS.writeSize(srcOS.getFlagDataSize());
	tmpOS.append(srcOS.getFlagData() ,srcOS.getFlagDataSize());
	tmpOS.append(srcOS.getData() ,srcOS.getDataSize());

	int tmpSize = tmpOS.getDataSize();

	CSerializeStream restultOS;
	restultOS.append(tmpOS.getData() ,tmpOS.getDataSize());
	restultOS.prepareToRead();

	int resultDataLength = restultOS.getDataSize();
	int resultFlagLength = restultOS.getFlagDataSize();

	restultOS.setUseBitMark(true);

	SeriaTestAll result;
	result.read(restultOS);

	if ( result == src )
	{
		int t = 0;
		std::cout << "test ok=" << std::endl;
	} else
	{
		std::cout << "test error=" << std::endl;
		int p = 0;
		assert(false);
	}
}




void testSerialize()
{
	for ( int i = 0; i < 10000; i++ )
	{
		std::cout << "test i=" << i << std::endl;
		testSerializeAll(10);
	}
	int k = 0;
	testSerializeShortExceptionEx();
	
	testSerializeShortException();

	testSerializeShort();

	testSerialize4();

	testSerializeVecInt();

	testSerializeStr();

	testSerializeVecStr();
}


#endif