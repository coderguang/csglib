#ifndef _FRAMEWORK_DESIGN_PATTERN_OBJECT_POOL_H_
#define _FRAMEWORK_DESIGN_PATTERN_OBJECT_POOL_H_

#include "engine/lock/lock.h"
#include "engine/serialize/serializestream.h"
#include <vector>

namespace csg
{
	template<typename T,typename LOCK=CLock>
	class CObjectPool
	{
	public: 
		typedef std::vector<T*> ObjectPool;

		CObjectPool() :_index(0) ,_fina(false)
		{
		}
		virtual ~CObjectPool()
		{

		}

		T* newObject()
		{
			T* t = NULL;
			{
				CAutoLock l(getLock());
				if ( 0 != _index )
				{
					t = _objectList[--_index];
				}
			}
			if ( NULL != t )
				return t;
			return new T;
		}

		void freeObject(T* t)
		{
			if ( _fina )
				return;
			if ( !t )
				return;
			t->reset();
			CAutoLock l(getLock());

			if ( _index == _objectList.size() )
			{
				_objectList.push_back(t);
				_index++;
			} else
			{
				_objectList[_index++] = t;
			}

		}

		void fina()
		{
			for ( size_t i = 0; i < _index; i++ )
			{
				delete _objectList[i];
			}
			_objectList.clear();
			_fina = true;
		}


		static CObjectPool* instance()
		{
			static CObjectPool ins;
			return &ins;
		}

	protected:
		LOCK& getLock()
		{
			return _lock;
		}
	private:
		ObjectPool _objectList;
		LOCK _lock;
		size_t _index;
		bool _fina;
	};


	template<typename T,typename LOCK=CLock>
	class CAutoObjectPool
	{
	public:
		explicit CAutoObjectPool(T *ptr = NULL)
			: _ptr(ptr)
		{
		}
		CAutoObjectPool(CAutoObjectPool<T>& right)
		{
			_ptr = right->get();
		}

		CAutoObjectPool<T>& operator=( CAutoObjectPool<T>& right )
		{
			if ( right->get() != _ptr )
			{
				CObjectPool<T>::instance()->freeObject(_ptr);
			}
			_ptr = right->get();
			return ( *this );
		}

		~CAutoObjectPool()
		{
			CObjectPool<T>::instance()->freeObject(_ptr);
		}
		T& operator*( ) 
		{
			return *_ptr;
		}

		T* operator->( ) 
		{
			return _ptr;
		}

		T* get() 
		{
			return _ptr;
		}
	private:
		T* _ptr;
	};

	typedef CAutoObjectPool<CSerializeStream> CAutoSerializeStream;
	typedef CObjectPool<CSerializeStream> CSerializeStreamPool;

}
#endif