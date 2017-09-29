#include "framework/timer/timer.h"

using namespace csg;

csg::CTimerBase::CTimerBase(std::string name) :_timerName(name)
{

}

csg::CTimerBase::~CTimerBase()
{

}

int csg::CTimerBase::handleTimeOut(CDateTime& current)
{
	return 0;
}

void csg::CTimerBase::handleException(const CException& ex)
{
	CSG_LOG_ERR("CTimerBase::handleException,name="<<_timerName<<",ex.what="<<ex.what()<<",ex.code="<<ex.code());
}

std::string csg::CTimerBase::getTimerName() const
{
	return _timerName;
}

bool csg::CTimerBase::operator<( const CTimerBase& other )
{
	return this < &other;
}

