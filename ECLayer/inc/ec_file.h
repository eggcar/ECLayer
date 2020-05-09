/**
 * @file	ec_file.h
 * @brief	Definition of file operation structure.
 * @authro	Eggcar
 * @date	2017.08.16
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

#ifndef __EC_FILE_H
#define __EC_FILE_H

#include "ec_atomic.h"
#include "ec_config.h"
#include "ec_list.h"
#include "ec_lock.h"

#include <stddef.h>
#include <stdint.h>

typedef enum file_type_e {
	e_FTYPE_DEV,
	e_FTYPE_LINK,
	e_FTYPE_SOCKET,
} file_type_t;

typedef struct file_s {
	char file_name[_FILE_NAME_MAXLEN + 1];
	list_t file_list;
	atomic_t file_refs;
	ec_lock_t file_lock;
	struct file_opts_s *file_opts;
	void *file_content;
	//!< point to dev struct
} file_t;

typedef struct file_des_s {
	uint32_t file_flags;
	int64_t file_pos;
	file_type_t file_type;
	union {
		struct file_s *file;
		int sock_num;
	};
} file_des_t;

typedef struct file_opts_s {
	int32_t (*open)(file_des_t *, const char *, uint32_t);
	int32_t (*read)(file_des_t *, char *, size_t);
	int32_t (*write)(file_des_t *, const char *, size_t);
	int32_t (*ioctl)(file_des_t *, uint32_t, uint64_t);
	int64_t (*lseek)(file_des_t *, int64_t, int32_t);
	void *(*mmap)(file_des_t *, size_t, int32_t);
	int32_t (*close)(file_des_t *);
} file_opts_t;

int32_t file_regist(file_t *file);

int32_t file_deregist(file_t *file);

file_t *create_file(const char *filename, file_opts_t *fopts, void *content);

int32_t delete_file(file_t *file);

file_t *search_file(const char *filename);

int32_t empty_file(file_t *file);

#endif
