#include "engine/mq/msgBlock.h"
#include "msgManager.h"

using namespace csg;


bool csg::SHandlerId::operator==( const SHandlerId& other ) const
{
	return !operator!=( other );
}

bool csg::SHandlerId::operator!=( const SHandlerId& other ) const
{
	if ( this == &other )
		return false;
	if ( id != other.id )
		return true;
	if ( type != other.type )
		return true;
	if ( typeEx != other.typeEx )
		return true;
	if ( typeEx2 != other.typeEx2 )
		return true;
	return false;
}

csg::SHandlerId::SHandlerId()
{
	_csg_init();
}

bool csg::SHandlerId::operator<( const SHandlerId& other ) const
{
	//id最后才做比较
	if ( this == &other )
		return false;
	if ( type < other.type )
		return true;
	if ( typeEx < other.typeEx )
		return true;
	if ( typeEx2 < other.typeEx2 )
		return true;
	if ( id < other.id )
		return true;
	return false;
}

// SHandlerId& csg::SHandlerId::operator=( const SHandlerId& other )
// {
// 	id = other.id;
// 	type = other.type;
// 	typeEx = other.typeEx;
// 	typeEx2 = other.typeEx2;
// }

void csg::SHandlerId::_csg_init()
{
	type = 0;
	typeEx = 0;
	typeEx2 = 0;
	id = 0;
}

void csg::SHandlerId::_csg_write(csg::CSerializeStream& os) const
{
	os.write(id);
	os.write(type);
	os.write(typeEx);
	os.write(typeEx2);
}

void csg::SHandlerId::_csg_read(csg::CSerializeStream& os) 
{
	os.read(id);
	os.read(type);
	os.read(typeEx);
	os.read(typeEx2);
}

csg::SMsgHead::SMsgHead()
{
	_csg_init();
}

bool csg::SMsgHead::operator!=( const SMsgHead& other ) const
{
	if ( this == &other )
	{
		return false;
	}
	if ( command != other.command )
	{
		return false;
	}
	if ( fromId != other.fromId )
	{
		return false;
	}
	return true;
}

bool csg::SMsgHead::operator==( const SMsgHead& other ) const
{
	return !operator!=( other );
}

bool csg::SMsgHead::operator<( const SMsgHead& other ) const
{
	if ( this == &other )
	{
		return false;
	}
	if ( command < other.command )
	{
		return true;
	}
	if ( fromId < other.fromId )
	{
		return true;
	}
	return false;
}

void csg::SMsgHead::_csg_init()
{
	command = 0;
	fromId._csg_init();
}

void csg::SMsgHead::_csg_write(csg::CSerializeStream& os) const
{
	os.write(command);
	fromId._csg_write(os);
}

void csg::SMsgHead::_csg_read(csg::CSerializeStream& os)
{
	os.read(command);
	fromId._csg_read(os);
}

csg::CMsgBlock::CMsgBlock()
{
	_msgHead._csg_init();
	if ( _msgBase )
		_msgBase->_csg_init();
}

csg::CMsgBlock::~CMsgBlock()
{

}

void csg::CMsgBlock::_csg_write(csg::CSerializeStream& os) const
{
	os.setUseBitMark(false);
	_csg_writeHead(os);
	os.setUseBitMark(true);
	_csg_writeBody(os);
}

void csg::CMsgBlock::_csg_writeHead(csg::CSerializeStream& os) const
{
	_msgHead._csg_write(os);
}

void csg::CMsgBlock::_csg_writeBody(csg::CSerializeStream& os) const
{
	if ( _msgBase )
	{
		os.write(_msgBase->getType()); //先写type
		_msgBase->_csg_write(os);
	} else
	{
		byte_t type = 0; //空结构
		os.write(type);
	}

}

void csg::CMsgBlock::_csg_read(csg::CSerializeStream& is)
{
	is.setUseBitMark(false);
	_csg_readHead(is);
	is.setUseBitMark(true);
	_csg_readBody(is);
}

void csg::CMsgBlock::_csg_readHead(csg::CSerializeStream& is)
{
	_msgHead._csg_read(is);
}

void csg::CMsgBlock::_csg_readBody(csg::CSerializeStream& is)
{
	int type = 0;
	is.read(type);
	if ( 0 != type )
	{
		_msgBase = CMsgManager::instance()->createMsg(type);
		if ( _msgBase )
		{
			_msgBase->_csg_read(is);
		} else
		{
			throw csg::CException("mq msg type not regist" ,ExceptionCodeMsgTypeNotRegist);
		}
	}
}
