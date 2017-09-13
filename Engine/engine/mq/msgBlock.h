#ifndef _ENGINE_MQ_MSG_BLOCK_H_
#define _ENGINE_MQ_MSG_BLOCK_H_

#include "framework/SmartPoint/refObject.h"
#include "../serialize/serializestream.h"
#include "framework/SmartPoint/smartPoint.h"

namespace csg
{

	//用于区分各个实体
	class SHandlerId
	{
	public:
		int id;
		int type;
		int typeEx;
		int typeEx2;

		//SHandlerId& operator=( const SHandlerId& other ); //为了避免一些可能存在的问题，这里不重载=
	public:
		SHandlerId();
	public:
		bool operator==( const SHandlerId& other )const;
		bool operator!=( const SHandlerId& other )const;
		bool operator<( const SHandlerId& other )const;

		void _csg_init();
		void _csg_write(csg::CSerializeStream&)const;
		void _csg_read(csg::CSerializeStream&);
	};


	//消息头定义
	class SMsgHead
	{
	public:
		int command;
		SHandlerId fromId;
	public:
		SMsgHead();
	public:
		bool operator==( const SMsgHead& other )const;
		bool operator!=( const SMsgHead& other )const;
		bool operator<( const SMsgHead& other )const;

		void _csg_init();
		void _csg_write(csg::CSerializeStream& os)const;
		void _csg_read(csg::CSerializeStream& os);

	};

	// 所有需要序列化的类继承此类
	class IMsgBase:public virtual CRefObject
	{
	public:
		virtual void _csg_write(csg::CSerializeStream&)const = 0;
		virtual void _csg_read(csg::CSerializeStream&) = 0;
		virtual void _csg_init()
		{
		};
		virtual IMsgBase* clone() = 0;
		IMsgBase& operator = ( const IMsgBase& msg );
		virtual int getType()const = 0;

		virtual void print()
		{
		};
	};

	typedef csg::CSmartPointShare<IMsgBase> IMsgBasePtr;

	//消息体（头+内容）
	class CMsgBlock :public virtual CRefObject
	{
	public:
		SMsgHead _msgHead; //消息头
		IMsgBasePtr _msgBase; //结构体指针
	public:
		CMsgBlock();
		virtual ~CMsgBlock();

		virtual void _csg_write(csg::CSerializeStream& os)const;
		virtual void _csg_writeHead(csg::CSerializeStream& os)const;
		virtual void _csg_writeBody(csg::CSerializeStream& os)const;

		virtual void _csg_read(csg::CSerializeStream& is);
		virtual void _csg_readHead(csg::CSerializeStream& is);
		virtual void _csg_readBody(csg::CSerializeStream& is);
	private:
		CMsgBlock& operator=( const CMsgBlock& ) = delete; //为了尽量降低内存消耗
		CMsgBlock(const CMsgBlock&) = delete;
	};

	typedef csg::CSmartPointShare<CMsgBlock> CMsgBlockPtr;

}

#endif