/**
 * @file	systime_port.h
 * @brief	Portable header file for systime api.
 * @author	Eggcar
 */

#pragma once

#include "ec_lock.h"

#include <stdint.h>

/* Begin include user-defined header. */

/* End include user-defined header. */

/* Begin user-defined macros */

/* End user-defined macros */

/* Begin user-defined variable. */

/* End user-defined variable. */

typedef struct timeStamp_s {
	uint64_t ts_sec;
	uint32_t ts_ns;
} timeStamp_t;

static inline void ECTimeStamp(timeStamp_t *ts)
{
	/* Begin portable GetTimeStamp function body */

	/* End portable GetTimeStamp function body */
}

/* This is an example of the API implementation */

//	#include "ec_lock.h"
//	
//	#include <stdint.h>
//	
//	/* Begin include user-defined header. */
//	#include "global_time.h"
//	#include "stm32f4xx.h"
//	#include "time_base.h"
//	/* End include user-defined header. */
//	
//	/* Begin user-defined macros */
//	#define __TIMER_FREQ (90000000)
//	/* End user-defined macros */
//	
//	/* Begin user-defined variable. */
//	extern TIM_HandleTypeDef htim2;
//	extern localTime_t GlobalTime;
//	/* End user-defined variable. */
//	
//	typedef struct timeStamp_s {
//		uint64_t ts_sec;
//		uint32_t ts_ns;
//	} timeStamp_t;
//	
//	static inline void ECTimeStamp(timeStamp_t *ts)
//	{
//		/* Begin portable GetTimeStamp function body */
//		uint32_t irq_flag;
//		while (ec_try_lock_irqsave(&GlobalTimeLock, &irq_flag) != 0)
//			;
//		ts->ts_ns = ((uint64_t)__HAL_TIM_GET_COUNTER(&htim2)) * 100 / 9;  // Simplified calculation (cnt * 10^9 / __TIMER_FREQ)
//		ts->ts_sec = GlobalTime.tai_Second;
//		ec_unlock_irqrestore(&GlobalTimeLock, irq_flag);
//		/* End portable GetTimeStamp function body */
//	}