/**
 * @file	ec_api.h
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

#ifndef __EC_API_H
#define __EC_API_H

#include "ec_config.h"
#include "ec_fcntl.h"
#include "ec_mmap.h"
#include "heap_port.h"
#include "ioctl_cmd.h"
#if _WITH_LWIP_SOCKET_WRAPPER
#	include "ec_lwip_wrapper.h"
#endif

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

int32_t open(const char *filename, uint32_t flags);

int32_t close(int32_t fd);

int32_t write(int32_t fd, const char *data, size_t length);

int32_t read(int32_t fd, char *data, size_t length);

int32_t ioctl(int32_t fd, uint32_t cmd, uint64_t arg);

int64_t lseek(int32_t fd, int64_t offset, int32_t origin);

int32_t fcntl(int32_t fd, int32_t cmd, int32_t arg);

#endif
