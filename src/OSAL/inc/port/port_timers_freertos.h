/**
 * @file port_timers_freertos.h
 * @author Marcos Dominguez
 *
 * @brief Port for FreeRTOS timers API
 *
 * @version 0.1
 * @date 2023-12-15
 */

#ifndef _PORT_TIMERS_FREERTOS_H
#define _PORT_TIMERS_FREERTOS_H

#ifdef  __cplusplus
extern "C" {
#endif

/*========= [DEPENDENCIES] =====================================================*/

#include "port_freertos.h"
#ifndef TEST
#include "timers.h"
#else
#include "FreeRTOS_timers_simulated.h"
#endif

#include "utils.h"
#include "data_types.h"
#include "timer_manager.h"

/*========= [PUBLIC MACRO AND CONSTANTS] =======================================*/

#define PORT_TIMERS_Reset(handler) xTimerReset(((port_timers_handler_t)(handler)), 50)

#define PORT_TIMERS_Start(handler) xTimerStart(((port_timers_handler_t)(handler)), 50)

#define PORT_TIMERS_Stop(handler)  xTimerStop(((port_timers_handler_t)(handler)), 50)

/*========= [PUBLIC DATA TYPE] =================================================*/

typedef StaticTimer_t port_timers_holder_t;

typedef TimerHandle_t port_timers_handler_t;

typedef struct {
    UtilsCallback_t callback;
    void *context;
} PortSWTimerCallback_t;


/*========= [PUBLIC FUNCTION DECLARATIONS] =====================================*/

port_timers_handler_t PORT_TIMER_Create(const char *name, port_tick_t time, bool_t repeat, PortSWTimerCallback_t *Callback, port_timers_holder_t *holder_ptr, timer_index_t index);

#ifdef  __cplusplus
}

#endif

#endif  /* _PORT_TIMERS_FREERTOS_H */
