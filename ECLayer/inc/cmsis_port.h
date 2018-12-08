/**
 * @file	cmsis_port.h
 * @brief	Portable file for ARM CMSIS core
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
