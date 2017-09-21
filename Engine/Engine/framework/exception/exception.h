#ifndef _FRAMEWORK_EXCEPTION_EXCEPTION_H_
#define _FRAMEWORK_EXCEPTION_EXCEPTION_H_


#include <exception>
#include <stdarg.h>
#include <sstream>


namespace csg
{
	enum ECSGEErrorCode
	{
		ExceptionCodeSystemBase = 10000 ,
		ExceptionCodeRMIBase = 20000 ,
		ExceptionCodeUserBase = 30000
	};

	enum ECSGEErrorCodeSystem
	{
		ExceptionCodeUnknow = 10000 ,
		ExceptionCodeOutOffMemery = 10001 ,
		ExceptionCodeReadOnly = 10002 ,  // searilize exception
		ExceptionCodeDatetime = 10003 ,//datetime exception
		ExceptionCodeMsgTypeNotRegist = 10004, //mq msg type not regist
		ExceptionCodeSerialize = 10005, //serialize error
	};


	class CException
		:public std::exception
	{
	public:

		CException(const char* msg = "CException" ,int code = ExceptionCodeUnknow)
			:_msg(msg) ,_code(code)
		{
		}

		CException(const CException& ex)
			:_msg(ex._msg) ,_code(ex._code)
		{
		}

		CException& operator = ( const CException &ex )
		{
			_msg = ex._msg;
			_code = ex._code;
			return *this;
		}

		virtual ~CException()throw()
		{

		}

		const char* what()const throw( )
		{
			return _msg.c_str();
		}
		int code()const
		{
			return _code;
		}

		void setMessage(const std::string& msg)
		{
			_msg = msg;
		}

		void setCode(int code)
		{
			_code = code;
		}

		void setException(const std::string& msg ,int code)
		{
			_msg = msg;
			_code = code;
		}


	protected:
		std::string _msg;
		int _code;
	};


	class CNullHandleException
		:public CException
	{
	public:
		CNullHandleException(const char* file ,int nline);
		virtual ~CNullHandleException()throw( );
	};

#define CSG_THROW_EXCEPTION(E,STR) \
													{ \
				std::ostringstream stream; \
				stream<<STR; \
				E e(stream.str().c_str()); \
				throw e; \
						}

#define CSG_THROW_CODE_EXCEPTION(E,CODE,STR)\
										{\
				std::ostringstream stream;\
				stream<<STR;\
				E e(stream.str().c_str(),CODE);\
				throw e;\
						}
}
#endif