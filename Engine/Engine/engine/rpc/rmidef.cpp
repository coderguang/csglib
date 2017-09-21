#include "engine/rpc/rmidef.h"
#include "framework/exception/exception.h"

using namespace csg;

void csg::_csg_write(CSerializeStream& __os ,ERMIMessageType __messageType)
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

void csg::_csg_read(CSerializeStream& __is ,ERMIMessageType& __messageType)
{
	byte_t value;
	__is.read(value);
	__messageType = static_cast<ERMIMessageType>( value );
	if ( __messageType > 3 || __messageType < 1 )
	{
		throw CException("ExceptionCodeSerialize" ,ExceptionCodeSerialize);
	}
}


csg::SRMIInfo::SRMIInfo()
{
	_csg_init();
}

bool
csg::SRMIInfo::operator==( const SRMIInfo& __rhs ) const
{
	return !operator!=( __rhs );
}

bool
csg::SRMIInfo::operator!=( const SRMIInfo& __rhs ) const
{
	if ( this == &__rhs )
	{
		return false;
	}
	if ( identify != __rhs.identify )
	{
		return true;
	}
	if ( operation != __rhs.operation )
	{
		return true;
	}
	return false;
}

bool
csg::SRMIInfo::operator<( const SRMIInfo& __rhs ) const
{
	if ( this == &__rhs )
	{
		return false;
	}
	if ( identify < __rhs.identify )
	{
		return true;
	} else if ( __rhs.identify < identify )
	{
		return false;
	}
	if ( operation < __rhs.operation )
	{
		return true;
	} else if ( __rhs.operation < operation )
	{
		return false;
	}
	return false;
}

void
csg::SRMIInfo::_csg_init()
{
}

void
csg::SRMIInfo::_csg_write(CSerializeStream& __os) const
{
	__os.write(identify);
	__os.write(operation);
}

void
csg::SRMIInfo::_csg_read(CSerializeStream& __is)
{
	__is.read(identify);
	__is.read(operation);
}


csg::SRMICall::SRMICall()
{
	_csg_init();
}

bool
csg::SRMICall::operator==( const SRMICall& __rhs ) const
{
	return !operator!=( __rhs );
}

bool
csg::SRMICall::operator!=( const SRMICall& __rhs ) const
{
	if ( this == &__rhs )
	{
		return false;
	}
	if ( messageId != __rhs.messageId )
	{
		return true;
	}
	if ( rpcId != __rhs.rpcId )
	{
		return true;
	}
	return false;
}

bool
csg::SRMICall::operator<( const SRMICall& __rhs ) const
{
	if ( this == &__rhs )
	{
		return false;
	}
	if ( messageId < __rhs.messageId )
	{
		return true;
	} else if ( __rhs.messageId < messageId )
	{
		return false;
	}
	if ( rpcId < __rhs.rpcId )
	{
		return true;
	} else if ( __rhs.rpcId < rpcId )
	{
		return false;
	}
	return false;
}

void
csg::SRMICall::_csg_init()
{
	messageId = 0;
	rpcId = 0;
}

void
csg::SRMICall::_csg_write(CSerializeStream& __os) const
{
	__os.write(messageId);
	__os.write(rpcId);
}

void
csg::SRMICall::_csg_read(CSerializeStream& __is)
{
	__is.read(messageId);
	__is.read(rpcId);
}



csg::SRMIReturn::SRMIReturn()
{
	_csg_init();
}

bool
csg::SRMIReturn::operator==( const SRMIReturn& __rhs ) const
{
	return !operator!=( __rhs );
}

bool
csg::SRMIReturn::operator!=( const SRMIReturn& __rhs ) const
{
	if ( this == &__rhs )
	{
		return false;
	}
	if ( messageId != __rhs.messageId )
	{
		return true;
	}
	if ( dispatchStatus != __rhs.dispatchStatus )
	{
		return true;
	}
	return false;
}

bool
csg::SRMIReturn::operator<( const SRMIReturn& __rhs ) const
{
	if ( this == &__rhs )
	{
		return false;
	}
	if ( messageId < __rhs.messageId )
	{
		return true;
	} else if ( __rhs.messageId < messageId )
	{
		return false;
	}
	if ( dispatchStatus < __rhs.dispatchStatus )
	{
		return true;
	} else if ( __rhs.dispatchStatus < dispatchStatus )
	{
		return false;
	}
	return false;
}

void
csg::SRMIReturn::_csg_init()
{
	messageId = 0;
	dispatchStatus = ::csg::ERMIDispatchResultOk;
}

void
csg::SRMIReturn::_csg_write(CSerializeStream& __os) const
{
	__os.write(messageId);
	::csg::_csg_write(__os ,dispatchStatus);
}

void
csg::SRMIReturn::_csg_read(CSerializeStream& __is)
{
	__is.read(messageId);
	::csg::_csg_read(__is ,dispatchStatus);
}
