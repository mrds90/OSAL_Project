/**
 * @file port_queue_freertos.h
 * @author Marcos Dominguez
 *
 * @brief Port for FreeRTOS queue API
 *
 * @version 0.1
 * @date 2023-11-24
 */

#ifndef _PORT_QUEUE_FREERTOS_H
#define _PORT_QUEUE_FREERTOS_H

#ifdef  __cplusplus
extern "C" {
#endif

/*========= [DEPENDENCIES] =====================================================*/
#include "port_freertos.h"
#ifndef TEST
#include "queue.h"
#else
#include "FreeRTOS_queue_simulated.h"
#endif
#include "data_types.h"
/*========= [PUBLIC MACRO AND CONSTANTS] =======================================*/

/*========= [PUBLIC DATA TYPE] =================================================*/

typedef QueueHandle_t osal_queue_handle_t;

typedef StaticQueue_t osal_queue_holder_t;

/*========= [PUBLIC FUNCTION DECLARATIONS] =====================================*/

osal_queue_handle_t PORT_QUEUE_Create(uint16_t queue_length, uint32_t data_size, uint8_t *queue_storage, osal_queue_holder_t *queue_struct);

bool_t PORT_QUEUE_Send(osal_queue_handle_t handler, void *data, port_tick_t wait_time);

bool_t PORT_QUEUE_Receive(osal_queue_handle_t handler, void *data, port_tick_t wait_time);

bool_t PORT_QUEUE_SendFromISR(osal_queue_handle_t handler, void *data, bool_t *yield_need);

bool_t PORT_QUEUE_ReceiveFromISR(osal_queue_handle_t handler, void *data, bool_t *yield_need);

#ifdef  __cplusplus
}

#endif

#endif  /* _PORT_QUEUE_FREERTOS_H */
