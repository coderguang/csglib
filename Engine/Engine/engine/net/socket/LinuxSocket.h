
#ifndef _ENGINE_NET_SOCKET_LINUX_SOCKET_H_
#define _ENGINE_NET_SOCKET_LINUX_SOCKET_H_

#include "engine/def/environment_def.h"

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

namespace csg
{

	class CLinuxSocket
	{
	public:
		static int getSocket(int family ,int type ,int protocol);

		static int Connect(int clientfd ,const struct sockaddr* servaddr ,socklen_t addrlen);

		static int Bind(int sockfd ,const struct sockaddr* servaddr ,socklen_t addrlen);

		static int Listen(int sockfd ,int backlog);

		static int Accept(int sockfd ,struct sockaddr* cliaddr ,socklen_t *addrlen);

		static int Select(int maxfd ,fd_set *readset ,fd_set *writeset ,fd_set *exceptset ,struct timeval *timeout);

		static int Poll(struct pollfd *fdarray,unsigned long nfds,int timeout);

		static int Epoll_create(int size);

		static int Epoll_ctl(int epfd ,int op ,int fd ,struct epoll_event *event);

		static int Epoll_wait(int epfd ,struct epoll_event *events ,int maxevents ,int timeout);

		static int closeSocket(int sockfd);

		static ssize_t Read(int sockfd ,void *buf ,size_t len);

		static ssize_t Write(int sockfd ,void *buf ,size_t len);

		static ssize_t Readn(int sockfd ,void *vptr ,size_t len);

		static ssize_t Writen(int sockfd ,const void* vptr ,size_t len);

		static ssize_t Writen(int sockfd ,const char* vptr ,size_t len);

		static ssize_t Recv(int sockfd ,void *buf ,size_t len ,int flags);

	};

}

#endif

#endif
