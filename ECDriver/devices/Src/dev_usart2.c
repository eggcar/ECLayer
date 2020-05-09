/**
 * @file	dev_usart2.c
 * @brief	Description of USART2 driver file.
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
#include "ec_drv_stm32_usart.h"
#include "ec_file.h"
#include "ec_lock.h"

#include <stdint.h>

static file_opts_t _opts_stm32_usart2 = {
	.open = stm32_usart_open,
	.read = stm32_usart_read,
	.write = stm32_usart_write,
	.ioctl = stm32_usart_ioctl,
	.lseek = stm32_usart_lseek,
	.mmap = NULL,
	.close = stm32_usart_close,
};

static LL_USART_InitTypeDef _init_stm32_usart2 = {
	.BaudRate = 115200,
	.DataWidth = LL_USART_DATAWIDTH_8B,
	.StopBits = LL_USART_STOPBITS_1,
	.Parity = LL_USART_PARITY_NONE,
	.TransferDirection = LL_USART_DIRECTION_TX_RX,
	.HardwareFlowControl = LL_USART_HWCONTROL_NONE,
	.OverSampling = LL_USART_OVERSAMPLING_16,
};

static LL_GPIO_InitTypeDef _init_stm32_usart2_tx = {
	.Pin = LL_GPIO_PIN_2,
	.Mode = LL_GPIO_MODE_ALTERNATE,
	.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH,
	.OutputType = LL_GPIO_OUTPUT_PUSHPULL,
	.Pull = LL_GPIO_PULL_UP,
	.Alternate = LL_GPIO_AF_7,
};

static LL_GPIO_InitTypeDef _init_stm32_usart2_rx = {
	.Pin = LL_GPIO_PIN_3,
	.Mode = LL_GPIO_MODE_ALTERNATE,
	.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH,
	.OutputType = LL_GPIO_OUTPUT_PUSHPULL,
	.Pull = LL_GPIO_PULL_UP,
	.Alternate = LL_GPIO_AF_7,
};

static config_stm32_usart_t _conf_stm32_usart2 = {
	.clock_bus = USART_Bus_APB2,
	.clock_ena_bit = LL_APB1_GRP1_PERIPH_USART2,
	.rx_buffer_size = 256,
	.tx_buffer_size = 256,
	.rx_pin_conf = &_init_stm32_usart2_rx,
	.tx_pin_conf = &_init_stm32_usart2_tx,
	.rx_io_port = GPIOA,
	.tx_io_port = GPIOA,
	.irqn = USART2_IRQn,
	.usart_conf = &_init_stm32_usart2,
};

static dev_stm32_usart_t _device_descriptor_stm32_usart2 = {
	.handle = USART2,
	.config = &_conf_stm32_usart2,
};

ec_dev_t device_stm32_usart2 = {
	.dev_name = "usart2",
	.dev_type = e_devt_STREAM,
	.private_data = (void *)(&_device_descriptor_stm32_usart2),
};

file_t driver_stm32_usart2 = {
	.file_name = "/drivers/stm32/usart2",
	.file_refs = 0,
	.file_lock = e_Unlocked,
	.file_opts = &_opts_stm32_usart2,
	.file_content = (void *)(&device_stm32_usart2),
};
