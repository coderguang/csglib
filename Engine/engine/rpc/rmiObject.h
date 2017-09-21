#ifndef _ENGINE_RPC_RMI_OBJECT_H_
#define _ENGINE_RPC_RMI_OBJECT_H_

#include "framework/SmartPoint/refObject.h"
#include <map>
#include "rmidef.h"
#include <set>
#include "framework/exception/exception.h"
#include "framework/SmartPoint/smartPoint.h"
#include "../net/session/session.h"

namespace csg
{

	class CSession;
	typedef CSmartPointShare<CSession> CSessionPtr;

	typedef std::map<int ,SRMIInfo> MapRMIInfo;
	typedef std::set<SRMIInfo> SetRMIInfo;
	typedef std::set<int> SetRpcId;

	//服务器被调用接口基类
	class CRMIObject :public virtual CRefObject
	{
	public:
		CRMIObject();
		virtual ~CRMIObject();

		// to call the local method from client,run in server
		virtual ERMIDispatchResult __onCall(const CSessionPtr& session,const SRMICall& rmiCall ,CSerializeStream& inStream);

		virtual void __response(CSerializeStream& stream);

		virtual void __responseException(const CException& ex);

		virtual void __responseException(const std::exception& ex);

		virtual void __responseException();

		virtual const MapRMIInfo& getRMIInfos()const ;

		const SetRpcId& getRpcIds()const;


	protected:

		bool addRMIInfo(int rpcId ,SRMIInfo info);

		bool isHaveRMIInfo(SRMIInfo info);

		bool isHaveRMIInfo(int rpcId);
		
		bool addRpcId(int rpcId);
	private:
		MapRMIInfo _mapRMIInfo;
		SetRMIInfo _checkRMIInfo;
		SetRpcId _rpcIdset;
	private:
		static CCounterHandler _count;
	};

	typedef CSmartPointShare<CRMIObject> CRMIObjectPtr;

	//客户端调用接口基类
	class CRMIProxyObject :public virtual CRefObject
	{
	public:
		CRMIProxyObject();

		virtual ~CRMIProxyObject();


		virtual void setSession(const CSessionPtr& session ,const SRMICall& rmiCall);

		CSessionPtr& getSession();

		int getMessageId()const;

	private:
		CSessionPtr _session;
		SRMICall _rmiCall;

	private:
		static CCounterHandler _count;

	};

	typedef CSmartPointShare<CRMIProxyObject> CRMIProxyObjectPtr;

	//服务器回调对象基类(服务器用于返回值到客户端)
	class CRMIObjectCallBackObject :public virtual CRefObject
	{
	public:
		virtual void setSession(const CSessionPtr& session ,const SRMICall& rmiCall);

	protected:
		CSessionPtr _session;
		SRMICall _rmiCall;
	};
	typedef CSmartPointShare<CRMIObjectCallBackObject> CRMIObjectCallBackObjectPtr;

	//客户端回调对象基类(客户端用)
	class CRMIProxyCallBackObject :public virtual CRefObject
	{
	public:
		CRMIProxyCallBackObject();

		virtual ~CRMIProxyCallBackObject();

		virtual void __response(CSerializeStream& __os) = 0;

	private:
		static CCounterHandler _count;

	};

	typedef CSmartPointShare<CRMIProxyCallBackObject> CRMIProxyCallBackObjectPtr;

	//客户端用于绑定回调的对象（客户端用）
	class CRMIObjectBind :public virtual CRefObject
	{
	public:

		bool bindProxyCallBackObject(const CRMIProxyCallBackObjectPtr& object);

		CRMIProxyCallBackObjectPtr _callBack;
	private:
		static CCounterHandler _count;

	};

	typedef CSmartPointShare<CRMIObjectBind> CRMIObjectBindPtr;

	typedef std::map<int ,CRMIObjectBindPtr> MapRMIObjectBind; 
}

#endif