#ifndef _FRAMEWORK_UTIL_ENDIAN_H_
#define _FRAMEWORK_UTIL_ENDIAN_H_

#include "engine/def/csg_def.h"

namespace csg
{
	template <typename T>
		inline T endian( T s )
	{
#ifdef CSG_LITTLE_ENDIAN
		return s;
#else
		T v1;
		byte_t*   = (byte_t*)&s + sizeof(T) - 1;
		byte_t* q = (byte_t*)&v1;
		switch( sizeof( s ) )
		{
		case 8:
			*q ++ = *p -- ;
			*q ++ = *p -- ;
			*q ++ = *p -- ;
			*q ++ = *p -- ;
		case 4:
			*q ++ = *p -- ;
			*q ++ = *p -- ;
		case 2:
			*q ++ = *p -- ;
		case 1:
			*q = *p;
			break;
		default:
			assert( false );
			break;
		}
		return v1;
#endif
	}
}
#endif
