/**
 * @file	cfifo.c
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

#include "cfifo.h"

#include "ec_lock.h"
#include "exceptions.h"
#include "heap_port.h"

#include <stddef.h>
#include <stdint.h>

static inline void __push(cfifo_t *fifo, const char ch);
static inline void __pop(cfifo_t *fifo, char *ch);

cfifo_t *cfifo_new(int32_t n)
{
	cfifo_t *fifo;
	if (n <= 0) {
		return NULL;
	}
	else {
		fifo = (cfifo_t *)ecmalloc(sizeof(cfifo_t) + (sizeof(char) * (n - 1)));
		if (fifo != NULL) {
			fifo->head = 0;
			fifo->tail = 0;
			fifo->usedw = 0;
			fifo->depth = n;
			fifo->pushlock = e_Unlocked;
			fifo->poplock = e_Unlocked;
		}
		return fifo;
	}
}

void cfifo_delete(cfifo_t *fifo)
{
	ecfree(fifo);
}

/**
  *@brief	Push a character into a specified fifo.
  *@param	*fifo		pointer to fifo structure to be operated
  *@param	ch			character to be pushed
  *@retval	0			operation succeed
  *@retval	-EFIFOFULL	fifo is full
  *@retval	-EBUSY		fifo is push-locked
  */
int32_t cfifo_push(cfifo_t *fifo, const char ch)
{
	uint32_t irqflag;
	if (fifo->usedw >= fifo->depth) {
		/*
		 * Because these situations below are unlikely to happen while using our legal 
		 * process funcs in this file, and we need this func runs very frequently, so 
		 * we just ignored them :
		 *	(fifo->tail == fifo->head - 1) ||
		 *	((fifo->head == 0) && (fifo->tail == fifo->depth - 1)))
		 */
		return -EFIFOFULL;
	}
	else if (ec_try_lock_irqsave(&(fifo->pushlock), &irqflag) != 0) {
		return -EBUSY;
	}
	else {
		__push(fifo, ch);
		ec_unlock_irqrestore(&(fifo->pushlock), irqflag);
		return 0;
	}
}

/**
  *@brief	Pop a character from a specified fifo
  *@param	*fifo		pointer to fifo structure to be operated
  *@param	*ch			popped character
  *@retval	0			operation succeed
  *@retval	-EFIFOEMPTY	fifo is empty
  *@retval	-EBUSY		fifo is pop-locked
  */
int32_t cfifo_pop(cfifo_t *fifo, char *ch)
{
	uint32_t irqflag;
	if (fifo->usedw <= 0) {
		return -EFIFOEMPTY;
	}
	else if (ec_try_lock_irqsave(&(fifo->poplock), &irqflag) != 0) {
		return -EBUSY;
	}
	else {
		__pop(fifo, ch);
		ec_unlock_irqrestore(&(fifo->poplock), irqflag);
		return 0;
	}
}

/**
  *@brief	Push n characters into a specified fifo
  *@param	*fifo		pointer to fifo structure to be operated
  *@param	ch[]		characters to be pushed
  *@param	n			count of pushed chars
  *@retval	-EFIFOFULL	fifo is full
  *@retval	-EBUSY		fifo is push-locked
  *@retval	>=0			count of pushed characters
  */
int32_t cfifo_pushn(cfifo_t *fifo, const char ch[], int32_t n)
{
	uint32_t irqflag;
	if (fifo->usedw >= fifo->depth) {
		return -EFIFOFULL;
	}
	else if (ec_try_lock_irqsave(&(fifo->pushlock), &irqflag) != 0) {
		return -EBUSY;
	}
	else {
		int i;
		for (i = 0;
			 (fifo->usedw < fifo->depth) && (i < n);
			 i++) {
			__push(fifo, ch[i]);
		}
		ec_unlock_irqrestore(&(fifo->pushlock), irqflag);
		return i;
	}
}

/**
  *@brief	Pop n characters from a specified fifo
  *@param	*fifo		pointer to fifo structure to be operated
  *@param	ch[]		array contains the popped characters
  *@param	n			count of popped chars
  *@retval	-EFIFOEMPTY	fifo is empty
  *@retval	-EBUSY		fifo is pop-locked
  *@retval	>=0			count of popped characters
  */
int32_t cfifo_popn(cfifo_t *fifo, char ch[], int32_t n)
{
	uint32_t irqflag;
	if (fifo->usedw <= 0) {
		return -EFIFOEMPTY;
	}
	else if (ec_try_lock_irqsave(&(fifo->poplock), &irqflag) != 0) {
		return -EBUSY;
	}
	else {
		int i;
		for (i = 0;
			 (fifo->usedw > 0) && (i < n);
			 i++) {
			__pop(fifo, &ch[i]);
		}
		ec_unlock_irqrestore(&(fifo->poplock), irqflag);
		return i;
	}
}

static inline void __push(cfifo_t *fifo, const char ch)
{
	fifo->fifo[fifo->tail] = ch;
	fifo->usedw++;
	if (fifo->tail == fifo->depth - 1) {
		fifo->tail = 0;
	}
	else {
		fifo->tail++;
	}
	return;
}

static inline void __pop(cfifo_t *fifo, char *ch)
{
	*ch = fifo->fifo[fifo->head];
	if (fifo->head == fifo->depth - 1) {
		fifo->head = 0;
	}
	else {
		fifo->head++;
	}
	fifo->usedw--;
	return;
}
