/**
 * @file	ec_lwip_wrapper.h
 * @brief	Wrap lwip socket api to ECLayer api.
 * @details	You need to import lwip into your project,
 * 			make sure to turnoff lwip compatible socket
 * 			by define LWIP_COMPAT_SOCKETS = 0 in lwip
 * 			option file.
 * @author	Eggcar
*/

/**
 * Copyright EggCar(eggcar@qq.com)
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 * 	http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
*/


#pragma once

#include "ec_config.h"

#if _WITH_LWIP_SOCKET_WRAPPER
#	ifndef _LWIP_SOCKET_HEADER_FILE
#		include "lwip/sockets.h"
#	else
#		include _LWIP_SOCKET_HEADER_FILE
#	endif

int ec_fd2sock(int32_t fd);
int socket(int domain, int type, int protocol);

static inline int accept(int s, struct sockaddr *addr, socklen_t *addrlen)
{
	return lwip_accept(ec_fd2sock(s), addr, addrlen);
}

static inline int bind(int s, const struct sockaddr *name, socklen_t namelen)
{
	return lwip_bind(ec_fd2sock(s), name, namelen);
}

static inline int shutdown(int s, int how)
{
	return lwip_shutdown(ec_fd2sock(s), how);
}

static inline int getpeername(int s, struct sockaddr *name, socklen_t *namelen)
{
	return lwip_getpeername(ec_fd2sock(s), name, namelen);
}

static inline int getsockname(int s, struct sockaddr *name, socklen_t *namelen)
{
	return lwip_getsockname(ec_fd2sock(s), name, namelen);
}

static inline int getsockopt(int s, int level, int optname, void *optval, socklen_t *optlen)
{
	return lwip_getsockopt(ec_fd2sock(s), level, optname, optval, optlen);
}

static inline int setsockopt(int s, int level, int optname, const void *optval, socklen_t optlen)
{
	return lwip_setsockopt(ec_fd2sock(s), level, optname, optval, optlen);
}

static inline int connect(int s, const struct sockaddr *name, socklen_t namelen)
{
	return lwip_connect(ec_fd2sock(s), name, namelen);
}

static inline int listen(int s, int backlog)
{
	return lwip_listen(ec_fd2sock(s), backlog);
}

static inline ssize_t recv(int s, void *mem, size_t len, int flags)
{
	return lwip_recv(ec_fd2sock(s), mem, len, flags);
}

static inline ssize_t readv(int s, const struct iovec *iov, int iovcnt)
{
	return lwip_readv(ec_fd2sock(s), iov, iovcnt);
}

static inline ssize_t recvfrom(int s, void *mem, size_t len, int flags, struct sockaddr *from, socklen_t *fromlen)
{
	return lwip_recvfrom(ec_fd2sock(s), mem, len, flags, from, fromlen);
}

static inline ssize_t recvmsg(int s, struct msghdr *message, int flags)
{
	return lwip_recvmsg(ec_fd2sock(s), message, flags);
}

static inline ssize_t send(int s, const void *dataptr, size_t size, int flags)
{
	return lwip_send(ec_fd2sock(s), dataptr, size, flags);
}

static inline ssize_t sendmsg(int s, const struct msghdr *message, int flags)
{
	return lwip_sendmsg(ec_fd2sock(s), message, flags);
}

static inline ssize_t sendto(int s, const void *dataptr, size_t size, int flags, const struct sockaddr *to, socklen_t tolen)
{
	return lwip_sendto(ec_fd2sock(s), dataptr, size, flags, to, tolen);
}

static inline ssize_t writev(int s, const struct iovec *iov, int iovcnt)
{
	return lwip_writev(ec_fd2sock(s), iov, iovcnt);
}

#	if LWIP_SOCKET_SELECT
// Not support yet.
// int select(int maxfdp1, fd_set *readset, fd_set *writeset, fd_set *exceptset, struct timeval *timeout);
#	endif
#	if LWIP_SOCKET_POLL
// Not support yet.
// int poll(struct pollfd *fds, nfds_t nfds, int timeout);
#	endif
static inline const char *inet_ntop(int af, const void *src, char *dst, socklen_t size)
{
	return lwip_inet_ntop(af, src, dst, size);
}

static inline int inet_pton(int af, const char *src, void *dst)
{
	return lwip_inet_pton(af, src, dst);
}

#endif
