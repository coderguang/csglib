#ifndef _FRAMEWORK_UTIL_TYPE_TRANSFORM_H_
#define _FRAMEWORK_UTIL_TYPE_TRANSFORM_H_

#include "engine/def/csg_def.h"
#include <string>
#include <sstream>

namespace csg
{
	static const int CSG_TRANSFORM_FLOAT_PRECISION = 15;
	static const int CSG_TRANSFORM_DOUBLE_PRECISION = 20;

	class CTypeTransform
	{
	public:
		
		CTypeTransform(int v);
		//float 精确为 12 位( 11.23 精度为4位 ) 会丢失一定的精度
		CTypeTransform(float v);
		//double 精确为 20 位 会丢失一定的精度
		CTypeTransform(double v);
		CTypeTransform(unsigned int v);
		CTypeTransform(short v);
		CTypeTransform(bool v);
		CTypeTransform(long64_t v);
		CTypeTransform(unsigned long long v);
		CTypeTransform(unsigned long v);
	public:
		std::string toString()const;
		virtual ~CTypeTransform();
	private:
		std::stringstream ss;
	};
}

#define  ToStr(V)  csg::CTypeTransform(V).toString()

#endif