/**
 * @file    ec_drv_lcd2002.c
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

#include "ec_drv_lcd2002.h"

#include "cfifo.h"
#include "cmsis_os.h"
#include "ec_compiler_compat.h"
#include "ec_dev.h"
#include "ec_fcntl.h"
#include "ec_file.h"
#include "exceptions.h"

#include <stdint.h>

#define __CMD_CLEAR (0x01U)
#define __CMD_HOME (0x02U)
#define __CMD_ENTRY (0x04U)
#define __ENTRY_ID (0x02U)
#define __ENTRY_S (0x01U)
#define __CMD_DISPLAY (0x08U)
#define __DISPLAY_D (0x04U)
#define __DISPLAY_C (0x02U)
#define __DISPLAY_B (0x01U)
#define __CMD_SHIFT (0x10U)
#define __SHIFT_SC (0x08U)
#define __SHIFT_RL (0x04U)
#define __CMD_FUNCTION (0x20U)
#define __FUNCTION_DL (0x10U)
#define __FUNCTION_N (0x08U)
#define __CMD_SETCGRAM (0x40U)
#define __CMD_SETDDRAM (0x80U)

static void __init_lcd2002(dev_lcd2002_t *dev);

int32_t lcd2002_open(file_des_t *fd, const char *filename, uint32_t flags)
{
	dev_lcd2002_t *lcd_dev = (dev_lcd2002_t *)(((ec_dev_t *)(fd->file->file_content))->private_data);
	if (fd == NULL) {
		return -EBADFD;
	}
	else {
		if (atomic_inc_and_eq(&(fd->file->file_refs), 1) == 0) {
			__init_lcd2002(lcd_dev);
			return 0;
		}
		else {
			atomic_dec(&(fd->file->file_refs));
			return -EBUSY;
		}
	}
}

int32_t lcd2002_read(file_des_t *fd, char *data, size_t count)
{
	return -ENOTSUP;
}

int32_t lcd2002_write(file_des_t *fd, const char *data, size_t count)
{
	file_t *file;
	ec_dev_t *dev;
	dev_lcd2002_t *lcd_dev;

	if (fd == NULL) {
		return -EBADFD;
	}
	else {
		file = (file_t *)fd->file;
	}

	if (file == NULL) {
		return -EBADF;
	}
	else {
		dev = (ec_dev_t *)file->file_content;
	}

	if (dev == NULL) {
		return -ENODEV;
	}
	else {
		lcd_dev = (dev_lcd2002_t *)dev->private_data;
	}

	if (lcd_dev == NULL) {
		return -ENODEV;
	}
	else {
	}

	if ((fd->file_flags & O_WRONLY) == 0) {
		return -ENOTSUP;
	}
	else {
	}

	uint32_t _to_be_write;
	volatile uint8_t *data_base = (uint8_t *)lcd_dev->lcd_base_addr + (lcd_dev->rs_addr_mask);
	_to_be_write = (count < lcd_dev->row_ddram_length) ? count : lcd_dev->row_ddram_length;
	EC_OPTIMIZE_ENTER
	EC_OPTIMIZE_O0
	// Put the if statement outside decrease some cost in the for loop.
	if (lcd_dev->shift_enabled) {
		for (int i = 0; i < _to_be_write; i++) {
			data_base[0] = data[i];
			lcd_dev->addr_counter += lcd_dev->addr_increment;
			lcd_dev->shift_counter += lcd_dev->addr_increment;
		}
	}
	else {
		for (int i = 0; i < _to_be_write; i++) {
			data_base[0] = data[i];
			lcd_dev->addr_counter += lcd_dev->addr_increment;
		}
	}
	EC_OPTIMIZE_EXIT
	return 0;
}

int32_t lcd2002_ioctl(file_des_t *fd, uint32_t cmd, uint64_t arg)
{
	file_t *file;
	ec_dev_t *dev;
	dev_lcd2002_t *lcd_dev;

	if (fd == NULL) {
		return -EBADFD;
	}
	else {
		file = (file_t *)fd->file;
	}

	if (file == NULL) {
		return -EBADF;
	}
	else {
		dev = (ec_dev_t *)file->file_content;
	}

	if (dev == NULL) {
		return -ENODEV;
	}
	else {
		lcd_dev = (dev_lcd2002_t *)dev->private_data;
	}

	if (lcd_dev == NULL) {
		return -ENODEV;
	}
	else {
	}

	volatile uint8_t *cmd_base = (uint8_t *)(lcd_dev->lcd_base_addr);

	EC_OPTIMIZE_ENTER
	EC_OPTIMIZE_O0
	switch (cmd) {
	case CMD_CHARLCD_CLEAR:
		cmd_base[0] = __CMD_CLEAR;  // Clear screen
		lcd_dev->addr_counter = 0;
		lcd_dev->addr_increment = 1;
		lcd_dev->shift_counter = 0;
		break;
	case CMD_CHARLCD_HOME:
		cmd_base[0] = __CMD_HOME;
		lcd_dev->addr_counter = 0;
		lcd_dev->shift_counter = 0;
		break;
	case CMD_CHARLCD_ENTRYMODE:
		cmd_base[0] = __CMD_ENTRY | ((uint8_t)(arg & 0x03));
		if (arg & 0x02) {
			lcd_dev->addr_increment = 1;
		}
		else {
			lcd_dev->addr_increment = -1;
		}
		if (arg & 0x01) {
			lcd_dev->shift_enabled = 1;
		}
		else {
			lcd_dev->shift_enabled = 0;
		}
		break;
	case CMD_CHARLCD_DISPLAY:
		cmd_base[0] = __CMD_DISPLAY | ((uint8_t)(arg & 0x07));
		break;
	case CMD_CHARLCD_SHIFT:
		cmd_base[0] = __CMD_SHIFT | ((uint8_t)(arg & 0x0C));
		if (arg & 0x04) {
			lcd_dev->addr_counter += 1;
			if (arg & 0x08) {
				lcd_dev->shift_counter += 1;
			}
		}
		else {
			lcd_dev->addr_counter -= 1;
			if (arg & 0x08) {
				lcd_dev->shift_counter -= 1;
			}
		}
		break;
	case CMD_CHARLCD_FUNCTION:
		/* Set to 4-bit bus mode is not permitted in this driver. */
		/* Set to 1 line display mode is not permitted in this driver. */
		cmd_base[0] = __CMD_FUNCTION | __FUNCTION_DL | __FUNCTION_N | (uint8_t)(arg & 0x03);
		break;
	case CMD_CHARLCD_SET_CGRAMADDR:
		lcd_dev->addr_type = 1;
		lcd_dev->addr_counter = (uint8_t)(arg & 0x3F);
		cmd_base[0] = __CMD_SETCGRAM | lcd_dev->addr_counter;
		break;
	case CMD_CHARLCD_SET_DDRAMADDR:
		lcd_dev->addr_type = 0;
		lcd_dev->addr_counter = (uint8_t)(arg & 0x7F);
		cmd_base[0] = __CMD_SETDDRAM | lcd_dev->addr_counter;
		break;
	case CMD_CHARLCD_CURSOR2LINE:
		switch (arg) {
		case 0:
			lcd_dev->addr_type = 0;
			lcd_dev->addr_counter = 0x00;
			cmd_base[0] = __CMD_SETDDRAM | lcd_dev->addr_counter;
			break;
		case 1:
			lcd_dev->addr_type = 0;
			lcd_dev->addr_counter = 0x40;
			cmd_base[0] = __CMD_SETDDRAM | lcd_dev->addr_counter;
			break;
		default:
			lcd_dev->addr_type = 0;
			lcd_dev->addr_counter = 0x00;
			cmd_base[0] = __CMD_SETDDRAM | lcd_dev->addr_counter;
			break;
		}
		break;
	case CMD_CHARLCD_REINIT:
		__init_lcd2002(lcd_dev);
		break;
	default:
		break;
	}
	EC_OPTIMIZE_EXIT
	return 0;
}

