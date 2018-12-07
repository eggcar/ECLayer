/**
 * @file	cmsis_port.h
 * @brief	Portable file for ARM CMSIS core
 * @author	Eggcar
*/

#ifndef __CMSIS_PORT_H
#define __CMSIS_PORT_H

//#include "core_cm0.h"
//#include "core_cm0plus.h"
//#include "core_cm3.h"
//#include "core_cm4.h"
//#include "core_cm7.h"
//#include "core_sc000.h"
//#include "core_sc300.h"

/*
#define __LDREXW(x) __ldrex(x)
#define __STREXW(x,y) __strex(x,y)

static inline uint32_t __get_PRIMASK(void)
{
	register uint32_t __regPriMask         __asm("primask");
	return(__regPriMask);
}

static inline void __set_PRIMASK(uint32_t priMask)
{
	register uint32_t __regPriMask         __asm("primask");
	__regPriMask = (priMask);
}
*/

#include "stm32f4xx.h"

#endif
