/**
 *@file		ec_fcntl.h
 *@brief	File flag control
 *@author	Eggcar
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

#define EC_SEEK_SET    (0)
#define EC_SEEK_CUR    (1)
#define EC_SEEK_END    (2)

#endif
