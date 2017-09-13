#ifndef _FRAMEWORK_IO_PRINT_IO_HANDLER_H_
#define _FRAMEWORK_IO_PRINT_IO_HANDLER_H_

#include "framework/io/IOHandler.h"
#include "framework/log/Log.h"

namespace csg
{
	class CPrintIOHandler
		:public IIOHandler
	{
	public:
		virtual int write(const void* data ,const int len ,void *act)
		{
			std::string str((char*) data,len);
// 			CLog log;
// 			log.Print(str ,"CPrintIOHandler");
			return 0;
		}
	
	};


}
#endif