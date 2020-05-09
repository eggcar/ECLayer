/**
 * @file	ec_drv_stm32_usart.c
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

#include "ec_drv_stm32_usart.h"

#include "cfifo.h"
#include "ec_config.h"
#include "ec_dev.h"
#include "ec_fcntl.h"
#include "ec_file.h"
#include "exceptions.h"
#include "stm32f4xx_ll_usart.h"

#if _EN_USART_TIMESTAMP
#	include "systime_port.h"
#endif

#if _WITH_CMSISOS_V2
#	include "cmsis_os2.h"
#endif

#include <stdint.h>
#include <string.h>

static void __init_stm32_usart(dev_stm32_usart_t *dev, uint32_t flags);
static void __disable_stm32_usart(dev_stm32_usart_t *dev);
static uint32_t __get_stm32_usart_periphclk(dev_stm32_usart_t *usart_dev);

int32_t stm32_usart_open(file_des_t *fd, const char *filename, uint32_t flags)
{
	dev_stm32_usart_t *usart_dev = (dev_stm32_usart_t *)(((ec_dev_t *)(fd->file->file_content))->private_data);
	int32_t err;
	if (fd == NULL) {
		return -EBADFD;
	}
	else {
		if (atomic_inc_and_eq(&(fd->file->file_refs), 1) == 0) {
			/**
			 * Read-only or write-only configuration not supported yet.
			*/
			usart_dev->rx_buffer = cfifo_new(usart_dev->config->rx_buffer_size);
			if (usart_dev->rx_buffer == NULL) {
				err = -ENOMEM;
				goto release_file_refs;
			}
			usart_dev->tx_buffer = cfifo_new(usart_dev->config->tx_buffer_size);
			if (usart_dev->tx_buffer == NULL) {
				err = -ENOMEM;
				goto release_rx_buffer;
			}
#if _WITH_CMSISOS_V2
			usart_dev->wr_sem = osSemaphoreNew(1, 1, NULL);
			if (usart_dev->wr_sem == NULL) {
				err = -ENOMEM;
				goto release_tx_buffer;
			}

			usart_dev->rd_sem = osSemaphoreNew(1, 1, NULL);
			if (usart_dev->rd_sem == NULL) {
				err = -ENOMEM;
				goto release_wr_sem;
			}

			usart_dev->rx_sem = osSemaphoreNew(1, 1, NULL);
			if (usart_dev->rx_sem == NULL) {
				err = -ENOMEM;
				goto release_rd_sem;
			}

			usart_dev->tx_sem = osSemaphoreNew(1, 1, NULL);
			if (usart_dev->tx_sem == NULL) {
				err = -ENOMEM;
				goto release_rx_sem;
			}
#else
			usart_dev->wr_lock = e_Unlocked;
			usart_dev->rd_lock = e_Unlocked;
#endif
			__init_stm32_usart(usart_dev, flags);
			return 0;
		}
		else {
			/**
			 * Multiple open not supported for USART yet.
			*/
			err = -EBUSY;
			goto release_file_refs;
		}
	}
#if _WITH_CMSISOS_V2
release_rx_sem:
	osSemaphoreDelete(usart_dev->rx_sem);
release_rd_sem:
	osSemaphoreDelete(usart_dev->rd_sem);
release_wr_sem:
	osSemaphoreDelete(usart_dev->wr_sem);
#endif
release_tx_buffer:
	cfifo_delete(usart_dev->rx_buffer);
release_rx_buffer:
	cfifo_delete(usart_dev->rx_buffer);
release_file_refs:
	atomic_dec(&(fd->file->file_refs));
	return err;
}

int32_t stm32_usart_read(file_des_t *fd, char *data, size_t count)
{
	file_t *file;
	ec_dev_t *dev;
	dev_stm32_usart_t *usart_dev;

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
		usart_dev = (dev_stm32_usart_t *)dev->private_data;
	}

	if (usart_dev == NULL) {
		return -ENODEV;
	}
	else {
	}

	if ((fd->file_flags & O_RDONLY) == 0) {
		return -ENOTSUP;
	}
	else {
	}

	size_t rest_count = count;
	int32_t err;

#if _WITH_CMSISOS_V2
	if (__get_IPSR() != 0) {
		if (osSemaphoreAcquire(usart_dev->rd_sem, 0) != osOK) {
			return -EBUSY;
		}
	}
	else {
		if (fd->file_flags & O_NOBLOCK) {
			if (osSemaphoreAcquire(usart_dev->rd_sem, 0) != osOK) {
				return -EBUSY;
			}
		}
		else {
			if (osSemaphoreAcquire(usart_dev->rd_sem, osWaitForever) != osOK) {
				// unlikely
				return -ENOLCK;
			}
		}
	}
