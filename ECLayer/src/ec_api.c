/**
 * @file	ec_api.c
 * @brief	Basic interface for ECLayer users.
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

#include "ec_api.h"

#include "ec_config.h"
#include "ec_fcntl.h"
#include "ec_fdlist.h"
#include "ec_file.h"
#include "ec_mmap.h"
#include "exceptions.h"
#include "heap_port.h"

#include <stddef.h>
#include <stdint.h>

#if _WITH_LWIP_SOCKET_WRAPPER
#	include "ec_lwip_wrapper.h"
#endif

int32_t open(const char *filename, uint32_t flags)
{
	int32_t err;
	file_t *file;
	int16_t new_file_opened = 0;
	file = search_file(filename);

	if (file == NULL) {
		if ((flags & O_CREAT) != 0x0) {
			file = create_file(filename, NULL, NULL);
			if (file == NULL) {
				err = -ENOMEM;	// failed to create new file.
				goto error;
			}
			else {
				new_file_opened = 1;
			}
		}
		else {
			err = -ENOENT;	// file name not exist.
			goto error;
		}
	}
	else {
		if ((flags & O_CREAT) != 0x0) {
			if ((flags & O_EXEC) != 0x0) {
				err = -EEXIST;	// file already exists.
				goto error;
			}
			else if ((flags & O_TRUNC) != 0x0) {
				if (empty_file(file) != 0) {
					err = -EBUSY;  // failed to trunc file, probably file is locked
					goto error;
				}
			}
			else {
				// assume O_CREAT is ignored and file is located successfully
			}
		}
		else {
			// file is located successfully
		}
	}

	int32_t fd;
	file_des_t *fd_st;
	fd_st = (file_des_t *)ecmalloc(sizeof(file_des_t));
	if (fd_st == NULL) {
		err = -ENOMEM;
		if (new_file_opened) {
			ecfree(file);
		}
		goto error;
	}
	else {
		/**
		 *@warning	if RTOS is not used, and fdlist is locked,
		 *			a deadlock is produced here.
		 *@todo		Probable solutions :
		 *			1.Use mutex or semaphore instead of ec_lock in ECLayer kernel
		 *			2.Introduce a wrapper for manual thread manager that suits RTOS and non-RTOS
		 *			3.Use macro that indicates non-RTOS situation and return error directly
		 *			I would like to modify ec_lock to support non block lock wait mechanism.
		 */
		do {
			fd = alloc_fd(fd_st);
		} while (fd == -EBUSY);
		if (fd < 0) {
			err = fd;
			if (new_file_opened) {
				ecfree(file);
			}
			ecfree(fd_st);
			while (free_fd(fd) != 0)
				;
			goto error;
		}
		else {
			fd_st->file_flags = flags;
			fd_st->file_pos = 0;
			/**
			 * Default set file type to e_FTYPE_DEV, but might be changed
			 * in later version. Do not rely on it.
			*/
			fd_st->file_type = e_FTYPE_DEV;
			fd_st->file = file;
		}
	}

	if (file->file_opts != NULL) {
		if (file->file_opts->open != NULL) {
			err = file->file_opts->open(fd_st, filename, flags);
			if (err < 0) {
				// error occurred when calling file specified open operation
				ecfree(fd_st);
				while (free_fd(fd) != 0)
					;
				goto error;
			}
			else {
				goto private_open_success;	// file-defined open operation success
			}
		}
		else {
			goto success;  // file does not provide open function
		}
	}
	else {
		goto success;  // file does not provide any operation
	}

success:
	atomic_inc(&(file->file_refs));
private_open_success:
	err = fd;
error:
	return err;
}

int32_t close(int32_t fd)
{
	int32_t err;
	if ((fd < 0) || (fd > _FD_LIST_MAXNUM)) {
		err = -EMFILE;	// illegal fd input
		goto error;
	}
	file_des_t *fd_st;
	fd_st = get_fd_struct(fd);
	if (fd_st == NULL) {
		// fd does not exist
		err = -EBADF;
		goto error;
	}
#if _WITH_LWIP_SOCKET_WRAPPER
	if (fd_st->file_type == e_FTYPE_SOCKET) {
		err = (int32_t)lwip_close(ec_fd2sock(fd));
		if (!err) {
			ecfree(fd_st);
		}
		return err;
	}
#endif
	file_t *file;
	file = fd_st->file;

	err = free_fd(fd);
	if (err < 0) {
		goto error;
	}
	if (file->file_opts != NULL) {
		if (file->file_opts->close != NULL) {
			err = file->file_opts->close(fd_st);
		}
		else {
			err = 0;
		}
	}
	else {
		err = 0;
	}
	ecfree(fd_st);
	atomic_dec(&(file->file_refs));
error:
	return err;
}

