#include "engine/serialize/serializestream.h"
#include "framework/exception/exception.h"
#include "framework/util/endian.h"
#include "../def/csg_def.h"
#include <string.h>

using namespace csg;

CSerializeStream::CSerializeStream()
	:_readIndex(0) ,_flagByte(0) ,_useBitMark(false),
	_flagByteReadPos(0) ,_flagedBit(FLAG_BIT_INIT) ,_isUseFlagBitMark(false)
{
}
CSerializeStream::~CSerializeStream()
{
	if ( !isReadOnly() )
	{
		clear();
	}
}

void CSerializeStream::clear()
{
	_flagBytes.clear();
	_readIndex = 0;
	_flagedBit = FLAG_BIT_INIT;
	_useBitMark = false;
	_flagByte = 0;
	_flagByteReadPos = 0;
	CBytesBuffer::clear();
}

void CSerializeStream::append(const void* buf ,int length)
{
	CBytesBuffer::append(reinterpret_cast<const char*>(buf) ,length);
}

void CSerializeStream::resize(int size)
{
	CBytesBuffer::resize(size);
}

void CSerializeStream::reset()
{
	clear();
}

void CSerializeStream::markBitFlag(bool isDefault)
{
	if ( !_useBitMark )
	{
		return;
	}
	if ( isDefault )
	{
		_flagByte |= _flagedBit; //在_flagByte中标记该字段位默认值 _flagedBit 中的1,即代表本字段所处的顺序,因为是从 10000000 开始的,每次写进后,会先 >>=1 (右移1位)
		if ( !_isUseFlagBitMark )
		{
			_isUseFlagBitMark = true;
		}
	}
	_flagedBit >>= 1;  //变化过程  10000000->01000000->00100000....->00000000
	if ( 0 == _flagedBit )//每8位 将本次8位的结果_flagByte 写进 _flagBytes 内
	{
		_flagBytes.append(&_flagByte ,SIZE_OF_BYTE_T);
		_flagedBit = FLAG_BIT_INIT;
		_flagByte = 0;
	}
}

bool CSerializeStream::readBitFlag()
{
	if ( !_useBitMark || _flagBytes.getDataSize() <= _flagByteReadPos )
	{
		return false;
	}
	bool result = _flagByte&_flagedBit; //read时,判断 & 运算结果,为true时代表该位为默认值
	_flagedBit >>= 1;
	if ( 0 == _flagedBit )
	{
		_flagedBit = FLAG_BIT_INIT;
		_flagByteReadPos++;//读取指针往后移动
		if ( _flagByteReadPos < _flagBytes.getDataSize() )
		{
			memcpy(&_flagByte ,_flagBytes.getData() + _flagByteReadPos ,SIZE_OF_BYTE_T);//这里是如何保证 _flagBytes 还有相应的长度的
		}
	}
	return result;
}

void CSerializeStream::checkReadLength(int readLength ,const char* name)
{
	int left = getBytesLeft();
	if ( getBytesLeft() < readLength )
	{
		CSG_THROW_EXCEPTION(CException ,"CSerializeStream::checkReadLength " << name << " " << __FILE__ << ":" << __LINE__);
	}
}

int CSerializeStream::getBytesLeft()const
{
	return CBytesBuffer::getDataSize() - _readIndex;
}


void CSerializeStream::addReadIndex(int len)
{
	_readIndex += len;
}

char* CSerializeStream::getReadPoint()const
{
	return const_cast<char*>( CBytesBuffer::getData() ) + _readIndex;
}


void CSerializeStream::setUseBitMark(bool useBitMark)
{
	_useBitMark = useBitMark;
}

bool CSerializeStream::getUseBitMark()
{
	return _useBitMark;
}

void CSerializeStream::prepareToAppend()
{
	if ( _isUseFlagBitMark )
	{
		if ( 0 != _flagByte )
		{
			_flagBytes.append(&_flagByte ,SIZE_OF_BYTE_T);
			_isUseFlagBitMark = false;
			_flagByte = 0;
		}
	}
	_flagedBit = FLAG_BIT_INIT;
}

void CSerializeStream::prepareToRead()
{
	int size = 0;
	readSize(size);
	checkReadLength(size*SIZE_OF_BYTE_T ,"CSerializeStream::prepareToRead()");
	if ( size > 0 )
	{
		_flagBytes.append(getReadPoint() ,size*SIZE_OF_BYTE_T);
		addReadIndex(size*SIZE_OF_BYTE_T);
		memcpy(&_flagByte ,_flagBytes.getData() ,SIZE_OF_BYTE_T);
	}
	_flagedBit = FLAG_BIT_INIT;
	_flagByteReadPos = 0;
}


