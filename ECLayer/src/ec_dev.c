/**
 * @file	ec_dev.c
 * @brief	Basic device operations.
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

#include "ec_dev.h"

#include "ec_list.h"
#include "ec_lock.h"
#include "exceptions.h"

#include <stddef.h>
#include <string.h>
#include <stdint.h>

static list_head_def(pv_dev_list);
static ec_lock_t pv_lock_dev_list = e_Unlocked;

int32_t device_regist(ec_dev_t *dev)
{
	uint32_t irqflag;
	if (ec_try_lock_irqsave(&pv_lock_dev_list, &irqflag) != 0) {
		return -EBUSY;  // Device list is busy
	}
	else {
		if (dev == NULL) {
			ec_unlock_irqrestore(&pv_lock_dev_list, irqflag);
			return -ENXIO;  // No such device
		}
		else {
			foreach (dlist_itr, &pv_dev_list) {
				if (list_get_node(dlist_itr, ec_dev_t, dev_list) == dev) {
					ec_unlock_irqrestore(&pv_lock_dev_list, irqflag);
					return -EDEVREGED;  // Device is already registed
				}
				else {
					// continue;
				}
			}
			list_append(&(dev->dev_list), &pv_dev_list);
			ec_unlock_irqrestore(&pv_lock_dev_list, irqflag);
			return 0;
		}
	}
}

int32_t device_deregist(ec_dev_t *dev)
{
	uint32_t irqflag;
	if (ec_try_lock_irqsave(&pv_lock_dev_list, &irqflag) != 0) {
		return -EBUSY;  // Device list is busy
	}
	else {
		if (dev == NULL) {
			ec_unlock_irqrestore(&pv_lock_dev_list, irqflag);
			return -ENXIO;
		}
		else {
			foreach (dlist_itr, &pv_dev_list) {
				if (list_get_node(dlist_itr, ec_dev_t, dev_list) == dev) {
					list_delete(&(dev->dev_list));
					ec_unlock_irqrestore(&pv_lock_dev_list, irqflag);
					return 0;
				}
				else {
					// continue;
				}
			}
			ec_unlock_irqrestore(&pv_lock_dev_list, irqflag);
			return -EDEVNOREG;
		}
	}
}

ec_dev_t *search_device(const char *device_name)
{
	ec_dev_t *dev_itr;
	foreach (dlist_itr, &pv_dev_list) {
		dev_itr = list_get_node(dlist_itr, ec_dev_t, dev_list);
		if (strcmp(device_name, dev_itr->dev_name) == 0) {
			return dev_itr;
		}
		else {
			// continue;
		}
	}
	return NULL;
}
