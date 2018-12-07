/**
 *@file		ec_api.h
 *@brief	Basic interface for ECLayer users.
 *@author	Eggcar
 */

#ifndef __EC_API_H
#define __EC_API_H

#include "ec_fcntl.h"
#include "ec_mmap.h"
#include "ioctl_cmd.h"
#include "heap_port.h"

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

int32_t open(const char *filename, uint32_t flags);

int32_t close(int32_t fd);

int32_t write(int32_t fd, const char *data, size_t length);

int32_t read(int32_t fd, char *data, size_t length);

int32_t ioctl(int32_t fd, uint32_t cmd, uint64_t arg);

int64_t lseek(int32_t fd, int64_t offset, int32_t origin);

#endif
