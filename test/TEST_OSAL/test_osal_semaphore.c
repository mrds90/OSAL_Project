/**
 * @file test_osal_semaphore.c
 * @author Marcos Dominguez
 * @brief Test module of osal semaphore
 * @version 0.1
 * @date 2023-12-19
 *
 * @copyright Copyright (c) 2023
 *
 */

/*========= [DEPENDENCIES] =====================================================*/

#include "unity.h"
#include "osal_semaphore.h"
#include "port_semaphore_freertos.h"
#include "FreeRTOS_semphr_simulated.h"
#include "FreeRTOS_queue_simulated.h"


/*========= [PRIVATE MACROS AND CONSTANTS] =====================================*/

#define MAX_COUNT_IN_TEST           10

#define INIT_COUNT_IN_TEST          5

#define HANDLER_VALUE_MUTEX         ((SemaphoreHandle_t)(1))
#define HANDLER_VALUE_BINARY        ((SemaphoreHandle_t)(10))
#define HANDLER_VALUE_COUNT         ((SemaphoreHandle_t)(100))

/*========= [PRIVATE DATA TYPES] ===============================================*/

typedef struct {
    osal_semaphore_t *semaphore_ptr;
    osal_semaphore_holder_t *holder_ptr;
    uint16_t max_count;
    uint16_t initial_count;
    bool_t mutex_type;
} test_osal_semaphore_t;


typedef enum {
    TEST_OSAL_NULL_SEMAPHORE_PTR,
    TEST_OSAL_NULL_HOLDER_HANDLER_PTR,
    TEST_OSAL_MUTEX_TYPE_TRUE,
    TEST_OSAL_BINARY,
    TEST_OSAL_COUNT,

    TEST_OSAL_QTY,
} test_osal_semaphore_cases_t;

typedef enum {
    TEST_OSAL_HANDLER_MUTEX,
    TEST_OSAL_HANDLER_BINARY,
    TEST_OSAL_HANDLER_COUNT,

    TEST_OSAL_HANDLER_QTY,
} test_osal_handler_t;

/*========= [TASK DECLARATIONS] ================================================*/

/*========= [PRIVATE FUNCTION DECLARATIONS] ====================================*/

/*========= [PRIVATE FUNCTION DECLARATIONS] ====================================*/

/*========= [LOCAL VARIABLES] ==================================================*/

static osal_semaphore_t semaphore;

static osal_semaphore_holder_t semaphore_holder;

static test_osal_semaphore_t test_osal_semaphore[TEST_OSAL_QTY] = {
    [TEST_OSAL_NULL_SEMAPHORE_PTR] = {
        .semaphore_ptr = NULL,
        .holder_ptr = &semaphore_holder,
        .max_count = 1,
        .initial_count = 0,
        .mutex_type = FALSE,
    },
    [TEST_OSAL_NULL_HOLDER_HANDLER_PTR] = {
        .semaphore_ptr = &semaphore,
        .holder_ptr = NULL,
        .max_count = 1,
        .initial_count = 0,
        .mutex_type = FALSE,
    },
    [TEST_OSAL_MUTEX_TYPE_TRUE] = {
        .semaphore_ptr = &semaphore,
        .holder_ptr = &semaphore_holder,
        .max_count = 1,
        .initial_count = 0,
        .mutex_type = TRUE,
    },
    [TEST_OSAL_BINARY] = {
        .semaphore_ptr = &semaphore,
        .holder_ptr = &semaphore_holder,
        .max_count = 1,
        .initial_count = 0,
        .mutex_type = FALSE,
    },
    [TEST_OSAL_COUNT] = {
        .semaphore_ptr = &semaphore,
        .holder_ptr = &semaphore_holder,
        .max_count = MAX_COUNT_IN_TEST,
        .initial_count = INIT_COUNT_IN_TEST,
        .mutex_type = FALSE,
    },
};

static test_osal_semaphore_cases_t test_case;

static uint16_t data_to_send;

static uint16_t data_to_receive;

static SemaphoreHandle_t handlers[TEST_OSAL_HANDLER_QTY] = {
    [TEST_OSAL_HANDLER_MUTEX] = HANDLER_VALUE_MUTEX,
    [TEST_OSAL_HANDLER_BINARY] = HANDLER_VALUE_BINARY,
    [TEST_OSAL_HANDLER_COUNT] = HANDLER_VALUE_COUNT,
};

