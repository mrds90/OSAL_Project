/**
 * @file port FreeRTOS_task_simulated custom.h
 * @author Marcos Dominguez
 *
 * @brief Port for FreeRTOS task API
 *
 * @version 0.1
 * @date 2023-12-11
 */

#ifndef _PORT_TASK_CUSTOM_H
#define _PORT_TASK_CUSTOM_H

#ifdef  __cplusplus
extern "C" {
#endif

/*========= [DEPENDENCIES] =====================================================*/

#include "FreeRTOS_simulated.h"

#include "utils.h"
#include "data_types.h"


/*========= [PUBLIC MACRO AND CONSTANTS] =======================================*/

#define configMAX_TASK_NAME_LEN   (35U)

#define tskIDLE_PRIORITY    (( UBaseType_t ) 0U)

/*========= [PUBLIC DATA TYPE] =================================================*/

typedef void (*TaskFunction_t)(void *);

typedef struct {
    TaskFunction_t TaskCallback;
    void *context;
} StaticTask_t;

typedef uint32_t StackType_t;

typedef StaticTask_t *TaskHandle_t;

/*========= [SHARED VARIABLES] =================================================*/

extern bool_t task_create_success;

/*========= [PUBLIC FUNCTION DECLARATIONS] =====================================*/

TaskHandle_t xTaskCreateStatic(TaskFunction_t pxTaskCode, const char *const pcName, const uint32_t ulStackDepth, void *const pvParameters, UBaseType_t uxPriority, StackType_t *const puxStackBuffer, StaticTask_t *const pxTaskBuffer);

TickType_t xTaskGetTickCount(void);

void Task_Simulated_HoldHandler(TaskHandle_t *handle_addr);

void vTaskDelay(TickType_t delay_ticks);

#ifdef  __cplusplus
}

#endif

#endif  /* _PORT_TASK_CUSTOM_H */
