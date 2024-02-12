/**
 * @file test_osal_task.c
 * @author Marcos Dominguez
 * @brief Test module of osal task
 * @version 0.1
 * @date 2023-12-19
 *
 * @copyright Copyright (c) 2023
 *
 */

/*========= [DEPENDENCIES] =====================================================*/

#include "unity.h"
#include "osal_task.h"
#include "port_task_freertos.h"
#include "FreeRTOS_task_simulated.h"

/*========= [PRIVATE MACROS AND CONSTANTS] =====================================*/

#define TEST_STACK_SIZE  (4 * OSAL_ATOMIC_TASK_SIZE)
#define TEST_TASK_HANDLER ((TaskHandle_t)100)
/*========= [PRIVATE DATA TYPES] ===============================================*/

typedef enum {
    TEST_CONDITION_TASK_PTR_NULL,
    TEST_CONDITION_STACK_PTR_NULL,
    TEST_CONDITION_TASK_HOLDER_PTR_NULL,
    TEST_CONDITION_FUNCTION_NULL,
    TEST_CONDITION_SIZE_0,
    TEST_CONDITION_SUCCESS,

    TEST_CONDITION_QTY,
} test_condition_t;


typedef struct {
    osal_task_t *task_ptr;
    osal_stack_holder_t *stack_holder_ptr;
    osal_task_holder_t *task_holder_ptr;
    uint16_t size;
    OSAL_TASK_Callback_t function;
} test_osal_task_t;

/*========= [TASK DECLARATIONS] ================================================*/

/*========= [PRIVATE FUNCTION DECLARATIONS] ====================================*/

/*========= [PRIVATE FUNCTION DECLARATIONS] ====================================*/

void TaskFunction(void *context);

/*========= [LOCAL VARIABLES] ==================================================*/

static osal_task_t task = {.task_handler = NULL};
static osal_stack_holder_t stack_holder[TEST_STACK_SIZE];
static osal_task_holder_t task_holder;
static uint16_t size = TEST_STACK_SIZE;
static uint8_t tick_calls = 0;

static test_osal_task_t test_osal_task[TEST_CONDITION_QTY] = {
    [TEST_CONDITION_TASK_PTR_NULL] = {
        .task_ptr = NULL,
        .stack_holder_ptr = stack_holder,
        .task_holder_ptr = &task_holder,
        .size = TEST_STACK_SIZE,
        .function = TaskFunction,
    },
    [TEST_CONDITION_STACK_PTR_NULL] = {
        .task_ptr = &task,
        .stack_holder_ptr = NULL,
        .task_holder_ptr = &task_holder,
        .size = TEST_STACK_SIZE,
        .function = TaskFunction,
    },
    [TEST_CONDITION_TASK_HOLDER_PTR_NULL] = {
        .task_ptr = &task,
        .stack_holder_ptr = stack_holder,
        .task_holder_ptr = NULL,
        .size = TEST_STACK_SIZE,
        .function = TaskFunction,
    },
    [TEST_CONDITION_FUNCTION_NULL] = {
        .task_ptr = &task,
        .stack_holder_ptr = stack_holder,
        .task_holder_ptr = &task_holder,
        .size = TEST_STACK_SIZE,
        .function = NULL,
    },
    [TEST_CONDITION_SIZE_0] = {
        .task_ptr = &task,
        .stack_holder_ptr = stack_holder,
        .task_holder_ptr = &task_holder,
        .size = 0,
        .function = TaskFunction,
    },
    [TEST_CONDITION_SUCCESS] = {
        .task_ptr = &task,
        .stack_holder_ptr = stack_holder,
        .task_holder_ptr = &task_holder,
        .size = TEST_STACK_SIZE,
        .function = TaskFunction,
    },
};

static test_condition_t condition;

/*========= [SHARED VARIABLES] =================================================*/

/*========= [TEST FUNCTION IMPLEMENTATION] =====================================*/

