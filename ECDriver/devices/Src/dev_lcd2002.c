/**
 * @file	dev_lcd2002.c
 * @brief	Description of UART4 driver file.
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
#include "ec_drv_lcd2002.h"
#include "ec_file.h"
#include "ec_lock.h"

#include <stdint.h>

static file_opts_t _opts_lcd2002 = {
	.open = lcd2002_open,
	.read = lcd2002_read,
	.write = lcd2002_write,
	.ioctl = lcd2002_ioctl,
	.lseek = lcd2002_lseek,
	.mmap = NULL,
	.close = lcd2002_close,
};

static dev_lcd2002_t _device_descriptor_lcd2002 = {
	.lcd_base_addr = (uint8_t *)0x64000000,
	.rs_addr_mask = 0x01000000,
	.column_num = 20,
	.row_num = 2,
	.row_ddram_length = 40,
	.cgram_length = 64,
	.addr_type = 0,
	.addr_counter = 0,
	.addr_increment = 1,
	.shift_counter = 0,
	.shift_enabled = 0,
};

ec_dev_t device_lcd2002 = {
	.dev_name = "lcd2002",
	.dev_type = e_devt_STREAM,
	.private_data = (void *)(&_device_descriptor_lcd2002),
};

file_t driver_lcd2002 = {
	.file_name = "/drivers/stm32/lcd2002",
	.file_refs = 0,
	.file_lock = e_Unlocked,
	.file_opts = &_opts_lcd2002,
	.file_content = (void *)(&device_lcd2002),
};
