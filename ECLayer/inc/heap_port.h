/**
  *@file	heap_port.h
  *@brief	Port file of heap management functions.
  *			When you're migrating the system to another system,
  *			you should check and modify the implementation of 
  *			this file so that these funcs defined here provides
  *			the correct function of heap management whether you're
  *			using C-standard malloc or third party heap managing
  *			component, or the heap implementation of your own.
  *@author	Eggcar
  *@date	2017.08.09
  */
#ifndef __HEAP_PORT_H
#define __HEAP_PORT_H

#include <stddef.h>

void* ecmalloc(size_t size);

void ecfree(void * p);

#endif
