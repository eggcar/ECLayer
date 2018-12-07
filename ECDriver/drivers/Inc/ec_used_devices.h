/**
 * @file    ec_used_devices.h
 * @brief   Header file that includes all used device description files in this project.
 * @author  Eggcar
*/

#pragma once

// clang-format off
#define __DEV_INCLUDE(x)	#x
#define _DEV_INCLUDE(x)		__DEV_INCLUDE(../../devices/Inc/##x.h)
#define DEV_INCLUDE(x)		_DEV_INCLUDE(x)
// clang-format on

// User defined device descriptor files.
#include DEV_INCLUDE(dev_uart4)
#include DEV_INCLUDE(dev_uart5)
#include DEV_INCLUDE(dev_uart7)
#include DEV_INCLUDE(dev_uart8)
#include DEV_INCLUDE(dev_usart1)
#include DEV_INCLUDE(dev_usart3)
#include DEV_INCLUDE(dev_usart6)
#include DEV_INCLUDE(dev_lcd2002)