static UBaseType_t isr_semph_given = pdFALSE;
static UBaseType_t semph_given = pdFALSE;

/*========= [SHARED VARIABLES] =================================================*/

/*========= [TEST FUNCTION IMPLEMENTATION] =====================================*/

void test_LoadStruct(void) {
    for (test_case = TEST_OSAL_NULL_SEMAPHORE_PTR; test_case < TEST_OSAL_QTY; test_case++) {
        bool_t res;
        res = OSAL_SEMAPHORE_LoadStruct(test_osal_semaphore[test_case].semaphore_ptr, test_osal_semaphore[test_case].holder_ptr, test_osal_semaphore[test_case].max_count, test_osal_semaphore[test_case].initial_count, test_osal_semaphore[test_case].mutex_type);
        switch (test_case) {
            case TEST_OSAL_NULL_SEMAPHORE_PTR:
            case TEST_OSAL_NULL_HOLDER_HANDLER_PTR:
                TEST_ASSERT_EQUAL_UINT8(FALSE, res);
                break;

            case TEST_OSAL_MUTEX_TYPE_TRUE:
                TEST_ASSERT_EQUAL_UINT8(TRUE, res);
                TEST_ASSERT_EQUAL_PTR(NULL, semaphore.handler);
                TEST_ASSERT_EQUAL_PTR(&semaphore_holder, semaphore.holder_ptr);
                TEST_ASSERT_EQUAL_UINT16(0, semaphore.max_count);
                TEST_ASSERT_EQUAL_UINT16(0, semaphore.initial_count);
                TEST_ASSERT_EQUAL_UINT8(TRUE, semaphore.mutex_type);
                break;

            case TEST_OSAL_BINARY:
                TEST_ASSERT_EQUAL_UINT8(TRUE, res);
                TEST_ASSERT_EQUAL_PTR(NULL, semaphore.handler);
                TEST_ASSERT_EQUAL_PTR(&semaphore_holder, semaphore.holder_ptr);
                TEST_ASSERT_EQUAL_UINT16(1, semaphore.max_count);
                TEST_ASSERT_EQUAL_UINT16(0, semaphore.initial_count);
                TEST_ASSERT_EQUAL_UINT8(FALSE, semaphore.mutex_type);
                break;

            case TEST_OSAL_COUNT:
                TEST_ASSERT_EQUAL_UINT8(TRUE, res);
                TEST_ASSERT_EQUAL_UINT8(TRUE, res);
                TEST_ASSERT_EQUAL_PTR(NULL, semaphore.handler);
                TEST_ASSERT_EQUAL_PTR(&semaphore_holder, semaphore.holder_ptr);
                TEST_ASSERT_EQUAL_UINT16(MAX_COUNT_IN_TEST, semaphore.max_count);
                TEST_ASSERT_EQUAL_UINT16(INIT_COUNT_IN_TEST, semaphore.initial_count);
                TEST_ASSERT_EQUAL_UINT8(FALSE, semaphore.mutex_type);
                break;

            default:
                break;
        }
    }
}

void test_Create(void) {
    for (test_case = TEST_OSAL_NULL_SEMAPHORE_PTR; test_case < TEST_OSAL_QTY; test_case++) {
        bool_t res;
        switch (test_case) {
            case TEST_OSAL_NULL_SEMAPHORE_PTR:
                res =  OSAL_SEMAPHORE_Create(NULL);
                TEST_ASSERT_EQUAL_UINT8(FALSE, res);


                break;

            case TEST_OSAL_NULL_HOLDER_HANDLER_PTR:
                semaphore.holder_ptr = NULL;
                res =  OSAL_SEMAPHORE_Create(&semaphore);
                TEST_ASSERT_EQUAL_UINT8(FALSE, res);
                break;

            case TEST_OSAL_MUTEX_TYPE_TRUE:
                semaphore.holder_ptr = &semaphore_holder;
                semaphore.mutex_type = TRUE;
                res =  OSAL_SEMAPHORE_Create(&semaphore);
                TEST_ASSERT_EQUAL_UINT8(TRUE, res);
                TEST_ASSERT_EQUAL_PTR(handlers[TEST_OSAL_HANDLER_MUTEX], semaphore.handler);
                break;

            case TEST_OSAL_BINARY:
                semaphore.holder_ptr = &semaphore_holder;
                semaphore.mutex_type = FALSE;
                semaphore.max_count = 1;
                res =  OSAL_SEMAPHORE_Create(&semaphore);
                TEST_ASSERT_EQUAL_UINT8(TRUE, res);
                TEST_ASSERT_EQUAL_PTR(handlers[TEST_OSAL_HANDLER_BINARY], semaphore.handler);
                break;

            case TEST_OSAL_COUNT:
                semaphore.holder_ptr = &semaphore_holder;
                semaphore.mutex_type = FALSE;
                semaphore.max_count = MAX_COUNT_IN_TEST;
                semaphore.initial_count = INIT_COUNT_IN_TEST;
                res =  OSAL_SEMAPHORE_Create(&semaphore);
                TEST_ASSERT_EQUAL_UINT8(TRUE, res);
                TEST_ASSERT_EQUAL_PTR(handlers[TEST_OSAL_HANDLER_COUNT], semaphore.handler);
                break;
        }
    }
}

