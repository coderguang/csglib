
#ifndef _ENGINE_SERIALIZE_BYTES_BUFFER_H_
#define _ENGINE_SERIALIZE_BYTES_BUFFER_H_

#ifndef CSG_BYTES_BUFFER_INIT_SIZE
#define CSG_BYTES_BUFFER_INIT_SIZE (1024*2)
#define CSG_BYTES_BUFFER_MAX_SIZE (1024*100)
#endif

#ifndef CSG_INC_BUFFER_SIZE
#define CSG_INC_BUFFER_SIZE 512
#endif

#include "../def/csg_def.h"

namespace csg
{

	class CBytesBuffer
	{
	public:
		CBytesBuffer(const int len = CSG_BYTES_BUFFER_INIT_SIZE);

		CBytesBuffer(const void* buf ,int len);

		virtual ~CBytesBuffer();

		virtual void clear();

		void append(const void* data ,int newDataSize);

		const int getDataSize()const;
		//返回buff的起始位置
		const char* getData()const;

		bool isReadOnly()const;

		void resize(size_t size);

		void popData(int len);
	private:
		
		void checkOverflowAndRefresh(int addLength);

		CBytesBuffer(const CBytesBuffer& other);

		CBytesBuffer& operator= ( const CBytesBuffer& other );

		void setReadOnly(bool readOnly);

		void reset();

		void swap(csg::CBytesBuffer& buf);

		void addDataSize(int addLen);

	private:
		bool _isRealOnly;
		char* _buffer;
		int _buffersSize;
		int _dataSize;

	};
}
#endif