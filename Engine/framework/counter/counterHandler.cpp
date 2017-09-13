#include "framework/counter/counterHandler.h"
#include "framework/io/IOHandler.h"
#include <strstream>
#include "framework/counter/counterHandlerManager.h"
#include "engine/lock/lockObjectManager.h"
#include <iostream>
#include "../util/typetransform.h"
#include "../log/Log.h"


using namespace csg;


CCounterHandler::CCounterHandler(const std::string& name)
:_name(name) 
{
	_count = 0;
	_countEx = 0;
	CCounterHandlerManager::registCounterHandler(this);
}
CCounterHandler::~CCounterHandler()
{
	CCounterHandlerManager::removeCounterHandler(this);
}

void CCounterHandler::increaseCount(int add)
{
	_count += add;
}
void CCounterHandler::decreaseCount(int dec)
{
	_count -= dec;
}

void CCounterHandler::increaseCountEx(int add)
{
	_countEx += add;
}
void CCounterHandler::decreaseCountEx(int dec)
{
	_countEx -= dec;
}

void CCounterHandler::print(IIOHandler* handle)
{
	CSG_LOG_SYS("CounterHandlerName:"<<_name<<" _count:"<<ToStr(_count)<< ",_countEx:"<<ToStr(_countEx));
}
