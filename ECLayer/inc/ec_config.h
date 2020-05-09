/**
 * @file	ec_config.h
 * @brief	Configurations of EC Layer.
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

#ifndef __EC_CONFIG_H
#define __EC_CONFIG_H

#define	_FILE_NAME_MAXLEN	32

#define _FD_LIST_MAXNUM		128

#define _DEV_NAME_MAXLEN	32

#define _USE_MMAP			0
#define _MMU_EXIST			0
#define _MPU_EXIST			0
#define _MMU_MPU_EXIST		_MMU_EXIST|_MPU_EXIST

#define _EN_USART_TIMESTAMP	1

#define _WITH_CMSISOS_V2	1

#define _WITH_LWIP_SOCKET_WRAPPER	1

#if _WITH_LWIP_SOCKET_WRAPPER
#define _LWIP_SOCKET_HEADER_FILE	"lwip/sockets.h"
#define _LWIP_OPT_FILE				"lwip/opt.h"
#endif

#endif
