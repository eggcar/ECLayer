/**
  *@file	cfifo.h
  *@brief	Character FIFO implementation
  *@author	Eggcar
  *@date	2017.08.09
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
