#include "engine/def/environment_def.h"
#include "client.h"

#ifdef CSG_WIN

#include "engine/net/socket/WinSocket.h"
#include "framework/log/Log.h"
#include "framework/util/typetransform.h"
#include "framework/util/sysUtil.h"


csg::CChatClient::CChatClient(std::string ipAddr ,int port) :_socketfd(-1) ,_ipAddr(ipAddr) ,_port(port) ,_stop(false) ,CRefObject()
{
	init();
}

csg::CChatClient::~CChatClient()
{
	CWinSocket::close(_socketfd);
	CWinSocket::clearWinSocket();
	_stop = false;
}

void csg::CChatClient::startRun()
{
	std::thread sendT(CChatClient::startSend ,this);
	sendT.detach();
	std::thread recvT(CChatClient::startRecv ,this);
	recvT.detach();
}

int csg::CChatClient::doSend()
{
	do
	{
		if ( _stop )
			break;
		//cin.get();
		std::string str = "from client,I'm thread " + ToStr(CThread::threadId()) + ",port="+ToStr(_port);
		int sResult = CWinSocket::send(_socketfd ,str.c_str() ,str.length() ,0);
		CSG_LOG_DEBUG("doSend Thread=" << ToStr(CThread::threadId()) << ",port="<<ToStr(_port)<<",length="<<ToStr(sResult)<<",say" << str);
		CThread::sleep_for(2000);
		if ( -1 == sResult )
		{
			break;
		}
	} while ( true );
	return 0;
}

void csg::CChatClient::startSend(CChatClient* client)
{
	client->doSend();
}

void csg::CChatClient::startRecv(CChatClient* client)
{
	client->doRecvChat();
}

bool csg::CChatClient::init()
{

	CWinSocket::InitSocket();

	_socketfd = CWinSocket::getSocket(AF_INET ,SOCK_STREAM ,IPPROTO_TCP);
	if ( INVALID_SOCKET == _socketfd )
	{
		CSG_LOG_DEBUG("CChatClient::init failed,invalid socket");
		return false;
	}
	struct sockaddr_in servaddr;
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(_port);
	servaddr.sin_addr.S_un.S_addr = inet_addr(_ipAddr.c_str());

	if(SOCKET_ERROR == connect(_socketfd,( struct sockaddr * )&servaddr ,sizeof( servaddr )) )
	{
		
		return false;
	}
	return true;
}

void csg::CChatClient::doRecvChat()
{
	do
	{
		if ( _stop )
			break;
		char buffer[1024];
		int n = ::recv(_socketfd ,buffer ,1024 ,0);
		if ( n > 0 )
		{
			buffer[n - 1] = '\0';
			CSG_LOG_DEBUG("doRecvChat Thread=" << ToStr(CThread::threadId()) << ",say:" << std::string(buffer));
		}
		CThread::sleep_for(100);
	} while ( true );
}

#endif