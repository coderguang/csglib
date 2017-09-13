#include "engine/thread/thread.h"
#ifdef CSG_WIN
#include <process.h>
#elif defined CSG_LINUX
#include <unistd.h>
#include <sys/syscall.h>
#endif
#include <sstream>
using namespace csg;

int csg::CThread::threadId()
{
#ifdef CSG_WIN
	csg_thread_id id = get_id();
	std::stringstream s;
	s << id;
	return atoi(s.str().c_str());
#else
	return syscall(SYS_gettid);
#endif

}

csg::csg_thread_id csg::CThread::get_id()
{
	return std::this_thread::get_id();
}

int csg::CThread::pid()
{
	return getpid();
}

void csg::CThread::sleep_for(long64_t milliseconds)
{
	std::chrono::milliseconds ms(milliseconds);
	std::this_thread::sleep_for(ms);
}

void csg::CThread::sleep_until(CDateTime dt)
{
	std::this_thread::sleep_until(std::chrono::system_clock::from_time_t(dt.getTotalSecond()));
}

void csg::CThread::yeild()
{
	std::this_thread::yield();
}

void csg::CThread::join(csg_thread& thr)
{
	thr.join();
}

void csg::CThread::deatch(csg_thread& thr)
{
	thr.detach();
}

