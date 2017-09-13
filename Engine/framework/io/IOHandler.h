#ifndef _FRAMEWORK_IO_IOHANDLER_H_
#define _FRAMEWORK_IO_IOHANDLER_H_

namespace csg
{

	class IIOHandler
	{
	public:
		IIOHandler()
		{
		}
		virtual ~IIOHandler()
		{
		}

		virtual int write(const void* data ,const int len ,void *act)
		{
			return -1;
		}
		virtual int read(const void* data ,const int len ,void* act)
		{
			return -1;
		}

		virtual int open(const void* handle ,void *act)
		{
			return -1;
		}

		virtual int close()
		{
			return -1;
		}
	};
}
#endif