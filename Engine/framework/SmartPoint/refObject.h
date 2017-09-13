
#ifndef _FRAMEWORK_SMART_POINT_REF_OBJECT_H_
#define _FRAMEWORK_SMART_POINT_REF_OBJECT_H_

#include "engine/def/csg_def.h"
#include "../counter/counterHandler.h"
#include <forward_list>

namespace csg
{

	//所有智能指针类对象的基类,主要是为了统一指针以及使用智能指针
	class CRefObject
	{
	public:
		CRefObject();

		CRefObject& operator=( const CRefObject& other );

		virtual ~CRefObject();

		virtual bool decRef();

		virtual void incRef();

		int getRef()const;

		void setNoDelete(bool b);

		bool getNoDelete()const;

	private:
		CRefObject(const CRefObject&)=delete;
		bool _noDelete;
		csg_atomic_int _refCount;
		static CCounterHandler _handlerCount;

	};
}
#endif   