/**
 * @file FreeRTOS_timers_simulated.h
 * @author Marcos Dominguez
 *
 * @brief Port for Custom timers API
 *
 * @version 0.1
 * @date 2023-12-15
 */

#ifndef _PORT_TIMERS_CUSTOM_H
#define _PORT_TIMERS_CUSTOM_H

#ifdef  __cplusplus
extern "C" {
#endif

/*========= [DEPENDENCIES] =====================================================*/

#include "utils.h"
#include "data_types.h"
#include "FreeRTOS_simulated.h"
#include "timer_manager.h"

/*========= [PUBLIC MACRO AND CONSTANTS] =======================================*/

#define configTIMER_TASK_STACK_DEPTH            256

/*========= [PUBLIC DATA TYPE] =================================================*/

typedef timer_index_t StaticTimer_t;

typedef StaticTimer_t *TimerHandle_t;

typedef void (*TimerCallbackFunction_t)(TimerHandle_t xTimer);

/*========= [PUBLIC FUNCTION DECLARATIONS] =====================================*/

TimerHandle_t xTimerCreateStatic(const char *const pcTimerName, const TickType_t xTimerPeriodInTicks, const BaseType_t xAutoReload, void *const pvTimerID, TimerCallbackFunction_t pxCallbackFunction, StaticTimer_t *pxTimerBuffer);

BaseType_t xTimerReset(TimerHandle_t xTimer, TickType_t xTicksToWait);

BaseType_t xTimerStart(TimerHandle_t xTimer, TickType_t xTicksToWait);

BaseType_t xTimerStop(TimerHandle_t xTimer, TickType_t xTicksToWait);

#ifdef  __cplusplus
}

#endif

#endif  /* _PORT_TIMERS_CUSTOM_H */
