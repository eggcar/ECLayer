/**
 * @file	ec_lock.h
 * @brief	Implementation or wrapper of the lock function.
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

#ifndef __EC_LOCK_H
#define __EC_LOCK_H

#include <stdint.h>

#define	e_Locked 1
#define e_Unlocked 0

typedef uint32_t ec_lock_t;

int32_t ec_try_lock_irqsave(ec_lock_t *lock, uint32_t *irqflag);

void ec_unlock_irqrestore(ec_lock_t *lock, uint32_t irqflag);

int32_t ec_try_lock(ec_lock_t *lock);

void ec_unlock(ec_lock_t *lock);

#endif
