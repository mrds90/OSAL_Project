/**
 * @file FreeRTOS_semphr_simulated.h
 * @author Marcos Dominguez
 *
 * @brief Port for FreeRTOS semaphore API
 *
 * @version 0.1
 * @date 2023-12-11
 */

#ifndef _PORT_SEMAPHORE_CUSTOM_H
#define _PORT_SEMAPHORE_CUSTOM_H

#ifdef  __cplusplus
extern "C" {
#endif

/*========= [DEPENDENCIES] =====================================================*/

#include "data_types.h"
#include "FreeRTOS_simulated.h"

/*========= [PUBLIC MACRO AND CONSTANTS] =======================================*/

#define MAX_CONT_SEMPHR     0xFF

/*========= [PUBLIC DATA TYPE] =================================================*/

typedef struct {
    uint8_t buffer[MAX_CONT_SEMPHR + 1];
    StaticQueue_t holder;
} StaticSemaphore_t;

typedef StaticSemaphore_t *SemaphoreHandle_t;

/*========= [PUBLIC FUNCTION DECLARATIONS] =====================================*/

SemaphoreHandle_t xSemaphoreCreateBinaryStatic(StaticSemaphore_t *pxSemaphoreBuffer);

SemaphoreHandle_t xSemaphoreCreateMutexStatic(StaticSemaphore_t *pxSemaphoreBuffer);

SemaphoreHandle_t xSemaphoreCreateCountingStatic(UBaseType_t uxMaxCount, UBaseType_t uxInitialCount, StaticSemaphore_t *pxSemaphoreBuffer);

BaseType_t xSemaphoreGive(SemaphoreHandle_t xSemaphore);

BaseType_t xSemaphoreTake(SemaphoreHandle_t xSemaphore, TickType_t wait_time);

BaseType_t xSemaphoreGiveFromISR(SemaphoreHandle_t handler, BaseType_t *HigherPriorityTaskWoken);

BaseType_t xSemaphoreTakeFromISR(SemaphoreHandle_t handler, BaseType_t *HigherPriorityTaskWoken);

#ifdef  __cplusplus
}

#endif

#endif  /* _PORT_SEMAPHORE_CUSTOM_H */
