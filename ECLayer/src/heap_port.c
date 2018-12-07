/**
  * @file 	heap_port.c
  * @brief	Port file of heap management functions.
  * 		When you're migrating the system to another system,
  * 		you should check and modify the implementation of 
  * 		this file so that these funcs defined here provides
  * 		the correct function of heap management whether you're
  * 		using C-standard malloc or third party heap managing
  * 		component, or the heap implementation of your own.
  * @author	Eggcar
  */
#include "FreeRTOS.h"

#include <stddef.h>

/*
 * In this version of implementation, we're using the FreeRTOS
 * heap_x for dynamic memory management.
 */
void *ecmalloc(size_t size)
{
	return pvPortMalloc(size);
}

void ecfree(void *p)
{
	vPortFree(p);
}
