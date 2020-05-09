/**
 * @file    ec_used_devices.h
 * @brief   Header file that includes all used device description files in this project.
 * @author  Eggcar
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

// clang-format off
#define _DEV_INCLUDE(x)	#x
#define DEV_INCLUDE(x)	_DEV_INCLUDE(../../devices/Inc/x.h)
// clang-format on

// User defined device descriptor files.
#include DEV_INCLUDE(dev_uart4)
#include DEV_INCLUDE(dev_uart5)
#include DEV_INCLUDE(dev_uart7)
#include DEV_INCLUDE(dev_uart8)
#include DEV_INCLUDE(dev_usart1)
#include DEV_INCLUDE(dev_usart2)
#include DEV_INCLUDE(dev_usart3)
#include DEV_INCLUDE(dev_usart6)
#include DEV_INCLUDE(dev_lcd2002)
