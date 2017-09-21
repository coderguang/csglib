#ifndef _ENGINE_SERIALIZE_SERIALIZE_STREAM_H_
#define _ENGINE_SERIALIZE_SERIALIZE_STREAM_H_

#include "engine/serialize//bytesbuffer.h"
#include <vector>
#include "engine/def/csg_def.h"
#include <string>

#define SERIALIZE_MAX_RESIZE (1024*100)

namespace csg
{
	static const size_t SIZE_OF_BYTE_T = sizeof(byte_t);
	static const size_t SIZE_OF_INT_T = sizeof(int);
	static const size_t SIZE_OF_SHORT_T = sizeof(short);
	static const size_t SIZE_OF_LONG64_T = sizeof(long64_t);
	static const byte_t	FLAG_BIT_INIT = 0x80;// 1000 0000 
	static const unsigned short	SIZE_SHORT_MAX = 0xFFFE;
	static const unsigned short UNSIGNED_SHORT_MAX = 0xFFFF;
	static const unsigned short	SIZE_BYTE_MAX = 0xFE;
	static const unsigned short	UNSIGNED_BYTE_MAX = 0xFF;


	class CSerializeStream :public virtual CBytesBuffer
	{
	public:
		explicit CSerializeStream();
		~CSerializeStream();

		void append(const void* buf ,int length);

		void resize(int size);

		void clear();

		void reset();

	public:

		void setUseBitMark(bool useBitMark);

		bool getUseBitMark();
		//将最后的 flagByte 写入到 _flagBytes,序列化前准备
		void prepareToAppend();
		//将 流中的 _flagBytes 写入到 _flagBytes,反序列化前准备
		void prepareToRead();

		//获取 _flagBytes 的长度
		int getFlagDataSize();

		const char* getFlagData()const;

		/*
		*data operation
		*only int,string,byte,bool,long
		*/
		void writeSize(int);
		void readSize(int&);
		
		void writeSize(uint_t);
		void readSize(uint_t&);


		/*
		* byte data
		*/
		void write(byte_t v);
		void read(byte_t &v);
		void write(const std::vector<byte_t>&);
		void read(std::vector<byte_t>&);

		/**
		* bool data operation
		*/
		void write(bool v);
		void write(const std::vector<bool>&);
		void read(bool& v);
		void read(std::vector<bool>&);

		/**
		* short data operation
		*/
		void write(short);
		void write(const std::vector<short>&);
		void read(short&);
		void read(std::vector<short>&);

		/**
		* int data operation
		*/
		void write(int);
		void write(const std::vector<int>&);
		void read(int&);
		void read(std::vector<int>&);

		/**
		* long64_t data operation
		*/
		void write(long64_t);
		void write(const std::vector<long64_t>&);
		void read(long64_t&);
		void read(std::vector<long64_t>&);

		/**
		* string data operation
		*/
		void write(const std::string&);
		void write(const std::vector<std::string>&);
		void read(std::string&);
		void read(std::vector<std::string>&);

		int getBytesLeft()const;

		void addReadIndex(int len);

		char* getReadPoint()const;

	protected:
		void checkReadLength(int readLength ,const char* name);

	private:
		//bitMark opeation
		void markBitFlag(bool isDefault = false);

		bool readBitFlag();

	private:
		bool _useBitMark;//是否使用bigMark
		bool _isUseFlagBitMark; //flagByte 是否存储了bitMark信息
		CBytesBuffer _flagBytes; //_flagBytes信息 默认值标志位buffer
		byte_t _flagedBit;//bitMark辅助变量 
		byte_t _flagByte;//bitMark辅助变量
		size_t _flagByteReadPos;//_flagBytes 位置
		int _readIndex;//继承自 bytesbuffer 内的 buffer的位置

	private:
		CSerializeStream(const CSerializeStream&);
		CSerializeStream& operator=( const CSerializeStream& );
	};
}

#endif