int64_t lcd2002_lseek(file_des_t *fd, int64_t offset, int32_t origin)
{
	file_t *file;
	ec_dev_t *dev;
	dev_lcd2002_t *lcd_dev;

	if (fd == NULL) {
		return -EBADFD;
	}
	else {
		file = (file_t *)fd->file;
	}

	if (file == NULL) {
		return -EBADF;
	}
	else {
		dev = (ec_dev_t *)file->file_content;
	}

	if (dev == NULL) {
		return -ENODEV;
	}
	else {
		lcd_dev = (dev_lcd2002_t *)dev->private_data;
	}

	if (lcd_dev == NULL) {
		return -ENODEV;
	}
	else {
	}

	volatile uint8_t *cmd_base = (uint8_t *)(lcd_dev->lcd_base_addr);
	int64_t _calc_addr;

	switch (origin) {
	case EC_SEEK_SET:
		_calc_addr = (offset > 0) ? offset : 0;
		break;
	case EC_SEEK_CUR:
		_calc_addr = lcd_dev->addr_counter + offset;
		_calc_addr = (_calc_addr > 0) ? _calc_addr : 0;
		break;
	case EC_SEEK_END:
		/** Well it's difficult to define "End" for lcd interface. 
		 *  So let us just ignore it.
		*/
		_calc_addr = lcd_dev->addr_counter;
		break;
	default:
		break;
	}
	EC_OPTIMIZE_ENTER
	EC_OPTIMIZE_O0
	if (lcd_dev->addr_type == 0) {
		lcd_dev->addr_counter = (uint8_t)(offset & 0x7F);
		cmd_base[0] = __CMD_SETDDRAM | lcd_dev->addr_counter;
	}
	else {
		lcd_dev->addr_counter = (uint8_t)(offset & 0x3F);
		cmd_base[0] = __CMD_SETCGRAM | lcd_dev->addr_counter;
	}
	EC_OPTIMIZE_EXIT
	return 0;
}

