/**
 * @file	ec_file.c
 * @brief	Basic file operations.
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

#include "ec_file.h"

#include "ec_config.h"
#include "ec_list.h"
#include "exceptions.h"
#include "heap_port.h"

#include <stddef.h>
#include <stdint.h>
#include <string.h>

static list_head_def(pv_sysfile_list);
static ec_lock_t pv_sysfile_list_lock = e_Unlocked;

int32_t file_regist(file_t *file)
{
	uint32_t irqflag;
	if (ec_try_lock_irqsave(&pv_sysfile_list_lock, &irqflag) != 0) {
		return -EBUSY;
	}
	else {
		if (file == NULL) {
			ec_unlock_irqrestore(&pv_sysfile_list_lock, irqflag);
			return -ENOENT;
		}
		else {
			foreach (flist_itr, &pv_sysfile_list) {
				if ((list_get_node(flist_itr, file_t, file_list)) == file) {
					ec_unlock_irqrestore(&pv_sysfile_list_lock, irqflag);
					return -EFREGED;  // file already registed in the list.
				}
				else {
					// continue;
				}
			}
			list_append(&(file->file_list), &pv_sysfile_list);
			ec_unlock_irqrestore(&pv_sysfile_list_lock, irqflag);
			return 0;
		}
	}
}

int32_t file_deregist(file_t *file)
{
	uint32_t irqflag;
	if (ec_try_lock_irqsave(&pv_sysfile_list_lock, &irqflag) != 0) {
		return -EBUSY;
	}
	else {
		if (file == NULL) {
			ec_unlock_irqrestore(&pv_sysfile_list_lock, irqflag);
			return -ENOENT;
		}
		else {
			foreach (flist_itr, &pv_sysfile_list) {
				if (list_get_node(flist_itr, file_t, file_list) == file) {
					list_delete(&(file->file_list));
					ec_unlock_irqrestore(&pv_sysfile_list_lock, irqflag);
					return 0;
				}
				else {
					// continue;
				}
			}
			ec_unlock_irqrestore(&pv_sysfile_list_lock, irqflag);
			return -EFNOREG;
		}
	}
}

file_t *create_file(const char *filename, file_opts_t *fopts, void *content)
{
	file_t *file;
	file_t *file_itr;
	if (strlen(filename) > _FILE_NAME_MAXLEN) {
		// file name invalid
		return NULL;
	}
	foreach (flist_itr, &pv_sysfile_list) {
		file_itr = list_get_node(flist_itr, file_t, file_list);
		if (strcmp(file_itr->file_name, filename) == 0) {
			// file name already exist
			return NULL;
		}
		else {
			// continue;
		}
	}
	file = (file_t *)ecmalloc(sizeof(file_t));
	if (file == NULL) {
		// do something
		return NULL;
	}
	else {
		strcpy(file->file_name, filename);
		atomic_set(&(file->file_refs), 0);
		file->file_lock = e_Unlocked;
		file->file_opts = fopts;
		file->file_content = content;
		return file;
	}
}

int32_t delete_file(file_t *file)
{
	uint32_t irqflag;
	if (ec_try_lock_irqsave(&pv_sysfile_list_lock, &irqflag) != 0) {
		return -EBUSY;
	}
	else {
		if (atomic_get(&(file->file_refs)) > 0) {
			return -EBUSY;  // file is opened by someone
		}
		if (list_is_in(&pv_sysfile_list, &(file->file_list)) == 0) {
			ec_unlock_irqrestore(&pv_sysfile_list_lock, irqflag);
			return -EFNOREG;  // *file is not in system file list
		}
		else {
			ecfree(file);
			ec_unlock_irqrestore(&pv_sysfile_list_lock, irqflag);
			return 0;
		}
	}
}

file_t *search_file(const char *filename)
{
	file_t *file_itr;
	if (strlen(filename) > _FILE_NAME_MAXLEN) {
		return NULL;
	}
	foreach (flist_itr, &pv_sysfile_list) {
		file_itr = list_get_node(flist_itr, file_t, file_list);
		if (strcmp(file_itr->file_name, filename) == 0) {
			return file_itr;
		}
		else {
			// continue;
		}
	}
	return NULL;
}

int32_t empty_file(file_t *file)
{
	uint32_t irqflag;
	if (file == NULL) {
		return -ENOENT;  // file pointer is null.
	}
	else {
		if (ec_try_lock_irqsave(&(file->file_lock), &irqflag) != 0) {
			return -EBUSY;  // file is locked.
		}
		else {
			file->file_opts = NULL;
			file->file_content = NULL;
			ec_unlock_irqrestore(&(file->file_lock), irqflag);
			return 0;
		}
	}
}
