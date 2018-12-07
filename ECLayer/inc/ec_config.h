/**
 *@file		ec_config.h
 *@brief	Configurations of EC Layer.
 *@author	Eggcar
 */

#ifndef __EC_CONFIG_H
#define __EC_CONFIG_H

#define	_FILE_NAME_MAXLEN	32

#define _FD_LIST_MAXNUM		128

#define _DEV_NAME_MAXLEN	32

#define _USE_MMAP			1
#define _MMU_EXIST			0
#define _MPU_EXIST			0
#define _MMU_MPU_EXIST		_MMU_EXIST|_MPU_EXIST

#endif