#else
	if (ec_try_lock(&(usart_dev->rd_lock)) == -EBUSY) {
		if (__get_IPSR() != 0) {
			// If we are in ISR, we should never block.
			return -EBUSY;
		}
		else {
			/**
				 * @todo Coroutine or something else for no-os configuration.
				 * */
			return (fd->file_flags & O_NOBLOCK) ? -EBUSY : -ENOTSUP;
		}
	}
#endif
	if (fd->file_flags & O_NOBLOCK) {
		err = cfifo_popn(usart_dev->rx_buffer, data, count);
	}
	else {
		while (rest_count > 0) {
			err = cfifo_popn(usart_dev->rx_buffer, &data[count - rest_count], rest_count);
			if (err < 0) {
#if _WITH_CMSISOS_V2 && STM32_USART_BLOCK_WITH_SCHEDULE
				if (osSemaphoreAcquire(usart_dev->rx_sem, osWaitForever) != osOK) {
					// Semaphore is not active or something else is wrong.
					// Normally unreachable.
					return -ENOLCK;
				}
#else
				/**
				 * @todo Coroutine or something else for no-os configuration.
				*/
#endif
			}
			else {
				rest_count -= err;
			}
		}
		err = count;
	}
#if _EN_USART_TIMESTAMP
	if (err > 0) {
		memcpy(&(usart_dev->read_timestamp), &(usart_dev->rx_timestamp), sizeof(timeStamp_t));
		usart_dev->rx_ts_valid = 0;
	}
#endif

#if _WITH_CMSISOS_V2
	osSemaphoreRelease(usart_dev->rd_sem);
#else
	ec_unlock(&(usart_dev->rd_lock));
#endif
	return err;
}

int32_t stm32_usart_write(file_des_t *fd, const char *data, size_t count)
{
	file_t *file;
	ec_dev_t *dev;
	dev_stm32_usart_t *usart_dev;

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
		usart_dev = (dev_stm32_usart_t *)dev->private_data;
	}

	if (usart_dev == NULL) {
		return -ENODEV;
	}
	else {
	}

	if ((fd->file_flags & O_WRONLY) == 0) {
		return -ENOTSUP;
	}
	else {
	}

	size_t rest_count = count;
	int32_t err;

#if _WITH_CMSISOS_V2
	if (__get_IPSR() != 0) {
		if (osSemaphoreAcquire(usart_dev->wr_sem, 0) != osOK) {
			return -EBUSY;
		}
	}
	else {
		if (fd->file_flags & O_NOBLOCK) {
			if (osSemaphoreAcquire(usart_dev->wr_sem, 0) != osOK) {
				return -EBUSY;
			}
		}
		else {
			if (osSemaphoreAcquire(usart_dev->wr_sem, osWaitForever) != osOK) {
				// unlikely
				return -ENOLCK;
			}
		}
	}
#else
	if (ec_try_lock(&(usart_dev->wr_lock)) == -EBUSY) {
		if (__get_IPSR() != 0) {
			// If we are in ISR, we should never block.
			return -EBUSY;
		}
		else {
			/**
				 * @todo Coroutine or something else for no-os configuration.
				 * */
			return (fd->file_flags & O_NOBLOCK) ? -EBUSY : -ENOTSUP;
		}
	}
#endif
	if (fd->file_flags & O_NOBLOCK) {
		err = cfifo_pushn(usart_dev->tx_buffer, data, count);
		LL_USART_EnableIT_TXE(usart_dev->handle);
	}
	else {
		while (rest_count > 0) {
			err = cfifo_pushn(usart_dev->tx_buffer, &data[count - rest_count], rest_count);
			LL_USART_EnableIT_TXE(usart_dev->handle);
			if (err < 0) {
#if _WITH_CMSISOS_V2 && STM32_USART_BLOCK_WITH_SCHEDULE
				if (osSemaphoreAcquire(usart_dev->tx_sem, osWaitForever) != osOK) {
					// Semaphore is not active or something else is wrong.
					// Normally unreachable.
					return -ENOLCK;
				}
#else
				/**
				 * @todo Coroutine or something else for no-os configuration.
				*/
#endif
			}
			else {
				rest_count -= err;
			}
		}
		err = count;
	}
