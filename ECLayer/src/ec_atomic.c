/**
 * @file	ec_atomic.c
 * @brief	Atomic operation support in ECLayer.
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

#include "ec_atomic.h"

#include "cmsis_port.h"

#include <stddef.h>
#include <stdint.h>

void inline atomic_set(atomic_t *ptr, atomic_t val)
{
	atomic_t tmp;
	do {
		tmp = __LDREXW(ptr);
	} while (__STREXW(val, ptr) == 1);
	(void)tmp;
	return;
}

atomic_t inline atomic_get(atomic_t *ptr)
{
	atomic_t tmp;
	do {
		tmp = __LDREXW(ptr);
	} while (__STREXW(tmp, ptr) == 1);
	return tmp;
}

void inline atomic_add(atomic_t *ptr, int32_t n)
{
	atomic_t tmp;
	do {
		tmp = __LDREXW(ptr);
	} while (__STREXW(tmp + n, ptr) == 1);
	return;
}

int32_t inline atomic_dec_and_test(atomic_t *ptr)
{
	atomic_t tmp;
	do {
		tmp = __LDREXW(ptr) - 1;
	} while (__STREXW(tmp, ptr) == 1);
	return (tmp == 0) ? 0 : 1;
}

int32_t inline atomic_inc_and_test(atomic_t *ptr)
{
	atomic_t tmp;
	do {
		tmp = __LDREXW(ptr) + 1;
	} while (__STREXW(tmp, ptr) == 1);
	return (tmp == 0) ? 0 : 1;
}

int32_t inline atomic_dec_and_eq(atomic_t *ptr, int32_t val)
{
	atomic_t tmp;
	do {
		tmp = __LDREXW(ptr) - 1;
	} while (__STREXW(tmp, ptr) == 1);
	return (tmp == val) ? 0 : 1;
}

int32_t inline atomic_inc_and_eq(atomic_t *ptr, int32_t val)
{
	atomic_t tmp;
	do {
		tmp = __LDREXW(ptr) + 1;
	} while (__STREXW(tmp, ptr) == 1);
	return (tmp == val) ? 0 : 1;
}

void inline atomic_inc(atomic_t *ptr)
{
	atomic_t tmp;
	do {
		tmp = __LDREXW(ptr) + 1;
	} while (__STREXW(tmp, ptr) == 1);
	return;
}

void inline atomic_dec(atomic_t *ptr)
{
	atomic_t tmp;
	do {
		tmp = __LDREXW(ptr) - 1;
	} while (__STREXW(tmp, ptr) == 1);
	return;
}
