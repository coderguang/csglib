#include "framework/util/sysUtil.h"
#include "engine/def/csg_def.h"

#ifdef CSG_LINUX
#include<unistd.h>
#include <sys/syscall.h>
#include <errno.h>
#elif defined(CSG_WIN)
//#include <winbase.h>
#endif
#include "../log/Log.h"
#include "typetransform.h"

int csg::CSysUtil::getNumberOfProcessors()
{
	int num = 0;
#ifdef CSG_WIN
	SYSTEM_INFO mySysInfo;
	GetSystemInfo(&mySysInfo);
	num=mySysInfo.dwNumberOfProcessors;
#elif defined(CSG_LINUX)
	num = sysconf(_SC_NPROCESSORS_CONF);
#endif
	return num;
}

unsigned long csg::CSysUtil::getLastError()
{
	unsigned long num = 0;
#ifdef CSG_WIN
	num= GetLastError();
#elif defined(CSG_LINUX)
	num = errno;
#endif
	return num;
}

void csg::CSysUtil::recordErrorCode(std::string info)
{
	unsigned long errnoCode = getLastError();
	CSG_LOG_INFO(info << ",errorCode=" << ToStr(errnoCode));
}

