#include "framework/exception/exception.h"
#include <string.h>

using namespace csg;

CNullHandleException::CNullHandleException(const char* file ,int nline)
{
	std::ostringstream str;
	str << "CNullHandleException " << file << " (" << nline << ")";
	_msg = str.str();
}

CNullHandleException::~CNullHandleException()
{

}