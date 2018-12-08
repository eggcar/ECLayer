/**
 * @file	cfifo.h
 * @brief	Character FIFO implementation
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

#pragma once
#include "ec_lock.h"

#include <stdint.h>

typedef struct {
	int32_t head;
	int32_t tail;
	int32_t usedw;
	int32_t depth;
	ec_lock_t pushlock;
	ec_lock_t poplock;
	char fifo[1];
} cfifo_t;

cfifo_t *cfifo_new(int32_t n);

void cfifo_delete(cfifo_t *fifo);

int32_t cfifo_push(cfifo_t *fifo, const char ch);

int32_t cfifo_pop(cfifo_t *fifo, char *ch);

int32_t cfifo_pushn(cfifo_t *fifo, const char ch[], int32_t n);

int32_t cfifo_popn(cfifo_t *fifo, char ch[], int32_t n);
