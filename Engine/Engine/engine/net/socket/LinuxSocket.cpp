#include "engine/net/socket/LinuxSocket.h"
#include "framework/log/Log.h"
#include "engine/def/environment_def.h"
#include "framework/util/sysUtil.h"
#include "framework/util/typetransform.h"

#ifdef CSG_LINUX
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <sys/poll.h>
#include <sys/select.h>
#include <unistd.h>


int csg::CLinuxSocket::getSocket(int family ,int type ,int protocol)
{
	int sockfd=::socket(family,type,protocol);
	if(-1==sockfd){
		CSG_RECORD_ERROR("CLinuxSocket::getSocket failed!");
	}
	return sockfd;
}

int csg::CLinuxSocket::Connect(int clientfd ,const struct sockaddr* servaddr ,socklen_t addrlen)
{
	int result=connect(clientfd,servaddr,addrlen);
	if(-1==result){
		CSG_RECORD_ERROR("CLinuxSocket::connect failed!");
	}
	return result;
}

int csg::CLinuxSocket::Bind(int sockfd ,const struct sockaddr* servaddr ,socklen_t addrlen)
{
	int result=::bind(sockfd,servaddr,addrlen);
	if(-1==result){
		CSG_RECORD_ERROR("CLinuxSocket::bind failed!");
	}
	return result;
}

int csg::CLinuxSocket::Listen(int sockfd ,int backlog)
{
	int result=listen(sockfd,backlog);
	if(-1==result){
		CSG_LOG_ERR("CLinuxSocket::listen failed!");
	}
	return result;
}

int csg::CLinuxSocket::Accept(int sockfd ,struct sockaddr* cliaddr ,socklen_t *addrlen)
{
	int result=accept(sockfd,cliaddr,addrlen);
	if(-1==result){
		CSG_LOG_ERR("CLinuxSocket::accept failed!");
	}
	return result;
}

int csg::CLinuxSocket::Select(int maxfd ,fd_set *readset ,fd_set *writeset ,fd_set *exceptset ,struct timeval *timeout)
{
	int result=select(maxfd,readset,writeset,exceptset,timeout);
	if(result>0||0==result){
		return result;
	}else if(-1==result){
		CSG_RECORD_ERROR("select occur error!");
		return CSG_RETURN_ERROR;
	}
	CSG_RECORD_ERROR("select occur unexcepted error!");
	return CSG_RETURN_ERROR;
}

int csg::CLinuxSocket::Poll(struct pollfd *fdarray ,unsigned long nfds ,int timeout)
{
	int temp=poll(fdarray,nfds,timeout);
	if(temp>0||temp==0){
		return temp;
	}else if(-1==temp){
		CSG_RECORD_ERROR("poll occur error!");
		return CSG_RETURN_ERROR;
	}
	CSG_RECORD_ERROR("poll occur unexcepted error!");
	return CSG_RETURN_ERROR;
}

int csg::CLinuxSocket::Epoll_create(int size)
{
	int temp=epoll_create(size);
	if(temp>0||temp==0){
		return temp;
	}else if(-1==temp){
		CSG_RECORD_ERROR("epoll_create error!");
		return CSG_RETURN_ERROR;
	}
	CSG_RECORD_ERROR("epoll_create occur unexcepted error!");
	return CSG_RETURN_ERROR;
}

int csg::CLinuxSocket::Epoll_ctl(int epfd ,int op ,int fd ,struct epoll_event *event)
{
	int temp=epoll_ctl(epfd,op,fd,event);
	if(temp>0||temp==0){
		return temp;
	}else if(-1==temp){
		CSG_RECORD_ERROR("epoll_ctl error");
		return CSG_RETURN_ERROR;
	}
	CSG_RECORD_ERROR("epoll_ctl occur unexcepted error!");
	return CSG_RETURN_ERROR;
}

