#ifndef _FRAMEWORK_UTIL_COUNTERHANDLER_MANAGER_H_
#define _FRAMEWORK_UTIL_COUNTERHANDLER_MANAGER_H_

#include <list>
#include "framework/counter/counterHandler.h"
#include "framework/DesignPattern/Singleton.h"
#include "framework/io/IOHandler.h"

namespace csg
{
	class CCounterHandlerManager
	{
	public:
	
		static void registCounterHandler( CCounterHandler* handler);

		static void removeCounterHandler( CCounterHandler* handler);

		static void printCounterHandler(IIOHandler* handle);

	private:
		static std::list<CCounterHandler*>& getCounterHandler();
	};



}
#endif