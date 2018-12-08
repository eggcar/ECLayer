/**
 * @file	ec_mmap.h
 * @brief	mmap() associated definitions.
 * @detail	mmap() function is implemented mostly for the operation
 * 			of FPGA and pixel-display-devices. But ECLayer does not
 * 			support MMU and MPU yet, mmap() just return the actual
 * 			address of the device registers or buffer directly to
 * 			user programs. Support for MMU and MPU should be added
 * 			if you're migrating ECLayer to a CPU that support MMU
 * 			or MPU. When doing this, Please refer to the OS you're 
 * 			using for more details and apis of how to map phys addr
 * 			to virtual addr. Or you can just disable mmap() function
 * 			by editing ec_config.h
 * 			But it's difficult to implement mmap() fully compatible
 * 			to Linux, as the protection and share flags need more OS
 * 			layer support, which is not my purpose to design ECLayer.
 * 			Full compatible implementation would be considered later.
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

#ifndef __EC_MMAP_H
#define __EC_MMAP_H
#include "ec_config.h"

#include <stdint.h>

#if _USE_MMAP > 0
// Infact, without MMU or MPU, memory protect has no effect.
#	define PROT_READ (int32_t)(0x01 << 0)
#	define PROT_WRITE (int32_t)(0x01 << 1)
#	define PROT_EXEC (int32_t)(0x01 << 2)
#	define PROT_NONE (int32_t)(0x01 << 3)
// Without MMU or MPU, there is no virtual memory space,
// so these flags has no effect either.
#	define MAP_FIXED (int32_t)(0x01)
#	define MAP_PRIVATE (int32_t)(0x01 << 1)
#	define MAP_SHARED (int32_t)(0x01 << 2)
#endif

#endif
