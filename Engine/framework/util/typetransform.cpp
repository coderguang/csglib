
#include "framework/util/typetransform.h"
#include "stdio.h"

csg::CTypeTransform::CTypeTransform(int v)
{
	ss << v;
}

std::string csg::CTypeTransform::toString()const
{
	return ss.str();
}

csg::CTypeTransform::~CTypeTransform()
{

}

csg::CTypeTransform::CTypeTransform(unsigned long v)
{
	ss << v;
}

csg::CTypeTransform::CTypeTransform(unsigned long long v)
{
	ss << v;
}

csg::CTypeTransform::CTypeTransform(long64_t v)
{
	ss << v;
}

csg::CTypeTransform::CTypeTransform(bool v)
{
	
	if ( v )
	{
		ss <<"true";
	} else
	{
		ss << "false";
	}
	
}

csg::CTypeTransform::CTypeTransform(short v)
{
	ss << v;
}

csg::CTypeTransform::CTypeTransform(unsigned int v)
{
	ss << v;
}

csg::CTypeTransform::CTypeTransform(double v)
{
	ss.precision(CSG_TRANSFORM_DOUBLE_PRECISION);
	ss << v;
}

csg::CTypeTransform::CTypeTransform(float v)
{
	ss.precision(CSG_TRANSFORM_FLOAT_PRECISION);
	ss << v;
}