int CSerializeStream::getFlagDataSize()
{
	return _flagBytes.getDataSize();
}

const char* CSerializeStream::getFlagData()const
{
	return _flagBytes.getData();
}

void CSerializeStream::writeSize(int size)
{
	writeSize( uint_t(size) );
}

void CSerializeStream::writeSize(uint_t size)
{
	if ( 0 == size&&_useBitMark )
	{
		markBitFlag(true);
	} else
	{
		markBitFlag(false);
		if ( size < 0 )
		{
			CSG_THROW_EXCEPTION(CException ,"CSerializeStream::writeSize,size=" << size << " less than 0");
		}
		bool originalUseBitMark = _useBitMark;
		_useBitMark = false;//读写长度时,不使用bitMark
		if ( size>SIZE_BYTE_MAX )
		{
			write((byte_t&) UNSIGNED_BYTE_MAX); //如果长度超过 254,先写入一个 标志值,后面的才是具体长度
			write((int&) size);
		} else
		{
			write(static_cast<byte_t>( size ));
		}
		_useBitMark = originalUseBitMark;
	}
}

void CSerializeStream::readSize(int& size)
{
	readSize((uint_t&) size);
}

void CSerializeStream::readSize(uint_t& size)
{
	if ( _useBitMark&&readBitFlag() )
	{
		size = 0;
	} else
	{
		bool originalUseBitMark = _useBitMark;
		_useBitMark = false;//读写长度时,不使用bitMark
		byte_t byteSize=0;
		read(byteSize);
		if ( UNSIGNED_BYTE_MAX == byteSize ) //取第一个值,如果是标志值,后面的int为具体长度
		{
			read((int&) size);;
			if ( size < 0 )
			{
				CSG_THROW_EXCEPTION(CException ,"CSerializeStream::readSize(uint_t& size");
			}
		} else
		{
			size = static_cast<int>( byteSize );
		}
		_useBitMark = originalUseBitMark;
	}

}

void CSerializeStream::write(byte_t bt)
{
	if ( 0 == bt&&_useBitMark )
	{
		markBitFlag(true);
	} else
	{
		markBitFlag(false);
		int size = CBytesBuffer::getDataSize();
		resize(size + SIZE_OF_BYTE_T);
		byte_t* dest = reinterpret_cast<byte_t*>( const_cast<char*>( CBytesBuffer::getData() + size ) );
		*dest = bt;
	}
}

void CSerializeStream::write(const std::vector<byte_t>& vecBt)
{
	int size = (int)vecBt.size();
	writeSize(size);
	if ( size > 0 )
	{
		size_t pos = CBytesBuffer::getDataSize();
		resize(pos + size*SIZE_OF_BYTE_T);
		//byte 类型，只需要将内存辅助过去即可
		memcpy(const_cast<char*>( CBytesBuffer::getData()) + pos ,&vecBt[0] ,size*SIZE_OF_BYTE_T );
	}
}

void CSerializeStream::read(byte_t &v)
{
	if ( _useBitMark&&readBitFlag() )
	{
		v = 0;
	} else
	{
		checkReadLength(SIZE_OF_BYTE_T ,"CSerializeStream::read(byte_t &v)");
		byte_t* dst = reinterpret_cast<byte_t*>( getReadPoint() );
		v = *dst;
		addReadIndex(SIZE_OF_BYTE_T);
	}
}

void CSerializeStream::read(std::vector<byte_t>& vecBt)
{
	uint_t size;
	readSize(size);
	if ( size > 0 )
	{
		checkReadLength(size*SIZE_OF_BYTE_T ,"CSerializeStream::read(std::vector<byte_t>& vecBt)");
		byte_t* begin = reinterpret_cast<byte_t*>( getReadPoint() );
		vecBt.resize(size);
		memcpy(&vecBt[0] ,begin ,size*SIZE_OF_BYTE_T);
		addReadIndex(size*SIZE_OF_BYTE_T);
	} else
	{
		vecBt.clear();
	}
}

void CSerializeStream::write(bool bv)
{
	write(static_cast<byte_t>( bv ));
}

void CSerializeStream::write(const std::vector<bool>& vecBool)
{
	std::vector<byte_t> vecByte;
	vecByte.resize(vecBool.size());
	for ( size_t i = 0; i < vecBool.size(); i++ )
	{
		vecByte[i] = static_cast<byte_t>( vecBool[i] );
	}
	write(vecByte);
}

void CSerializeStream::read(bool& v)
{
	byte_t b;
	read(b);
	v = b == 0 ? false : true;
}

