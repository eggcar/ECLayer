/**
 * @file	heap_port.h
 * @brief	Port file of heap management functions.
 * 			When you're migrating the system to another system,
 * 			you should check and modify the implementation of 
 * 			this file so that these funcs defined here provides
 * 			the correct function of heap management whether you're
 * 			using C-standard malloc or third party heap managing
 * 			component, or the heap implementation of your own.
 * @author	Eggcar
 * @date	2017.08.09
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

#ifndef __HEAP_PORT_H
#define __HEAP_PORT_H

#include "ec_config.h"

#include <stddef.h>

void *ecmalloc(size_t size);

void ecfree(void *p);

void *ecrealloc(void *p, size_t size);

void *eccalloc(size_t n, size_t size);

#endif
