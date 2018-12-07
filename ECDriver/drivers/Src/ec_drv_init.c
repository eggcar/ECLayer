/**
 * @file	ec_drv_init.c
 * @brief	Initialize system device drivers.
 * @author	Eggcar
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
