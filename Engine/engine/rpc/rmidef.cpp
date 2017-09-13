#include "engine/rpc/rmidef.h"
#include "framework/exception/exception.h"

using namespace csg;

void csg::_csg_write(CSerializeStream& __os ,EMessageType __messageType)
{
	__os.write(static_cast<byte_t>( __messageType ));
}

void csg::_csg_read(CSerializeStream& __is ,ERMIDispatchResult& __result)
{
	byte_t value;
	__is.read(value);
	__result = static_cast<ERMIDispatchResult>( value );
	if ( __result > 4 || __result < 1 )
	{
		throw CException("ExceptionCodeSerialize" ,ExceptionCodeSerialize);
	}
}

void csg::_csg_write(CSerializeStream& __os ,ERMIDispatchResult __result)
{
	__os.write(static_cast<byte_t>(__result));
}

void csg::_csg_read(CSerializeStream& __is ,EMessageType& __messageType)
{
	byte_t value;
	__is.read(value);
	__messageType = static_cast<EMessageType>( value );
	if ( __messageType > 3 || __messageType < 1 )
	{
		throw CException("ExceptionCodeSerialize" ,ExceptionCodeSerialize);
	}
}