#if _EN_USART_TIMESTAMP
	memcpy(&(usart_dev->write_timestamp), &(usart_dev->tx_timestamp), sizeof(timeStamp_t));
	usart_dev->tx_ts_valid = 0;
#endif

#if _WITH_CMSISOS_V2
	osSemaphoreRelease(usart_dev->wr_sem);
#else
	ec_unlock(&(usart_dev->wr_lock));
#endif
	return err;
}

int32_t stm32_usart_ioctl(file_des_t *fd, uint32_t cmd, uint64_t arg)
{
	file_t *file;
	ec_dev_t *dev;
	dev_stm32_usart_t *usart_dev;

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
		usart_dev = (dev_stm32_usart_t *)dev->private_data;
	}

	if (usart_dev == NULL) {
		return -ENODEV;
	}
	else {
	}

	switch (cmd) {
	case CMD_USART_SETBAUD: {
		uint32_t periphclk;
		uint32_t oversampling;
		uint32_t baudrate;
		LL_USART_DisableIT_RXNE(usart_dev->handle);
		LL_USART_Disable(usart_dev->handle);
		periphclk = __get_stm32_usart_periphclk(usart_dev);
		oversampling = LL_USART_GetOverSampling(usart_dev->handle);
		baudrate = (uint32_t)(arg & 0xffffffffU);
		usart_dev->config->usart_conf->BaudRate = baudrate;
		LL_USART_SetBaudRate(usart_dev->handle, periphclk, oversampling, baudrate);
		LL_USART_Enable(usart_dev->handle);
		LL_USART_EnableIT_RXNE(usart_dev->handle);
		break;
	}
	case CMD_USART_GETBAUD: {
		uint32_t periphclk;
		uint32_t oversampling;
		uint32_t baudrate;
		uint32_t *rtval;
		rtval = (uint32_t *)((uintptr_t)arg & 0xffffffffU);
		periphclk = __get_stm32_usart_periphclk(usart_dev);
		oversampling = LL_USART_GetOverSampling(usart_dev->handle);
		baudrate = LL_USART_GetBaudRate(usart_dev->handle, periphclk, oversampling);
		*rtval = baudrate;
		break;
	}
	case CMD_USART_ENABLE: {
		LL_USART_Enable(usart_dev->handle);
		LL_USART_EnableIT_RXNE(usart_dev->handle);
		break;
	}
	case CMD_USART_DISABLE: {
		LL_USART_DisableIT_RXNE(usart_dev->handle);
		LL_USART_Disable(usart_dev->handle);
		break;
	}
#if _EN_USART_TIMESTAMP
	case CMD_USART_GETREADTS: {
		timeStamp_t *dest = (timeStamp_t *)((uintptr_t)arg & 0xffffffffU);
		memcpy(dest, &(usart_dev->read_timestamp), sizeof(timeStamp_t));
		break;
	}
	case CMD_USART_GETWRITETS: {
		timeStamp_t *dest = (timeStamp_t *)((uintptr_t)arg & 0xffffffffU);
		memcpy(dest, &(usart_dev->write_timestamp), sizeof(timeStamp_t));
		break;
	}
#endif
	default:
		return -EBADCMD;
	}
	return 0;
}

int64_t stm32_usart_lseek(file_des_t *fd, int64_t offset, int32_t origin)
{
	return -ENOTSUP;
}

int32_t stm32_usart_close(file_des_t *fd)
{
	dev_stm32_usart_t *usart_dev = (dev_stm32_usart_t *)(((ec_dev_t *)(fd->file->file_content))->private_data);
	if (fd == NULL) {
		return -EBADFD;
	}
	else {
		if (atomic_dec_and_test(&(fd->file->file_refs)) == 0) {
			__disable_stm32_usart(usart_dev);
			atomic_inc(&(fd->file->file_refs));
			cfifo_delete(usart_dev->rx_buffer);
			cfifo_delete(usart_dev->tx_buffer);
#if _WITH_CMSISOS_V2
			osSemaphoreDelete(usart_dev->rd_sem);
			osSemaphoreDelete(usart_dev->wr_sem);
			osSemaphoreDelete(usart_dev->rx_sem);
			osSemaphoreDelete(usart_dev->tx_sem);
#endif
			return 0;
		}
		else {
			atomic_inc(&(fd->file->file_refs));
			return 0;
		}
	}
}

