/**
 *@file		ec_api.c
 *@brief	Basic interface for ECLayer users.
 *@author	Eggcar
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
				err = -ENOMEM;  // failed to create new file.
				goto error;
			}
			else {
				new_file_opened = 1;
			}
		}
		else {
			err = -ENOENT;  // file name not exist.
			goto error;
		}
	}
	else {
		if ((flags & O_CREAT) != 0x0) {
			if ((flags & O_EXEC) != 0x0) {
				err = -EEXIST;  // file already exists.
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
				goto private_open_success;  // file-defined open operation success
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
		err = -EMFILE;  // illegal fd input
		goto error;
	}
	file_des_t *fd_st;
	fd_st = get_fd_struct(fd);
	if (fd_st == NULL) {
		// fd does not exist
		err = -EBADF;
		goto error;
	}
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
	else if (file_des->file == NULL) {
		err = -EFDNOFILE;  // file descriptor does not contain valid file.
	}
	else if (file_des->file->file_opts->write == NULL) {
		err = -ENOTSUP;  // file does not support write operation.
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
	else if (file_des->file == NULL) {
		err = -EFDNOFILE;  // file descriptor does not contain valid file.
	}
	else if (file_des->file->file_opts->read == NULL) {
		err = -ENOTSUP;  // file does not support read operation.
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

#if _USE_MMAP > 0
void *mmap(void *addr, size_t len, int32_t prot, int32_t flags, int32_t fd, int32_t offset)
{
	int32_t err;
	file_des_t *file_des;
	void *rtptr = NULL;
	if (fd < 0) {
		// Anonymous map not supported yet.
		err = -EBADF;
		goto error;
	}
	file_des = get_fd_struct(fd);
	if (file_des == NULL) {
		err = -EBADF;
		goto error;
	}
	else if (file_des->file == NULL) {
		err = -EFDNOFILE;
		goto error;
	}
	else if (file_des->file->file_opts->mmap == NULL) {
		err = -ENOTSUP;
		goto error;
	}
	else {
		rtptr = file_des->file->file_opts->mmap(file_des, len, offset);
	}
error :
	return rtptr;
}
#endif
