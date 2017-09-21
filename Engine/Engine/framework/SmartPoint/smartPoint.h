#ifndef _FRAMEWORK_SMART_POINT_SMART_POINT_H_
#define _FRAMEWORK_SMART_POINT_SMART_POINT_H_

#include "../counter/counterHandler.h"
#include "../exception/exception.h"
#include "../log/Log.h"

namespace csg
{
	

	template<typename T>
	class CSmartPointShareBase
	{
	public:
		T* get()const
		{
			return _ptr;
		}
		T* operator->( )const
		{
			if ( !_ptr )
			{
				CSG_LOG_INFO("operator->() null ptr");
				throw CNullHandleException(__FILE__ ,__LINE__);
			}
			return _ptr;
		}
		T& operator*( )const
		{
			if ( !_ptr )
			{
				CSG_LOG_INFO("operator->() null ptr");
				throw CNullHandleException(__FILE__ ,__LINE__);
			}
			return *_ptr;
		}
		operator bool()const
		{
			return _ptr ? true : false;
		}
		void swap(CSmartPointShareBase& other)
		{
			std::swap(_ptr ,other._ptr);
		}
		T* _ptr;
	};

	template<typename T,typename U>
	bool operator ==( const CSmartPointShareBase<T>& lsp ,const CSmartPointShareBase<U>& rsp )
	{
		T* l = lsp.get();
		U* r = rsp.get();
		if ( l&&r )
		{
			return *l == *r;
		} else
		{
			return !l&&!r;
		}
	}

	template<typename T ,typename U>
	bool operator !=( const CSmartPointShareBase<T>& lsp ,const CSmartPointShareBase<U>& rsp )
	{
		T* l = lsp.get();
		U* r = rsp.get();
		if ( l&&r )
		{
			return *l != *r;
		} else
		{
			return l||r;
		}
	}

	template<typename T ,typename U>
	bool operator <( const CSmartPointShareBase<T>& lsp ,const CSmartPointShareBase<U>& rsp )
	{
		T* l = lsp.get();
		U* r = rsp.get();
		if ( l&&r )
		{
			return *l < *r;
		} else
		{
			return !l&&r;
		}
	}

	template<typename T ,typename U>
	bool operator >( const CSmartPointShareBase<T>& lsp ,const CSmartPointShareBase<U>& rsp )
	{
		T* l = lsp.get();
		U* r = rsp.get();
		if ( l&&r )
		{
			return *l > *r;
		} else
		{
			return l&&!r;
		}
	}

	template<typename T>
	class CSmartPointShare :public CSmartPointShareBase<T>
	{
	public:
		CSmartPointShare(T* p = NULL)
		{
			this->_ptr = p;
			if ( this->_ptr )
			{
				this->_ptr->incRef();
			}
		}

		template<typename Y>
		CSmartPointShare(const CSmartPointShare<Y>& r)
		{
			this->_ptr = r._ptr;
			if ( this->_ptr )
			{
				this->_ptr->incRef();
			}
		}

		CSmartPointShare(const CSmartPointShare& r)
		{
			this->_ptr = r._ptr;
			if ( this->_ptr )
			{
				this->_ptr->incRef();
			}
		}

		~CSmartPointShare()
		{
			if ( this->_ptr )
			{
				this->_ptr->decRef();
			}
		}

	
		CSmartPointShare& operator= ( T* p )
		{
			if ( this->_ptr != p )
			{
				if ( p )
				{
					p->incRef();
				}
				T* thisPtr = this->_ptr;
				this->_ptr = p;
				if ( thisPtr )
				{
					thisPtr->decRef();
				}
			}
			return *this;
		}
		
		template<typename Y>
		CSmartPointShare& operator=( const CSmartPointShare<Y>& r )
		{
			if ( this->_ptr != r._ptr )
			{
				if ( r._ptr )
				{
					r._ptr->incRef();
				}
				T* thisPtr = this->_ptr;
				this->_ptr = r._ptr;
				if ( thisPtr )
				{
					thisPtr->decRef();
				}
			}
			return *this;
		}

		CSmartPointShare& operator = ( const CSmartPointShare& r )
		{
			if ( this->_ptr != r._ptr )
			{
				if ( r._ptr )
				{
					r._ptr->incRef();
				}
				T* thisPtr = this->_ptr;
				this->_ptr = r._ptr;
				if ( thisPtr )
				{
					thisPtr->decRef();
				}
			}
			return *this;
		}

		template<class Y>
		static CSmartPointShare dynamicCast(const CSmartPointShare<Y>& r)
		{
			return CSmartPointShare(dynamic_cast<T*>( r._ptr ));
		}
		template<class Y>
		static CSmartPointShare dynamicCast(Y* p)
		{
			return CSmartPointShare(dynamic_cast<T*>( p ));
		}
	};
}
#endif