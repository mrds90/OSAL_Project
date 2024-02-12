/**
 * @file port_task_freertos.h
 * @author Marcos Dominguez
 *
 * @brief Port for FreeRTOS task API
 *
 * @version 0.1
 * @date 2023-11-24
 */

#ifndef _PORT_TASK_FREERTOS_H
#define _PORT_TASK_FREERTOS_H

#ifdef  __cplusplus
extern "C" {
#endif

/*========= [DEPENDENCIES] =====================================================*/

#ifndef TEST
#include "FreeRTOS.h"
#include "task.h"
#else
#include "FreeRTOS_task_simulated.h"
#endif

/*========= [PUBLIC MACRO AND CONSTANTS] =======================================*/

#define OSAL_TASK_Delay(delay_ticks)     vTaskDelay(delay_ticks)

#define OSAL_MAX_TASK_NAME_LEN           configMAX_TASK_NAME_LEN

/*========= [PUBLIC DATA TYPE] =================================================*/

typedef TaskHandle_t osal_task_handler_t;

typedef StaticTask_t osal_task_holder_t;

typedef StackType_t osal_stack_holder_t;

typedef TaskFunction_t OSAL_TASK_Callback_t;

#define OSAL_TASK_GetTickCount()    xTaskGetTickCount()

/*========= [PUBLIC FUNCTION DECLARATIONS] =====================================*/

osal_task_handler_t PORT_TASK_CreateStaticTask(OSAL_TASK_Callback_t function, char *name, uint16_t size, void *context, uint8_t priority, osal_stack_holder_t *stack_ptr, osal_task_holder_t *task_hold_ptr);

#ifdef  __cplusplus
}

#endif

#endif  /* _PORT_TASK_FREERTOS_H */
