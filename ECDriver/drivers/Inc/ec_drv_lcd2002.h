/**
 * @file    ec_drv_lcd2002.h
 * @brief   ECLayer based driver -  Port LCD2002 operations.
 * author   Eggcar
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

#include "cfifo.h"
#include "ec_dev.h"
#include "ec_file.h"
#include "ec_ioctl.h"
#include "ioctl_cmd.h"

#include <stdint.h>

typedef struct dev_lcd2002_s {
	const uint8_t *volatile lcd_base_addr;
	const uintptr_t rs_addr_mask;  // 0x0100 0000
	const uint32_t column_num;
	const uint32_t row_num;
	const uint32_t row_ddram_length;
	const uint32_t cgram_length;
	int32_t addr_type;  //<! 0 for ddram, 1 for cgram
	int32_t addr_counter;
	int32_t addr_increment;
	int32_t shift_counter;
	int32_t shift_enabled;
} dev_lcd2002_t;

int32_t lcd2002_open(file_des_t *fd, const char *filename, uint32_t flags);

int32_t lcd2002_read(file_des_t *fd, char *data, size_t count);

int32_t lcd2002_write(file_des_t *fd, const char *data, size_t count);

int32_t lcd2002_ioctl(file_des_t *fd, uint32_t cmd, uint64_t arg);

int64_t lcd2002_lseek(file_des_t *fd, int64_t offset, int32_t origin);

int32_t lcd2002_close(file_des_t *fd);