void test_TakeGive(void) {
    for (test_case = TEST_OSAL_NULL_SEMAPHORE_PTR; test_case < TEST_OSAL_QTY; test_case++) {
        bool_t res;
        switch (test_case) {
            case TEST_OSAL_NULL_SEMAPHORE_PTR:
                semaphore.handler = NULL;
                res =  OSAL_SEMAPHORE_TakeFromISR(&semaphore);
                TEST_ASSERT_EQUAL_UINT8(FALSE, res);
                res =  OSAL_SEMAPHORE_GiveFromISR(&semaphore);
                TEST_ASSERT_EQUAL_UINT8(FALSE, res);
                res =  OSAL_SEMAPHORE_TakeFromISR(&semaphore);
                TEST_ASSERT_EQUAL_UINT8(FALSE, res);
                res =  OSAL_SEMAPHORE_Take(&semaphore, OSAL_MS_TO_TICKS(100));
                TEST_ASSERT_EQUAL_UINT8(FALSE, res);
                res =  OSAL_SEMAPHORE_Give(&semaphore);
                TEST_ASSERT_EQUAL_UINT8(FALSE, res);
                res =  OSAL_SEMAPHORE_Take(&semaphore, OSAL_MS_TO_TICKS(100));
                TEST_ASSERT_EQUAL_UINT8(FALSE, res);
                break;

            case TEST_OSAL_NULL_HOLDER_HANDLER_PTR:
                semaphore.handler = handlers[TEST_OSAL_HANDLER_BINARY];

                res =  OSAL_SEMAPHORE_TakeFromISR(NULL);
                TEST_ASSERT_EQUAL_UINT8(FALSE, res);
                res =  OSAL_SEMAPHORE_GiveFromISR(NULL);
                TEST_ASSERT_EQUAL_UINT8(FALSE, res);
                res =  OSAL_SEMAPHORE_TakeFromISR(NULL);
                TEST_ASSERT_EQUAL_UINT8(FALSE, res);
                res =  OSAL_SEMAPHORE_Take(NULL, OSAL_MS_TO_TICKS(100));
                TEST_ASSERT_EQUAL_UINT8(FALSE, res);
                res =  OSAL_SEMAPHORE_Give(NULL);
                TEST_ASSERT_EQUAL_UINT8(FALSE, res);
                res =  OSAL_SEMAPHORE_Take(NULL, OSAL_MS_TO_TICKS(100));
                TEST_ASSERT_EQUAL_UINT8(FALSE, res);
                break;

            case TEST_OSAL_MUTEX_TYPE_TRUE:
                semaphore.handler = handlers[TEST_OSAL_HANDLER_MUTEX];
                res =  OSAL_SEMAPHORE_TakeFromISR(&semaphore);
                TEST_ASSERT_EQUAL_UINT8(FALSE, res);
                res =  OSAL_SEMAPHORE_GiveFromISR(&semaphore);
                TEST_ASSERT_EQUAL_UINT8(TRUE, res);
                res =  OSAL_SEMAPHORE_TakeFromISR(&semaphore);
                TEST_ASSERT_EQUAL_UINT8(TRUE, res);
                res =  OSAL_SEMAPHORE_Take(&semaphore, OSAL_MS_TO_TICKS(100));
                TEST_ASSERT_EQUAL_UINT8(FALSE, res);
                res =  OSAL_SEMAPHORE_Give(&semaphore);
                TEST_ASSERT_EQUAL_UINT8(TRUE, res);
                res =  OSAL_SEMAPHORE_Take(&semaphore, OSAL_MS_TO_TICKS(100));
                TEST_ASSERT_EQUAL_UINT8(TRUE, res);
                break;

            case TEST_OSAL_BINARY:
                semaphore.handler = handlers[TEST_OSAL_HANDLER_BINARY];
                res =  OSAL_SEMAPHORE_TakeFromISR(&semaphore);
                TEST_ASSERT_EQUAL_UINT8(FALSE, res);
                res =  OSAL_SEMAPHORE_GiveFromISR(&semaphore);
                TEST_ASSERT_EQUAL_UINT8(TRUE, res);
                res =  OSAL_SEMAPHORE_TakeFromISR(&semaphore);
                TEST_ASSERT_EQUAL_UINT8(TRUE, res);
                res =  OSAL_SEMAPHORE_Take(&semaphore, OSAL_MS_TO_TICKS(100));
                TEST_ASSERT_EQUAL_UINT8(FALSE, res);
                res =  OSAL_SEMAPHORE_Give(&semaphore);
                TEST_ASSERT_EQUAL_UINT8(TRUE, res);
                res =  OSAL_SEMAPHORE_Take(&semaphore, OSAL_MS_TO_TICKS(100));
                TEST_ASSERT_EQUAL_UINT8(TRUE, res);
                break;

            case TEST_OSAL_COUNT:
                semaphore.handler = handlers[TEST_OSAL_HANDLER_COUNT];
                res =  OSAL_SEMAPHORE_TakeFromISR(&semaphore);
                TEST_ASSERT_EQUAL_UINT8(FALSE, res);
                res =  OSAL_SEMAPHORE_GiveFromISR(&semaphore);
                TEST_ASSERT_EQUAL_UINT8(TRUE, res);
                res =  OSAL_SEMAPHORE_TakeFromISR(&semaphore);
                TEST_ASSERT_EQUAL_UINT8(TRUE, res);
                res =  OSAL_SEMAPHORE_Take(&semaphore, OSAL_MS_TO_TICKS(100));
                TEST_ASSERT_EQUAL_UINT8(FALSE, res);
                res =  OSAL_SEMAPHORE_Give(&semaphore);
                TEST_ASSERT_EQUAL_UINT8(TRUE, res);
                res =  OSAL_SEMAPHORE_Take(&semaphore, OSAL_MS_TO_TICKS(100));
                TEST_ASSERT_EQUAL_UINT8(TRUE, res);
                break;
        }
    }
}