void CSerializeStream::read(std::vector<bool> &vecBool)
{
	std::vector<byte_t> vecByte;
	read(vecByte);
	vecBool.resize(vecByte.size());
	for ( int i = 0; i < vecByte.size(); ++i )
	{
		vecBool[i] = vecByte[i] == 0 ? false : true;
	}
}

void CSerializeStream::write(short sv)
{
	if ( _useBitMark && 0 == sv )
	{
		markBitFlag(true);
	} else
	{
		markBitFlag(false);
		int size = CBytesBuffer::getDataSize();
		resize(size + SIZE_OF_SHORT_T);
		sv = endian(sv);
		byte_t* dst = reinterpret_cast<byte_t*>( const_cast<char*>( CBytesBuffer::getData() + size ) );
		memcpy(dst ,&sv ,SIZE_OF_SHORT_T);
	}
}

void CSerializeStream::write(const std::vector<short>& vecShort)
{
	int size = vecShort.size();
	writeSize(size);
	if ( size > 0 )
	{
		size_t dataSize = CBytesBuffer::getDataSize();
		resize(size*SIZE_OF_SHORT_T + dataSize);
		char* dst = const_cast<char*>( CBytesBuffer::getData() ) + dataSize;
		for ( int i = 0; i < size; i++ )
		{
			short sv = endian(vecShort[i]);
			memcpy(dst ,&sv ,SIZE_OF_SHORT_T);
			dst += SIZE_OF_SHORT_T;
		}
	}
}

void CSerializeStream::read(short& sv)
{
	if ( _useBitMark&&readBitFlag() )
	{
		sv = 0;
	} else
	{
		checkReadLength(SIZE_OF_SHORT_T ,"CSerializeStream::read(short& sv)");
		byte_t* dst = reinterpret_cast<byte_t*>( getReadPoint() );
		memcpy(&sv ,dst ,SIZE_OF_SHORT_T);
		sv = endian(sv);
		addReadIndex(SIZE_OF_SHORT_T);
	}
}

void CSerializeStream::read(std::vector<short>& vecShort)
{
	uint_t size;
	readSize(size);
	if ( size > 0 )
	{
		checkReadLength(size*SIZE_OF_SHORT_T ,"CSerializeStream::read(std::vector<short>& vecShort)");
		byte_t *begin = reinterpret_cast<byte_t*>( getReadPoint() );
		vecShort.resize(size);
		memcpy(&vecShort[0] ,begin ,size*SIZE_OF_SHORT_T);
		for ( int i = 0; i < size; i++ )
		{
			vecShort[i] = endian(vecShort[i]);
		}
		addReadIndex(size*SIZE_OF_SHORT_T);
	} else
	{
		vecShort.clear();
	}
}



void CSerializeStream::write(int iv)
{
	if ( _useBitMark&&0==iv)
	{
		markBitFlag(true);
	} else
	{
		markBitFlag(false);
		int size = CBytesBuffer::getDataSize();
		resize(size + SIZE_OF_INT_T);
		byte_t* dst = reinterpret_cast<byte_t*>( const_cast<char*>( CBytesBuffer::getData()) + size);
		iv = endian(iv);
		memcpy(dst ,&iv ,SIZE_OF_INT_T);
	}
}

void CSerializeStream::write(const std::vector<int>& vecInt)
{
	int size = (int) vecInt.size();
	writeSize(size);
	if ( size > 0)
	{
		//这里不循环调用 write(int iv) 是为了避免底层函数频繁调用，例如resize
		size_t dataSize = CBytesBuffer::getDataSize();
		resize(dataSize + size*SIZE_OF_INT_T);
		char *p = const_cast<char*>( CBytesBuffer::getData() ) + dataSize;
		for ( int i = 0; i < size; i++ )
		{
			int iv = endian(vecInt[i]);
			memcpy(p ,&iv ,SIZE_OF_INT_T);
			p += SIZE_OF_INT_T;
		}
	}

}


void CSerializeStream::read(int& iv)
{
	if ( _useBitMark&&readBitFlag() )
	{
		iv = 0;
	} else
	{
		checkReadLength(SIZE_OF_INT_T ,"CSerializeStream::read(int& iv)");
		byte_t* dst = reinterpret_cast<byte_t*>( getReadPoint() );
		memcpy(&iv ,dst ,SIZE_OF_INT_T);
		iv = endian(iv);
		addReadIndex(SIZE_OF_INT_T);
	}
}