int csg::CLinuxSocket::Epoll_wait(int epfd ,struct epoll_event *events ,int maxevents ,int timeout)
{
	int temp=epoll_wait(epfd,events,maxevents,timeout);
	if(temp>0||temp==0){
		return temp;
	}else if(-1==temp){
		CSG_RECORD_ERROR("epoll_wait error");
		return CSG_RETURN_ERROR;
	}
	CSG_RECORD_ERROR("epoll_wait occur unexcepted error!errno=");
	return CSG_RETURN_ERROR;
}

int csg::CLinuxSocket::closeSocket(int sockfd)
{
	int temp=close(sockfd);
	if(0==temp){
		return CSG_RETURN_OK;
	}else{
		CSG_RECORD_ERROR("eclose error!");
		return CSG_RETURN_ERROR;
	}
}

ssize_t csg::CLinuxSocket::Read(int sockfd ,void *buf ,size_t len)
{
	int temp=read(sockfd,buf,len);
	if(temp<0){
		if(errno==EINTR){//should call read again
			temp=0;
			return CSG_RETURN_OK;
		}else{
			CSG_RECORD_ERROR("read socket error!");
			return CSG_RETURN_ERROR;
		}
	}else{
		return temp;
	}
}

ssize_t csg::CLinuxSocket::Write(int sockfd ,void *buf ,size_t len)
{
	int temp=write(sockfd,buf,len);
	if(temp<=0){
		if(temp<0&&errno==EINTR){//should call write again
			temp=0;
			return CSG_RETURN_OK;
		}else{
			CSG_RECORD_ERROR("write socket error");
			return CSG_RETURN_ERROR;
		}
	}else{
		return temp;
	}
}

ssize_t csg::CLinuxSocket::Readn(int sockfd ,void *vptr ,size_t len)
{
	size_t nleft;
	ssize_t nread;

	char *ptr;
	ptr=(char*)vptr;

	nleft=len;

	while(nleft>0){
		if((nread=read(sockfd,ptr,nleft))<0){
			if(errno==EINTR){
				nread=0; //should call read again
			}else{
				CSG_RECORD_ERROR("readn sockfd error");
				return CSG_RETURN_ERROR;
			}
		}else if(nread==0){
			break;//EOF
		}
		nleft = nleft - nread;
		ptr = ptr + nread;
	}
	return ( len - nleft );
}

ssize_t csg::CLinuxSocket::Writen(int sockfd ,const void* vptr ,size_t len)
{
	size_t nleft;
	ssize_t nwritten;
	const char *ptr;

	ptr=(char*)vptr;
	nleft=len;
	while(nleft>0){
		if((nwritten=write(sockfd,ptr,nleft))<=0){
			if(nwritten<0&&errno==EINTR){
				nwritten=0;
			}else{
				CSG_RECORD_ERROR("writen sockfd error");
				return CSG_RETURN_ERROR;
			}
		}

		nleft = nleft - nwritten;
		ptr += nwritten;
	}
	return len;
}

ssize_t csg::CLinuxSocket::Recv(int sockfd ,void *buf ,size_t len ,int flags)
{
	int temp=recv(sockfd,buf,len,flags);
	if(temp<0){
		if(errno==EINTR){//should call read again
			temp=0;
			return CSG_RETURN_OK;
		}else{
			CSG_RECORD_ERROR("recv sockfd void* error");
			return CSG_RETURN_ERROR;
		}
	}else{
		return temp;
	}
}

ssize_t csg::CLinuxSocket::Writen(int sockfd ,const char* vptr ,size_t len)
{
	size_t nleft;
	ssize_t nwritten;
	const char *ptr;

	ptr = vptr;
	nleft = len;

	while ( nleft > 0 )
	{
		if ( ( nwritten = write(sockfd ,ptr ,nleft) ) <= 0 )
		{
			if ( nwritten < 0 && errno == EINTR )
			{
				nwritten = 0;
			} else
			{
				CSG_RECORD_ERROR("writen sockfd char* error");
				return CSG_RETURN_ERROR;
			}
		}
		nleft = nleft - nwritten;
		ptr += nwritten;
	}
	return len;
}

#endif
