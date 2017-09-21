#ifndef _ENGINE_MQ_MSG_QUEUE_H_
#define _ENGINE_MQ_MSG_QUEUE_H_

#include "framework/DesignPattern/Singleton.h"
#include "msgBlock.h"
#include "framework/SmartPoint/refObject.h"
#include <list>


namespace csg
{
	class CMsgSendData :public virtual CRefObject
	{
	public:
		int socketfd;
		CMsgBlockPtr msgBlock;
	};
	typedef csg::CSmartPointShare<CMsgSendData> CMsgSendDataPtr;

	typedef std::list<CMsgSendDataPtr> ListMsg;

	class CMsgQueue
	{
	public:
		static CMsgQueue* instance();

		bool pushMessage(int socketfd,const CMsgBlockPtr& mb);

		bool runLoop();

	protected:
		CLock& getWriteLock();

		CLock& getReadLock();
	private:

		CMsgQueue();

		virtual ~CMsgQueue();

		CLock _writeLock;
		CLock _readLock;

		ListMsg _writeList;
		ListMsg _readList;
	};


}
#endif