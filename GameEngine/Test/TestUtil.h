#ifndef _TEST_TEST_UTIL_H_
#define _TEST_TEST_UTIL_H_

#include "framework/util/sysUtil.h"

void processNum()
{
	int num = csg::CSysUtil::getNumberOfProcessors();
	CSG_LOG_DEBUG("core=" << ToStr(num));
}



void testUtil()
{ 
	processNum();
}

#endif