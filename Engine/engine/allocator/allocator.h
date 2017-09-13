
#ifndef _ENGINE_ALLOCATOR_ALLOCATOR_H_
#define _ENGINE_ALLOCATOR_ALLOCATOR_H_

//为了方面后续管理内存,这里还是写一个内存分配器管理

#include "engine/def/csg_def.h"
 
namespace csg
{
	class CAllocator
	{
	public:
		virtual void *malloc(size_t size);

		virtual void *realloc(void *tmp ,size_t newSize);

		virtual void free(void *buf);

		virtual void gc();

		static CAllocator* instance();

		static CAllocator* instance(CAllocator* alloc);
	private:
		static CAllocator *_alloc;
		//static int _count;
	};
}
#endif