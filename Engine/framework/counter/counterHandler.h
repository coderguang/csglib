#ifndef _FRAMEWORK_UTIL_COUNTER_HANDLER_H_
#define _FRAMEWORK_UTIL_COUNTER_HANDLER_H_

#include "framework/io/IOHandler.h"
#include <string>
#include "engine/def/csg_def.h"

namespace csg
{

	class CCounterHandler
	{
	public:
		CCounterHandler(const std::string& name);
		virtual ~CCounterHandler();
		virtual void print();

		void increaseCount(int add=1);

		void decreaseCount(int dec=1);

		void increaseCountEx(int add=1);

		void decreaseCountEx(int dec=1);

	private:
		CCounterHandler()
		{
			_count = 0;
			_countEx = 0;
		}
		CCounterHandler(const CCounterHandler&)
		{
		}
		std::string _name;
		csg_atomic_int _count;
		csg_atomic_int _countEx;
	};
}
#endif