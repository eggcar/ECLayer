/**
 *@file		ec_fdlist.h
 *@brief	File descriptor list.
 *@author	Eggcar
 */
#ifndef __EC_FDLIST_H
#define __EC_FDLIST_H

#include "ec_file.h"

#include <stdint.h>

file_des_t *get_fd_struct(int32_t fd);

int32_t alloc_fd(file_des_t *fs);

int32_t free_fd(int32_t fd);

#endif
