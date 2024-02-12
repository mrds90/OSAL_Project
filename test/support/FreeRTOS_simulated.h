/**
 * @file FreeRTOS_simulated.h
 * @author Marcos Dominguez
 *
 * @brief Port for FreeRTOS top API
 *
 * @version 0.1
 * @date 2023-11-24
 */

#ifndef _PORT_CUSTOM_H
#define _PORT_CUSTOM_H

#ifdef  __cplusplus
extern "C" {
#endif

/*========= [DEPENDENCIES] =====================================================*/

#include <stdint.h>
#include <stdio.h>

/*========= [PUBLIC MACRO AND CONSTANTS] =======================================*/

typedef uint32_t TickType_t;

typedef uint32_t UBaseType_t;

typedef int32_t BaseType_t;

#define portMAX_DELAY                     ((TickType_t) 0xFFFFFFFF)

#define pdMS_TO_TICKS(ms)                 (ms)

/*  the __builtin_return_address(0) function is used to determine the return address, and if it's NULL, it is likely that you are inside a void function.*/
#define configASSERT(x)                   if ((x) == 0) {printf("%s - line: %d\n", __FILE__, __LINE__); return FALSE;}


#define configMINIMAL_STACK_SIZE          (32)
#define portEND_SWITCHING_ISR(condition)  (condition)

#ifndef pdFALSE
#define pdFALSE                                  ((BaseType_t) 0)
#endif
#ifndef pdTRUE
#define pdTRUE                                   ((BaseType_t) 1)
#endif
#ifndef pdPASS
#define pdPASS                                   (pdTRUE)
#endif
#ifndef pdFAIL
#define pdFAIL                                   (pdFALSE)
#endif

/*========= [PUBLIC DATA TYPE] =================================================*/

typedef struct {
    uint8_t *queue_storage;
    uint8_t *pop_ptr;
    uint8_t *push_ptr;
    uint16_t queue_length;
    uint32_t data_size;
    uint16_t used_elements;
} StaticQueue_t;

/*========= [PUBLIC FUNCTION DECLARATIONS] =====================================*/

#ifdef  __cplusplus
}

#endif

#endif  /* _PORT_CUSTOM_H */
