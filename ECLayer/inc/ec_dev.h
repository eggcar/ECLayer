/**
 * @file	ec_dev.h
 * @brief	Device structure definition file.
 * @author	Eggcar
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

#ifndef __EC_DEV_H
#define __EC_DEV_H

#include "ec_config.h"
#include "ec_list.h"

#include <stddef.h>
#include <stdint.h>

typedef enum {
	e_devt_STREAM = 1,
	e_devt_GPIO,
	e_devt_STORAGE,
	e_devt_NETWORK,
	e_devt_FILE,
	e_devt_MMAP
} ec_dev_type_t;

typedef struct ec_dev_s {
	char dev_name[_DEV_NAME_MAXLEN + 1];
	ec_dev_type_t dev_type;
	list_t dev_list;
	void *private_data;
} ec_dev_t;

int32_t device_regist(ec_dev_t *dev);

int32_t device_deregist(ec_dev_t *dev);

ec_dev_t *search_device(const char *device_name);

#endif
