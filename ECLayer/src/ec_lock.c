/**
 * @file	ec_lock.c
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

#include "ec_lock.h"

#include "cmsis_port.h"
#include "ec_atomic.h"
#include "exceptions.h"

#include <stddef.h>
#include <stdint.h>

int32_t ec_try_lock_irqsave(ec_lock_t *lock, uint32_t *irqflag)
{
	*irqflag = __get_PRIMASK();
	__set_PRIMASK(1);
	ec_lock_t lock_state = atomic_get((atomic_t *)lock);
	if (lock_state == e_Unlocked) {
		atomic_set((atomic_t *)lock, e_Locked);
		return 0;
	}
	else {
		// If we failed to acquire the lock, we should recover
		// the IRQ status.
		__set_PRIMASK(*irqflag);
		return -EBUSY;
	}
}

void ec_unlock_irqrestore(ec_lock_t *lock, uint32_t irqflag)
{
	atomic_set((atomic_t *)lock, e_Unlocked);
	__set_PRIMASK(irqflag);
	return;
}

int32_t ec_try_lock(ec_lock_t *lock)
{
	uint32_t irqflag = __get_PRIMASK();
	int32_t rtval;
	__set_PRIMASK(1);
	ec_lock_t lock_state = atomic_get((atomic_t *)lock);
	if (lock_state == e_Unlocked) {
		atomic_set((atomic_t *)lock, e_Locked);
		rtval = 0;
	}
	else {
		rtval = -EBUSY;
	}
	__set_PRIMASK(irqflag);
	return rtval;
}

void ec_unlock(ec_lock_t *lock)
{
	uint32_t irqflag = __get_PRIMASK();
	__set_PRIMASK(1);
	atomic_set((atomic_t *)lock, e_Unlocked);
	__set_PRIMASK(irqflag);
	return;
}
