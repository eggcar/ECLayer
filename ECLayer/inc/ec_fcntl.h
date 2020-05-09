/**
 * @file	ec_fcntl.h
 * @brief	File flag control
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

#ifndef __EC_FCNTL_H
#define __EC_FCNTL_H

#define O_RDONLY	(0x00000001U)
#define O_WRONLY	(0x00000002U)
#define O_RDWR		(0x00000003U)
#define O_APPEND	(0x00000004U)
#define O_CREAT		(0x00000008U)
#define O_EXEC		(0x00000010U)
#define O_NOBLOCK	(0x00000020U)
#define O_TRUNC		(0x00000040U)


#define F_GETFL		(3)
#define F_SETFL		(4)

#define EC_SEEK_SET    (0)
#define EC_SEEK_CUR    (1)
#define EC_SEEK_END    (2)

#endif