int32_t lcd2002_close(file_des_t *fd)
{
	dev_lcd2002_t *lcd_dev = (dev_lcd2002_t *)(((ec_dev_t *)(fd->file->file_content))->private_data);
	(void)lcd_dev;
	if (fd == NULL) {
		return -EBADFD;
	}
	else {
		if (atomic_dec_and_test(&(fd->file->file_refs)) == 0) {
			return 0;
		}
		else {
			atomic_inc(&(fd->file->file_refs));
			return 0;
		}
	}
}

void __init_lcd2002(dev_lcd2002_t *lcd_dev)
{
	EC_OPTIMIZE_ENTER
	EC_OPTIMIZE_O0
	volatile uint8_t *cmd_base = (uint8_t *)(lcd_dev->lcd_base_addr);
	osDelay(15);
	cmd_base[0] = __CMD_FUNCTION | __FUNCTION_DL | __FUNCTION_N;  // 8bit interface | display line 2 | luminance 100%
	osDelay(2);
	cmd_base[0] = __CMD_CLEAR;  // Clear screen, set AC to ddram 0x00
	osDelay(2);
	cmd_base[0] = __CMD_HOME;  // Set cursor to home, set AC to ddram 0x00
	lcd_dev->addr_type = 0;
	lcd_dev->addr_counter = 0;
	lcd_dev->shift_counter = 0;
	osDelay(2);
	cmd_base[0] = __CMD_ENTRY | __ENTRY_ID;  // ID = 1 -> AC increase 1 after each write | S = 0 -> disable shift function
	lcd_dev->addr_increment = 1;
	lcd_dev->shift_enabled = 0;
	cmd_base[0] = __CMD_DISPLAY | __DISPLAY_D;  // D = 1 -> Enable display | C = 0 -> Cursor off | B = 0 -> blink off
	osDelay(1);
	return;
	EC_OPTIMIZE_EXIT
}
