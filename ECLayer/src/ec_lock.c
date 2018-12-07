/**
 * @file	ec_lock.c
 * @brief	Implementation or wrapper of the lock function.
 * @author	Eggcar
 * @date	2017.08.16
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
