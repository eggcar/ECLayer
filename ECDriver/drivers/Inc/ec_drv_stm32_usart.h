/**
 * @file	ec_drv_stm32_usart.h
 * @brief	ECLayer based driver - STM32 USART operation.
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

#ifndef __EC_DRV_STM32_USART_H
#define __EC_DRV_STM32_USART_H

#include "cfifo.h"
#include "ec_config.h"
#include "ec_dev.h"
#include "ec_file.h"
#include "ec_ioctl.h"
#include "ioctl_cmd.h"
#include "stm32f4xx_ll_bus.h"
#include "stm32f4xx_ll_gpio.h"
#include "stm32f4xx_ll_rcc.h"
#include "stm32f4xx_ll_usart.h"

#if _WITH_CMSISOS_V2
#	include "cmsis_os2.h"
#endif

#include <stdint.h>

#if _EN_USART_TIMESTAMP
#	include "systime_port.h"
#endif

#define STM32_USART_BLOCK_WITH_SCHEDULE 1

typedef struct config_stm32_usart_s {
	enum {
		USART_Bus_APB1,
		USART_Bus_APB2
	} clock_bus;
	uint32_t clock_ena_bit;
	uint32_t rx_buffer_size;
	uint32_t tx_buffer_size;
	LL_GPIO_InitTypeDef *rx_pin_conf;
	LL_GPIO_InitTypeDef *tx_pin_conf;
	GPIO_TypeDef *rx_io_port;
	GPIO_TypeDef *tx_io_port;
	IRQn_Type irqn;
	LL_USART_InitTypeDef *usart_conf;
} config_stm32_usart_t;

typedef struct dev_stm32_usart_s {
	USART_TypeDef *handle;
#if _WITH_CMSISOS_V2
	osSemaphoreId_t wr_sem;
	osSemaphoreId_t rd_sem;
	osSemaphoreId_t tx_sem;
	osSemaphoreId_t rx_sem;
#else
	ec_lock_t wr_lock;
	ec_lock_t rd_lock;
#endif
	cfifo_t *rx_buffer;
	cfifo_t *tx_buffer;
	config_stm32_usart_t *config;
#if _EN_USART_TIMESTAMP
	timeStamp_t rx_timestamp;
	timeStamp_t tx_timestamp;
	int32_t rx_ts_valid;
	int32_t tx_ts_valid;
	timeStamp_t read_timestamp;
	timeStamp_t write_timestamp;
#endif
} dev_stm32_usart_t;

int32_t stm32_usart_open(file_des_t *fd, const char *filename, uint32_t flags);

int32_t stm32_usart_read(file_des_t *fd, char *data, size_t count);

int32_t stm32_usart_write(file_des_t *fd, const char *data, size_t count);

int32_t stm32_usart_ioctl(file_des_t *fd, uint32_t cmd, uint64_t arg);

int64_t stm32_usart_lseek(file_des_t *fd, int64_t offset, int32_t origin);

int32_t stm32_usart_close(file_des_t *fd);

void ECDRV_IRQ_Handler_USART(ec_dev_t *dev);

#endif