int32_t write(int32_t fd, const char *data, size_t length)
{
	int32_t err;
	file_des_t *file_des;
	file_des = get_fd_struct(fd);
	if (file_des == NULL) {
		err = -EBADF;
	}
#if _WITH_LWIP_SOCKET_WRAPPER
	else if (file_des->file_type == e_FTYPE_SOCKET) {
		err = (int32_t)lwip_write(ec_fd2sock(fd), data, length);
	}
#endif
	else if (file_des->file == NULL) {
		err = -EFDNOFILE;  // file descriptor does not contain valid file.
	}
	else if (file_des->file->file_opts->write == NULL) {
		err = -ENOTSUP;	 // file does not support write operation.
	}
	else {
		err = file_des->file->file_opts->write(file_des, data, length);
	}
	return err;
}

int32_t read(int32_t fd, char *data, size_t length)
{
	int32_t err;
	file_des_t *file_des;
	file_des = get_fd_struct(fd);
	if (file_des == NULL) {
		err = -EBADF;
	}
#if _WITH_LWIP_SOCKET_WRAPPER
	else if (file_des->file_type == e_FTYPE_SOCKET) {
		err = (int32_t)lwip_read(ec_fd2sock(fd), data, length);
	}
#endif
	else if (file_des->file == NULL) {
		err = -EFDNOFILE;  // file descriptor does not contain valid file.
	}
	else if (file_des->file->file_opts->read == NULL) {
		err = -ENOTSUP;	 // file does not support read operation.
	}
	else {
		err = file_des->file->file_opts->read(file_des, data, length);
	}
	return err;
}

int32_t ioctl(int32_t fd, uint32_t cmd, uint64_t arg)
{
	int32_t err;
	file_des_t *file_des;
	file_des = get_fd_struct(fd);
	if (file_des == NULL) {
		err = -EBADF;
	}
#if _WITH_LWIP_SOCKET_WRAPPER
	else if (file_des->file_type == e_FTYPE_SOCKET) {
		err = (int32_t)lwip_ioctl(ec_fd2sock(fd), cmd, (void *)arg);
	}
#endif
	else if (file_des->file == NULL) {
		err = -EFDNOFILE;  // file descriptor does not contain valid file.
	}
	else if (file_des->file->file_opts->ioctl == NULL) {
		err = -ENOTSUP;
	}
	else {
		err = file_des->file->file_opts->ioctl(file_des, cmd, arg);
	}
	return err;
}

int64_t lseek(int32_t fd, int64_t offset, int32_t origin)
{
	int32_t err;
	file_des_t *file_des;
	file_des = get_fd_struct(fd);
	if (file_des == NULL) {
		err = -EBADF;
	}
#if _WITH_LWIP_SOCKET_WRAPPER
	else if (file_des->file_type == e_FTYPE_SOCKET) {
		err = -ENOTSUP;
	}
#endif
	else if (file_des->file == NULL) {
		err = -EFDNOFILE;
	}
	else if (file_des->file->file_opts->lseek == NULL) {
		err = -ENOTSUP;
	}
	else {
		err = file_des->file->file_opts->lseek(file_des, offset, origin);
	}
	return err;
}

int32_t fcntl(int32_t fd, int32_t cmd, int32_t arg)
{
	int32_t err;
	file_des_t *file_des;
	file_des = get_fd_struct(fd);
	if (file_des == NULL) {
		err = -EBADF;
	}
#if _WITH_LWIP_SOCKET_WRAPPER
	else if (file_des->file_type == e_FTYPE_SOCKET) {
		err = lwip_fcntl(ec_fd2sock(fd), (int)cmd, (int)arg);
		if (err >= 0) {
			switch (cmd) {
			case F_GETFL:
				err = file_des->file_flags;
				break;
			case F_SETFL:
				file_des->file_flags = arg;
				err = 0;
				break;
			default:
				err = -EINVAL;
				break;
			}
		}
	}
#endif
	else if (file_des->file == NULL) {
		err = -EFDNOFILE;
	}
	else {
		switch (cmd) {
		case F_GETFL:
			err = file_des->file_flags;
			break;
		case F_SETFL:
			file_des->file_flags = arg;
			err = 0;
			break;
		default:
			err = -EINVAL;
			break;
		}
	}
	return err;
}

#if _USE_MMAP > 0
void *mmap(void *addr, size_t len, int32_t prot, int32_t flags, int32_t fd, int32_t offset)
{
	int32_t err;
	file_des_t *file_des;
	void *rtptr = NULL;
	if (fd < 0) {
		// Anonymous map not supported yet.
		err = -EBADF;
		rtptr = (void *)err;
		goto error;
	}
	file_des = get_fd_struct(fd);
	if (file_des == NULL) {
		err = -EBADF;
		rtptr = (void *)err;
		goto error;
	}
	else if (file_des->file == NULL) {
		err = -EFDNOFILE;
		rtptr = (void *)err;
		goto error;
	}
	else if (file_des->file->file_opts->mmap == NULL) {
		err = -ENOTSUP;
		rtptr = (void *)err;
		goto error;
	}
	else {
		rtptr = file_des->file->file_opts->mmap(file_des, len, offset);
	}
error:
	return rtptr;
}
#endif