/*========= [SUPPORT FUNCTION] =================================================*/

SemaphoreHandle_t xSemaphoreCreateBinaryStatic(StaticSemaphore_t *pxSemaphoreBuffer) {
    return handlers[TEST_OSAL_HANDLER_BINARY];
}

SemaphoreHandle_t xSemaphoreCreateMutexStatic(StaticSemaphore_t *pxSemaphoreBuffer) {
    return handlers[TEST_OSAL_HANDLER_MUTEX];
}

SemaphoreHandle_t xSemaphoreCreateCountingStatic(UBaseType_t uxMaxCount, UBaseType_t uxInitialCount, StaticSemaphore_t *pxSemaphoreBuffer) {
    return handlers[TEST_OSAL_HANDLER_COUNT];
}

BaseType_t xSemaphoreGive(SemaphoreHandle_t xSemaphore) {
    semph_given = pdTRUE;
    return semph_given;
}

BaseType_t xSemaphoreTake(SemaphoreHandle_t xSemaphore, TickType_t wait_time) {
    BaseType_t ret = semph_given;
    semph_given = pdFALSE;
    return ret;
}

BaseType_t xSemaphoreGiveFromISR(SemaphoreHandle_t handler, BaseType_t *HigherPriorityTaskWoken) {
    isr_semph_given = pdTRUE;
    *HigherPriorityTaskWoken = isr_semph_given;
    return isr_semph_given;
}

BaseType_t xSemaphoreTakeFromISR(SemaphoreHandle_t handler, BaseType_t *HigherPriorityTaskWoken) {
    BaseType_t ret = isr_semph_given;
    isr_semph_given = pdFALSE;
    *HigherPriorityTaskWoken = ret;
    return ret;
}
