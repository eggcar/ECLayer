/**
 * @file	ec_drv_init.c
 * @brief	Initialize system device drivers.
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

#include "ec_dev.h"
#include "ec_file.h"
#include "ec_used_devices.h"

void EC_Driver_Initialize(void)
{
	device_regist(&device_stm32_usart1);
	file_regist(&driver_stm32_usart1);

	device_regist(&device_stm32_usart3);
	file_regist(&driver_stm32_usart3);

	device_regist(&device_stm32_uart4);
	file_regist(&driver_stm32_uart4);

	device_regist(&device_stm32_uart5);
	file_regist(&driver_stm32_uart5);

	device_regist(&device_stm32_usart6);
	file_regist(&driver_stm32_usart6);

	device_regist(&device_stm32_uart7);
	file_regist(&driver_stm32_uart7);

	device_regist(&device_stm32_uart8);
	file_regist(&driver_stm32_uart8);

	device_regist(&device_lcd2002);
	file_regist(&driver_lcd2002);

	return;
}