void test_LoadStruct(void) {
    for (condition = TEST_CONDITION_TASK_PTR_NULL; condition < TEST_CONDITION_QTY; condition++) {
        task.size = 0;
        task.stack_ptr = NULL;
        task.task_hold_ptr = NULL;
        bool_t res = OSAL_TASK_LoadStruct(test_osal_task[condition].task_ptr, test_osal_task[condition].stack_holder_ptr, test_osal_task[condition].task_holder_ptr, test_osal_task[condition].size);
        if ((condition == TEST_CONDITION_SUCCESS) || (condition == TEST_CONDITION_FUNCTION_NULL)) {
            TEST_ASSERT_EQUAL_UINT8(TRUE, res);
            TEST_ASSERT_EQUAL_UINT8(task.size, TEST_STACK_SIZE);
            TEST_ASSERT_EQUAL_PTR(task.stack_ptr, stack_holder);
            TEST_ASSERT_EQUAL_PTR(task.task_hold_ptr, &task_holder);
        }
        else {
            TEST_ASSERT_EQUAL_UINT8(FALSE, res);
            TEST_ASSERT_EQUAL_UINT8(task.size, 0);
            TEST_ASSERT_EQUAL_PTR(task.stack_ptr, NULL);
            TEST_ASSERT_EQUAL_PTR(task.task_hold_ptr, NULL);
        }
    }
}

void test_Create(void) {
    for (condition = TEST_CONDITION_TASK_PTR_NULL; condition < TEST_CONDITION_QTY; condition++) {
        for (osal_task_priority_t priority = (TASK_PRIORITY_IDLE); priority <= TASK_PRIORITY_QTY; priority++) {
            if (condition == TEST_CONDITION_SIZE_0) {
                task.size = 0;
            }
            else {
                task.size = TEST_STACK_SIZE;
            }
            bool_t res = OSAL_TASK_Create(test_osal_task[condition].task_ptr, test_osal_task[condition].function, NULL, priority);
            if ((priority >= TASK_PRIORITY_IDLE) && (priority < TASK_PRIORITY_QTY)) {
                switch (condition) {
                    case TEST_CONDITION_TASK_PTR_NULL:
                        TEST_ASSERT_EQUAL_UINT8(FALSE, res);

                        break;

                    case TEST_CONDITION_TASK_HOLDER_PTR_NULL:
                        TEST_ASSERT_EQUAL_UINT8(FALSE, res);

                        break;

                    case TEST_CONDITION_FUNCTION_NULL:
                        TEST_ASSERT_EQUAL_UINT8(FALSE, res);

                        break;

                    case TEST_CONDITION_SIZE_0:
                        TEST_ASSERT_EQUAL_UINT8(FALSE, res);

                        break;

                    case TEST_CONDITION_SUCCESS:
                        TEST_ASSERT_EQUAL_UINT8(TRUE, res);
                        TEST_ASSERT_EQUAL_PTR(TEST_TASK_HANDLER, task.task_handler);
                        break;
                }
            }
            else {
                TEST_ASSERT_EQUAL_UINT8(FALSE, res);
            }
        }
    }
    TEST_ASSERT_EQUAL_UINT8(TASK_PRIORITY_QTY, tick_calls);
}

/*========= TEST_STACK_SIZE,[SUPPORT FUNCTION] =================================================*/

TaskHandle_t xTaskCreateStatic(TaskFunction_t pxTaskCode, const char *const pcName, const uint32_t ulStackDepth, void *const pvParameters, UBaseType_t uxPriority, StackType_t *const puxStackBuffer, StaticTask_t *const pxTaskBuffer) {
    if ((condition == TEST_CONDITION_TASK_HOLDER_PTR_NULL) || (condition == TEST_CONDITION_STACK_PTR_NULL))
        return NULL;
    else {
        tick_calls = xTaskGetTickCount();
        return TEST_TASK_HANDLER;
    }
}

void TaskFunction(void *context) {}
