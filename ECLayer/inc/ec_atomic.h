/**
 * @file	ec_atomic.h
 * @brief	Atomic operation support in ECLayer
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
