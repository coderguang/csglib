#ifndef _ENGINE_RPC_RMI_OBJECT_H_
#define _ENGINE_RPC_RMI_OBJECT_H_


namespace csg
{
	class CRMIObject :public virtual CRefObject
	{
	public:
		CRMIObject();
		virtual ~CRMIObject();

	public:
		MapRMIInfo _mapRMIInfo;
	};
}

#endif