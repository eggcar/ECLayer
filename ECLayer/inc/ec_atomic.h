/**
 * @file	ec_atomic.h
 * @brief	Atomic operation support in ECLayer
 * @author	Eggcar
*/
#ifndef __EC_ATOMIC_H
#define __EC_ATOMIC_H

#include <stdint.h>

typedef int32_t atomic_t;

void atomic_set(atomic_t *ptr, atomic_t val);

atomic_t atomic_get(atomic_t *ptr);

void atomic_add(atomic_t *ptr, int32_t n);

int32_t atomic_dec_and_test(atomic_t *ptr);

int32_t atomic_inc_and_test(atomic_t *ptr);

int32_t atomic_dec_and_eq(atomic_t *ptr, int32_t val);

int32_t atomic_inc_and_eq(atomic_t *ptr, int32_t val);

void atomic_inc(atomic_t *ptr);

void atomic_dec(atomic_t *ptr);

#endif
