/**
 * @file	ec_lwip_wrapper.c
 * @brief	Wrap lwip socket api to ECLayer api.
 * @details	You need to import lwip into your project,
 * 			make sure to turnoff lwip compatible socket
 * 			by define LWIP_COMPAT_SOCKETS = 0 in lwip
 * 			option file, and turn on LWIP_SOCKET = 1
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


#include "ec_config.h"

#if _WITH_LWIP_SOCKET_WRAPPER

#	ifndef _LWIP_SOCKET_HEADER_FILE
#		include "lwip/sockets.h"
#	else
#		include _LWIP_SOCKET_HEADER_FILE
#	endif

#	ifndef _LWIP_OPT_FILE
#		include "lwip/opt.h"
#	else
#		include _LWIP_OPT_FILE
#	endif

#	include "exceptions.h"
#	include "ec_fdlist.h"
#	include "ec_file.h"
#	include "string.h"
#	include "heap_port.h"

int ec_fd2sock(int32_t fd)
{
	file_des_t *fd_st;
	fd_st = get_fd_struct(fd);
	if (fd_st == NULL) {
		return -EBADF;
	}
	else if (fd_st->file_type != e_FTYPE_SOCKET) {
		return -ENOTSOCK;
	}
	else {
		return fd_st->sock_num;
	}
}

int socket(int domain, int type, int protocol)
{
	/**
	 * First, we call lwip socket api to alloc the sock number
	*/
	int sock_n = lwip_socket(domain, type, protocol);
	if (sock_n < 0) {
#	ifdef LWIP_PROVIDE_ERRNO
		/**
		 * LWIP errno.h provide POSIX errno definitions that is compatible with 
		 * ECLayer exceptions.h. So if we have lwip global errno, just return 
		 * the exact value.
		*/
		return errno;
#	else
		/**
		 * Actual reason of error is not available.
		*/
		return -ENODEV;
#	endif
	}

	int32_t err;
	int32_t fd;
	file_des_t *fd_st;
	fd_st = (file_des_t *)ecmalloc(sizeof(file_des_t));
	if (fd_st == NULL) {
		err = -ENOMEM;
		goto close_socket;
	}
	else {
		do {
			fd = alloc_fd(fd_st);
		} while (fd == -EBUSY);
		if (fd < 0) {
			err = fd;
			ecfree(fd_st);
			while (free_fd(fd) != 0)
				;
			goto close_socket;
		}
		else {
			fd_st->file_flags = O_RDWR;
			fd_st->file_pos = 0;
			fd_st->file_type = e_FTYPE_SOCKET;
			fd_st->sock_num = sock_n;
		}
	}
	return err;

close_socket:
	lwip_close(sock_n);
error:
	return err;
}

#endif
