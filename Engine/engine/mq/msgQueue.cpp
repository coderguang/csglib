#include "engine/mq/msgQueue.h"
#include "../net/session/session.h"
#include "../net/session/sessionManager.h"


using namespace csg;

CMsgQueue* csg::CMsgQueue::instance()
{
	static CMsgQueue ins;
	return &ins;
}

bool csg::CMsgQueue::pushMessage(int socketfd ,const CMsgBlockPtr& mb)
{
	CMsgSendDataPtr sendMb = new CMsgSendData();
	sendMb->socketfd = socketfd;
	sendMb->msgBlock = mb;

	CAutoLock l(getWriteLock());
	_writeList.push_back(sendMb);
	return true;
}

bool csg::CMsgQueue::runLoop()
{
	{
		CAutoLock l(getWriteLock());
		std::swap(_writeList ,_readList);
	}
	CAutoLock l(getReadLock());
	for ( ListMsg::iterator it = _readList.begin(); it != _readList.end(); ++it )
	{
		CSessionPtr session = CSessionManager::instance()->getSession(( *it )->socketfd);
		if ( session )
		{
			session->pushMessage((*it)->msgBlock);
		}
	}
	_readList.clear();
	return true;
}

CLock& csg::CMsgQueue::getWriteLock()
{
	return _writeLock;
}

CLock& csg::CMsgQueue::getReadLock()
{
	return _readLock;
}

csg::CMsgQueue::CMsgQueue()
{

}

csg::CMsgQueue::~CMsgQueue()
{

}

