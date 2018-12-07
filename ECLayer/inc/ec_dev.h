/**
  *@file	ec_dev.h
  *@brief	Device structure definition file.
  *@author	Eggcar
  *@date	2017.08.16 
  */
#ifndef __EC_DEV_H
#define __EC_DEV_H

#include "ec_config.h"
#include "ec_list.h"

#include <stddef.h>
#include <stdint.h>

typedef enum {
	e_devt_STREAM = 1,
	e_devt_GPIO,
	e_devt_STORAGE,
	e_devt_NETWORK,
	e_devt_FILE,
	e_devt_MMAP
} ec_dev_type_t;

typedef struct ec_dev_s {
	char dev_name[_DEV_NAME_MAXLEN + 1];
	ec_dev_type_t dev_type;
	list_t dev_list;
	void *private_data;
} ec_dev_t;

int32_t device_regist(ec_dev_t *dev);

int32_t device_deregist(ec_dev_t *dev);

ec_dev_t *search_device(const char *device_name);

#endif
