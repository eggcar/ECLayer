/**
 * @file	ioctl_cmd.h
 * @brief	Definitions of ioctl commands.
 * @author 	Eggcar
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

#include "ec_config.h"
#include "ec_ioctl.h"

#include <stdint.h>

/**
 * STM32 U(S)ART commands
*/
#define STM32_USART_MAGIC	   'U'
#define CMD_USART_SETBAUD	   _IOW(STM32_USART_MAGIC, 1, uint32_t)
#define CMD_USART_GETBAUD	   _IOR(STM32_USART_MAGIC, 2, uint32_t *)
#define CMD_USART_SETDATAWIDTH _IOW(STM32_USART_MAGIC, 3, uint32_t)
#define CMD_USART_GETDATAWIDTH _IOR(STM32_USART_MAGIC, 4, uint32_t *)
#define CMD_USART_SETSTOPBIT   _IOW(STM32_USART_MAGIC, 5, uint32_t)
#define CMD_USART_GETSTOPBIT   _IOR(STM32_USART_MAGIC, 6, uint32_t *)
#define CMD_USART_SETPARITY	   _IOW(STM32_USART_MAGIC, 7, uint32_t)
#define CMD_USART_GETPARITY	   _IOR(STM32_USART_MAGIC, 8, uint32_t *)
#define CMD_USART_ENABLE	   _IO(STM32_USART_MAGIC, 9)
#define CMD_USART_DISABLE	   _IO(STM32_USART_MAGIC, 10)
#define CMD_USART_GETREADTS	   _IOR(STM32_USART_MAGIC, 11, void *)
#define CMD_USART_GETWRITETS   _IOR(STM32_USART_MAGIC, 12, void *)

/**
 * Char LCD module commands
*/
#define CHAR_LCD_MAGIC			  'C'
#define CMD_CHARLCD_CLEAR		  _IO(CHAR_LCD_MAGIC, 1)
#define CMD_CHARLCD_HOME		  _IO(CHAR_LCD_MAGIC, 2)
#define CMD_CHARLCD_ENTRYMODE	  _IOW(CHAR_LCD_MAGIC, 3, uint32_t)
#define CMD_CHARLCD_DISPLAY		  _IOW(CHAR_LCD_MAGIC, 4, uint32_t)
#define CMD_CHARLCD_SHIFT		  _IOW(CHAR_LCD_MAGIC, 5, uint32_t)
#define CMD_CHARLCD_FUNCTION	  _IOW(CHAR_LCD_MAGIC, 6, uint32_t)
#define CMD_CHARLCD_SET_CGRAMADDR _IOW(CHAR_LCD_MAGIC, 7, uint32_t)
#define CMD_CHARLCD_SET_DDRAMADDR _IOW(CHAR_LCD_MAGIC, 8, uint32_t)
#define CMD_CHARLCD_CURSOR2LINE	  _IOW(CHAR_LCD_MAGIC, 9, uint32_t)
#define CMD_CHARLCD_REINIT		  _IO(CHAR_LCD_MAGIC, 10)

#if _WITH_LWIP_SOCKET_WRAPPER
#	ifndef FIONREAD
#		define FIONREAD _IOR('f', 127, unsigned long) /* get # bytes to read */
#	endif
#	ifndef FIONBIO
#		define FIONBIO _IOW('f', 126, unsigned long) /* set/clear non-blocking i/o */
#	endif

/* Socket I/O Controls: unimplemented */
#	ifndef SIOCSHIWAT
#		define SIOCSHIWAT _IOW('s', 0, unsigned long) /* set high watermark */
#		define SIOCGHIWAT _IOR('s', 1, unsigned long) /* get high watermark */
#		define SIOCSLOWAT _IOW('s', 2, unsigned long) /* set low watermark */
#		define SIOCGLOWAT _IOR('s', 3, unsigned long) /* get low watermark */
#		define SIOCATMARK _IOR('s', 7, unsigned long) /* at oob mark? */
#	endif

#endif
