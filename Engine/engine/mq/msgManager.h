#ifndef _ENGINE_MQ_MSG_MANAGER_H_
#define _ENGINE_MQ_MSG_MANAGER_H_

#include "framework/DesignPattern/Singleton.h"
#include "msgBlock.h"
#include "../lock/lock.h"
#include <map>

namespace csg
{
	typedef std::map<int ,IMsgBasePtr> MapMsgBase;

	class CMsgManager :public virtual csg::Singleton<CMsgManager>
	{
	public:
		bool regist(const IMsgBasePtr& msgBase);

		bool unRegist(int type);

		IMsgBasePtr getMsg(int type);

		IMsgBasePtr createMsg(int type);

		CLock& getLock();
	private:
		CLock _lock;
		MapMsgBase _msgMap;
	};
}
#endif