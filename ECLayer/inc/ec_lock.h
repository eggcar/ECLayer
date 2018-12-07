/**
 * @file	ec_lock.h
 * @brief	Implementation or wrapper of the lock function.
 * @author	Eggcar
 * @date	2017.08.16
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