void ECDRV_IRQ_Handler_USART(ec_dev_t *dev)
{
	dev_stm32_usart_t *dev_usart = (dev_stm32_usart_t *)dev->private_data;
	USART_TypeDef *husart = dev_usart->handle;
	int32_t err;
	char ch, discard;

#if _EN_USART_TIMESTAMP
	timeStamp_t tmp_ts;
	ECTimeStamp(&tmp_ts);
#endif

	if (dev == NULL) {
		return;
	}

	if (dev_usart == NULL) {
		return;
	}

	if (husart == NULL) {
		return;
	}

	if (LL_USART_IsActiveFlag_RXNE(husart)) {
		if (dev_usart->rx_buffer == NULL) {
			return;
		}
		else {
			ch = LL_USART_ReceiveData8(husart);
			while (cfifo_push(dev_usart->rx_buffer, ch) == -EFIFOFULL) {
				err = cfifo_pop(dev_usart->rx_buffer, &discard);
			}
#if _EN_USART_TIMESTAMP
			if (!dev_usart->rx_ts_valid) {
				memcpy(&(dev_usart->rx_timestamp), &tmp_ts, sizeof(timeStamp_t));
				dev_usart->rx_ts_valid = 1;
			}
#endif
		}
		osSemaphoreRelease(dev_usart->rx_sem);
	}

	if (LL_USART_IsEnabledIT_TXE(husart)) {
		if (LL_USART_IsActiveFlag_TXE(husart)) {
			if (dev_usart->tx_buffer == NULL) {
				LL_USART_DisableIT_TXE(husart);
				return;
			}
			else {
				err = cfifo_pop(dev_usart->tx_buffer, &ch);
				if (err == -EFIFOEMPTY) {
					LL_USART_DisableIT_TXE(husart);
					// Transmit end callback add here
				}
				else {
#if _EN_USART_TIMESTAMP
					if (!dev_usart->tx_ts_valid) {
						ECTimeStamp(&(dev_usart->tx_timestamp));
						dev_usart->tx_ts_valid = 1;
					}
#endif
					LL_USART_TransmitData8(husart, ch);
				}
			}
			osSemaphoreRelease(dev_usart->tx_sem);
		}
	}
}

static void __init_stm32_usart(dev_stm32_usart_t *usart_dev, uint32_t flags)
{
	// Peripheral clock enable
	switch (usart_dev->config->clock_bus) {
	case USART_Bus_APB1:
		LL_APB1_GRP1_EnableClock(usart_dev->config->clock_ena_bit);
		break;
	case USART_Bus_APB2:
		LL_APB2_GRP1_EnableClock(usart_dev->config->clock_ena_bit);
		break;
	default:
		return;
	}
	// Config peripheral GPIO
	LL_GPIO_Init(usart_dev->config->rx_io_port, usart_dev->config->rx_pin_conf);
	LL_GPIO_Init(usart_dev->config->tx_io_port, usart_dev->config->tx_pin_conf);
	// Config peripheral IRQn
	NVIC_SetPriority(usart_dev->config->irqn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 5, 0));
	NVIC_EnableIRQ(usart_dev->config->irqn);
	// USART configuration
	LL_USART_Init(usart_dev->handle, usart_dev->config->usart_conf);
	// Currently, we only use USART ports as asynchronize mode.
	// You could add synchronize mode support by yourself.
	LL_USART_ConfigAsyncMode(usart_dev->handle);
	// Enable peripheral
	LL_USART_Enable(usart_dev->handle);
	// Enable 'receive not empty' interrupt
	LL_USART_EnableIT_RXNE(usart_dev->handle);
	return;
}

static void __disable_stm32_usart(dev_stm32_usart_t *usart_dev)
{
	// Should we disable the peripheral clock here?
	// Not sure about it. Should check if this clock is shared with other periphs.

	// Disable 'receive not empty' interrupt
	LL_USART_DisableIT_RXNE(usart_dev->handle);
	// Disable peripheral
	LL_USART_Disable(usart_dev->handle);
	// Disable peripheral IRQn
	NVIC_DisableIRQ(usart_dev->config->irqn);
	return;
}

uint32_t __get_stm32_usart_periphclk(dev_stm32_usart_t *usart_dev)
{
	uint32_t periphclk;
	LL_RCC_ClocksTypeDef rcc_clocks;
	LL_RCC_GetSystemClocksFreq(&rcc_clocks);
	switch (usart_dev->config->clock_bus) {
	case USART_Bus_APB1:
		periphclk = rcc_clocks.PCLK1_Frequency;
		break;
	case USART_Bus_APB2:
		periphclk = rcc_clocks.PCLK2_Frequency;
		break;
	default:
		periphclk = LL_RCC_PERIPH_FREQUENCY_NO;
		break;
	}
	return periphclk;
}
