/**
 * @file	ec_fdlist.c
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

#include "ec_fdlist.h"

#include "ec_config.h"
#include "ec_file.h"
#include "ec_lock.h"
#include "exceptions.h"

#include <stddef.h>
#include <stdint.h>

static file_des_t *pv_fd_array[_FD_LIST_MAXNUM];
static ec_lock_t pv_fd_lock = e_Unlocked;

file_des_t *get_fd_struct(int32_t fd)
{
	if ((fd >= 0) && (fd < _FD_LIST_MAXNUM))
		return pv_fd_array[fd];
	// Does get function need to be thread-safe?
	else
		return NULL;
}

int32_t alloc_fd(file_des_t *file_des)
{
	uint32_t irqflag;
	if (ec_try_lock_irqsave(&pv_fd_lock, &irqflag) == 0) {
		for (int32_t i = 0; i < _FD_LIST_MAXNUM; i++) {
			if (pv_fd_array[i] == NULL) {
				pv_fd_array[i] = file_des;
				ec_unlock_irqrestore(&pv_fd_lock, irqflag);
				return i;
			}
			else {
				// continue;
			}
		}
		ec_unlock_irqrestore(&pv_fd_lock, irqflag);
		return -EBADF;  // No alloc-able fd.
	}
	else {
		return -EBUSY;  // file descriptor list is locked.
	}
}

int32_t free_fd(int32_t fd)
{
	uint32_t irqflag;
	if (ec_try_lock_irqsave(&pv_fd_lock, &irqflag) == 0) {
		pv_fd_array[fd] = NULL;
		ec_unlock_irqrestore(&pv_fd_lock, irqflag);
		return 0;
	}
	else {
		return -EBUSY;  // file descriptor list is locked.
	}
}
