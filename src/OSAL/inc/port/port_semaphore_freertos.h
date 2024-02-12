/**
 * @file port_semaphore_freertos.h
 * @author Marcos Dominguez
 *
 * @brief Port for FreeRTOS semaphore API
 *
 * @version 0.1
 * @date 2023-12-11
 */

#ifndef _PORT_SEMAPHOR_EFREERTOS_H
#define _PORT_SEMAPHOR_EFREERTOS_H

#ifdef  __cplusplus
extern "C" {
#endif

/*========= [DEPENDENCIES] =====================================================*/

#include "port_freertos.h"
#ifndef TEST
#include "semphr.h"
#else
#include "FreeRTOS_semphr_simulated.h"

#endif

#include "data_types.h"

/*========= [PUBLIC MACRO AND CONSTANTS] =======================================*/

/*========= [PUBLIC DATA TYPE] =================================================*/

typedef SemaphoreHandle_t osal_semaphore_handle_t;

typedef StaticSemaphore_t osal_semaphore_holder_t;

/*========= [PUBLIC FUNCTION DECLARATIONS] =====================================*/

#define PORT_SEMAPHORE_CreateBinary(semaphore_holder)                               xSemaphoreCreateBinaryStatic((osal_semaphore_holder_t  *) semaphore_holder)

#define PORT_SEMAPHORE_CreateMutex(semaphore_holder)                                xSemaphoreCreateMutexStatic((osal_semaphore_holder_t * ) semaphore_holder)

#define PORT_SEMAPHORE_CreateCounting(max_count, initial_count, semaphore_holder)   xSemaphoreCreateCountingStatic(max_count, initial_count, (osal_semaphore_holder_t *) semaphore_holder)

bool_t PORT_SEMAPHORE_Give(osal_semaphore_handle_t handler);

bool_t PORT_SEMAPHORE_Take(osal_semaphore_handle_t handler, port_tick_t wait_time);

bool_t PORT_SEMAPHORE_GiveFromISR(osal_semaphore_handle_t handler);

bool_t PORT_SEMAPHORE_TakeFromISR(osal_semaphore_handle_t handler);

#ifdef  __cplusplus
}

#endif

#endif  /* _PORT_SEMAPHOR_EFREERTOS_H */
