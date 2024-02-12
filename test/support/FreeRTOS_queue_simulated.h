/**
 * @file FreeRTOS_queue_simulated.h
 * @author Marcos Dominguez
 *
 * @brief Port for FreeRTOS queue API
 *
 * @version 0.1
 * @date 2023-11-24
 */

#ifndef _PORT_QUEUE_CUSTOM_H
#define _PORT_QUEUE_CUSTOM_H

#ifdef  __cplusplus
extern "C" {
#endif

/*========= [DEPENDENCIES] =====================================================*/

#include "FreeRTOS_simulated.h"
#include "data_types.h"

/*========= [PUBLIC MACRO AND CONSTANTS] =======================================*/

/*========= [PUBLIC DATA TYPE] =================================================*/

typedef StaticQueue_t *QueueHandle_t;

/*========= [SHARED VARIABLES] =================================================*/

extern bool_t queue_create_success;

extern bool_t queue_send_success;

extern bool_t queue_receive_success;

/*========= [PUBLIC FUNCTION DECLARATIONS] =====================================*/

QueueHandle_t xQueueCreateStatic(const UBaseType_t queue_length, const UBaseType_t data_size, uint8_t *queue_storage, StaticQueue_t *queue_struct);

UBaseType_t xQueueSend(QueueHandle_t handler, void *data, TickType_t wait_time);

UBaseType_t xQueueReceive(QueueHandle_t handler, void *data, TickType_t wait_time);

BaseType_t xQueueSendFromISR(QueueHandle_t xQueue, void *pvItemToQueue, BaseType_t *const pxHigherPriorityTaskWoken);

BaseType_t xQueueReceiveFromISR(QueueHandle_t xQueue, void *const pvBuffer, BaseType_t *const pxHigherPriorityTaskWoken);


#ifdef  __cplusplus
}

#endif

#endif  /* _PORT_QUEUE_CUSTOM_H */
