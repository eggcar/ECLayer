/**
 * @file	ec_fdlist.h
 * @brief	File descriptor list.
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

#ifndef __EC_FDLIST_H
#define __EC_FDLIST_H

#include "ec_file.h"

#include <stdint.h>

file_des_t *get_fd_struct(int32_t fd);

int32_t alloc_fd(file_des_t *fs);

int32_t free_fd(int32_t fd);

#endif
