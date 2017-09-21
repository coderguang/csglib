#include "engine/serialize/bytesbuffer.h"
#include <malloc.h>
#include "engine/def/csg_def.h"
#include "framework/exception/exception.h"
#include "engine/allocator/allocator.h"
#include <string.h>

using namespace csg;

CBytesBuffer::CBytesBuffer(const int len /* = CSG_BYTES_BUFFER_INIT_SIZE */) 
	:_isRealOnly(false) ,_buffersSize(0) ,_dataSize(0) ,_buffer(NULL)
{
	if ( 0 != len )
	{
		_buffer = (char*)CAllocator::instance()->malloc(len);
		if ( NULL != _buffer )
		{
			_buffersSize = len;
		}
	}

}

CBytesBuffer::CBytesBuffer(const void* buf ,int len)
{
	_isRealOnly = true;
	_buffer = (char*) buf;
	_buffersSize = len;
	_dataSize = len;
}

CBytesBuffer::~CBytesBuffer()
{
	if ( !_isRealOnly&&NULL != _buffer )
	{
		CAllocator::instance()->free(_buffer);
	}
	_buffer = NULL;
}

void CBytesBuffer::checkOverflowAndRefresh(int addLength)
{
	if ( _isRealOnly )
	{
		throw csg::CException("ExceptionCodeReadOnly" ,csg::ExceptionCodeReadOnly);
	}
	addLength += _dataSize;//频繁调用的底层内，应尽量避免创建新变量
	if ( _buffersSize > addLength )
	{
		return; // memory is enough
	}
	int incsize = _buffersSize;
	if ( 0 == incsize )
	{
		incsize = CSG_BYTES_BUFFER_INIT_SIZE;
	}
	while ( incsize < addLength )
	{
		incsize += ( incsize >> 1 );
	}
	void* newPtmp = NULL;
	if ( 0 != _buffersSize )
	{
		newPtmp = CAllocator::instance()->realloc(_buffer ,incsize);
	} else
	{
		newPtmp = CAllocator::instance()->malloc(incsize);
	}

	if ( NULL == newPtmp )
	{
		throw csg::CException("CExceptionCodeOutOffMemery" ,csg::ExceptionCodeOutOffMemery);
	}
	_buffer = (char*) newPtmp;
	_buffersSize = incsize;
}

const int CBytesBuffer::getDataSize()const
{
	return _dataSize;
}

const char* CBytesBuffer::getData()const
{
	return _buffer;
}

void CBytesBuffer::clear()
{
	if ( _isRealOnly )
	{
		throw csg::CException("ExceptionCodeReadOnly" ,csg::ExceptionCodeReadOnly);
	}
	_dataSize = 0;
	if ( _buffersSize > CSG_BYTES_BUFFER_MAX_SIZE )
	{
		_buffersSize = 0;
		CAllocator::instance()->free(_buffer);
		_buffer = NULL;
	}

}

void CBytesBuffer::reset()
{
	clear();
}

bool CBytesBuffer::isReadOnly()const
{
	return _isRealOnly;
}

void CBytesBuffer::popData(int len)
{
	if ( _isRealOnly )
	{
		throw csg::CException("ExceptionCodeReadOnly" ,csg::ExceptionCodeReadOnly);
	}

	if ( len <= 0 )
	{
		return;
	}
	if ( len >= _dataSize )
	{
		clear();
		return;
	}
	_dataSize -= len;
	memmove(_buffer ,_buffer + len ,_dataSize);
}

void CBytesBuffer::append(const void* data ,int newDataSize)
{
	checkOverflowAndRefresh(newDataSize);
	memcpy(_buffer + _dataSize ,data ,newDataSize);//往后添加时,其实位置为
	_dataSize += newDataSize;
}

void CBytesBuffer::resize(size_t size)
{
	if ( _isRealOnly )
	{
		throw csg::CException("ExceptionCodeReadOnly" ,csg::ExceptionCodeReadOnly);
	}
	if ( size < (size_t) _buffersSize )
	{
		_dataSize = (int) size;
		return;
	}
	checkOverflowAndRefresh((int) size - _dataSize);//realloc memory
	_dataSize = (int) size;
}

void CBytesBuffer::swap(CBytesBuffer& buff)
{
	if ( _isRealOnly || buff._isRealOnly )
	{
		throw csg::CException("ExceptionCodeReadOnly" ,csg::ExceptionCodeReadOnly);
	}
	std::swap(_buffer ,buff._buffer);
	std::swap(_buffersSize ,buff._buffersSize);
	std::swap(_dataSize ,buff._dataSize);
}

void CBytesBuffer::addDataSize(int addLen)
{
	if ( _isRealOnly )
	{
		assert(false);
		return;
	}
	if ( addLen < 0 )
	{
		assert(false);
		return;
	}


	checkOverflowAndRefresh(addLen);
	_dataSize += addLen;
}