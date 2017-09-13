#ifndef _FRAMEWORK_UTIL_SYS_UTIL_H_
#define _FRAMEWORK_UTIL_SYS_UTIL_H_

#include <string>

namespace csg
{
	class CSysUtil
	{
	public:
		//获取核数
		static int getNumberOfProcessors();
		//获取上次错误码--必须在错误发生时立即调用,否则中途会被其他函数重置
		static unsigned long getLastError();

		static void recordErrorCode(std::string info);
	};
#define CSG_RECORD_ERROR(X) CSysUtil::recordErrorCode(X)
}
#endif