void CSerializeStream::read(std::vector<int>& vecInt)
{
	uint_t size;
	readSize(size);
	if ( size > 0 )
	{
		checkReadLength(size*SIZE_OF_INT_T ,"void CSerializeStream::read(std::vector<int>& vecInt)");
		vecInt.resize(size);
		byte_t* begin = reinterpret_cast<byte_t*>( getReadPoint() );
		memcpy(&vecInt[0] ,begin ,size*SIZE_OF_INT_T);
		for ( int i = 0; i < size; i++ )
		{
			vecInt[i] = endian(vecInt[i]);
		}
		addReadIndex(size*SIZE_OF_INT_T);
	} else
	{
		vecInt.clear();
	}
}

void CSerializeStream::write(long64_t lv)
{
	if ( _useBitMark && 0 == lv )
	{
		markBitFlag(true);
	} else
	{
		markBitFlag(false);
		int dataSize = CBytesBuffer::getDataSize();
		resize(dataSize + SIZE_OF_LONG64_T);
		byte_t* dst = reinterpret_cast<byte_t*>( const_cast<char*>( CBytesBuffer::getData() ) + dataSize );
		lv = endian(lv);
		memcpy(dst ,&lv ,SIZE_OF_LONG64_T);
	}
}

void CSerializeStream::write(const std::vector<long64_t> &veclong )
{
	uint_t size = veclong.size();
	writeSize(size);
	if ( size > 0 )
	{
		size_t dataSize = CBytesBuffer::getDataSize();
		resize(dataSize + SIZE_OF_LONG64_T*size);
		char *p = const_cast<char*>( CBytesBuffer::getData() + dataSize );
		for ( int i = 0; i < veclong.size(); i++ )
		{
			long64_t lv = endian(veclong[i]);
			memcpy(p ,&lv ,SIZE_OF_LONG64_T);
			p += SIZE_OF_LONG64_T;
		}
	}
}

void CSerializeStream::read(long64_t& ll)
{
	if ( _useBitMark && readBitFlag() )
	{
		ll = 0;
	} else
	{
		checkReadLength(SIZE_OF_LONG64_T ,"CSerializeStream::read(long64_t& ll)");
		byte_t* dest = reinterpret_cast<byte_t*>( getReadPoint() );
		memcpy(&ll ,getReadPoint() ,SIZE_OF_LONG64_T);
		ll = endian(ll);
		addReadIndex(SIZE_OF_LONG64_T);
	}
}
void CSerializeStream::read(std::vector<long64_t>& llV)
{
	uint_t size;
	readSize(size);
	if ( size > 0 )
	{
		checkReadLength(size * SIZE_OF_LONG64_T ,"CSerializeStream::read(std::vector<long64_t>& llV)");
		byte_t* begin = reinterpret_cast<byte_t*>( getReadPoint() );
		addReadIndex(size * SIZE_OF_LONG64_T);
		llV.resize(size);
		memcpy(&llV[0] ,begin ,size * SIZE_OF_LONG64_T);
		for ( size_t i = 0; i != llV.size(); i++ )
		{
			llV[i] = endian(llV[i]);
		}
	} else
	{
		llV.clear();
	}
}


/**
* 读取写入字符串行型 8字节
*/
void CSerializeStream::write(const std::string& str)
{
	size_t size = str.length();
	writeSize((int) size);
	if ( size > 0 )
	{
		size_t pos = CBytesBuffer::getDataSize();
		resize((int) pos + (int) str.length());
		memcpy(const_cast<char*>( CBytesBuffer::getData() ) + pos ,str.c_str() ,size);
	}
}
void CSerializeStream::write(const std::vector<std::string>& strV)
{
	size_t size = strV.size();
	writeSize((int) size);
	if ( size > 0 )
	{
		bool originalUseBitMark = _useBitMark;
		_useBitMark = false;
		std::vector<std::string>::const_iterator iter;
		for ( iter = strV.begin(); iter != strV.end(); iter++ )
		{
			write(*iter);
		}
		_useBitMark = originalUseBitMark;
	}
}
void CSerializeStream::read(std::string& str)
{
	uint_t size;
	readSize(size);
	if ( size > 0 )
	{
		checkReadLength(size ,"CSerializeStream::read(std::string& str)");
		char* begin = reinterpret_cast<char*>( getReadPoint() );
		addReadIndex(size);
		str = "";
		str.append(begin ,size);
	} else
	{
		str = "";
	}
}
void CSerializeStream::read(std::vector<std::string>& strV)
{
	uint_t size;
	readSize(size);
	if ( size > 0 )
	{
		bool originalUseBitMark = _useBitMark;
		_useBitMark = false;
		strV.clear();
		strV.resize(size);
		for ( size_t i = 0; i < (size_t) size; i++ )
		{
			read(strV[i]);
		}
		_useBitMark = originalUseBitMark;
	} else
	{
		strV.clear();
	}
}







