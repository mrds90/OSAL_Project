/**
 * @file test_osal_timer.c
 * @author Marcos Dominguez
 * @brief Test module of osal timer
 * @version 0.1
 * @date 2023-12-16
 *
 * @copyright Copyright (c) 2023
 *
 */

/*========= [DEPENDENCIES] =====================================================*/

#include "unity.h"
#include "osal_timers.h"
#include "port_timers_freertos.h"
#include "FreeRTOS_timers_simulated.h"

/*========= [PRIVATE MACROS AND CONSTANTS] =====================================*/

#define TIMER_HANDLER_VALUE 0xCFA55A

/*========= [PRIVATE DATA TYPES] ===============================================*/

typedef struct {
    port_timers_handler_t handler;
    UtilsCallback_t callback;
    void *context;
} port_timer_t;

typedef enum {
    NULL_TIMER,
    NULL_HOLDER_PTR,
    NULL_CALLBACK,
    NULL_NOTHING,

    NULL_QTY
} fail_conditions_index_t;

typedef struct {
    osal_timer_t *timer;
    osal_timer_holder_t *holder_ptr;
    UtilsCallback_t callback;
} critical_ptrs_t;


/*========= [TASK DECLARATIONS] ================================================*/

/*========= [PRIVATE FUNCTION DECLARATIONS] ====================================*/

void TimerCallback(port_timers_handler_t xTimer);

/*========= [PRIVATE FUNCTION DECLARATIONS] ====================================*/

void TestTimerCallback(void *context);

/*========= [LOCAL VARIABLES] ==================================================*/

static osal_timer_t timer;
static osal_timer_holder_t holder_ptr;
static timer_index_t timer_index = TIMER_INDEX_TEST_1;
static TimerHandle_t timer_handler[TIMER_INDEX_QTY];
static uint8_t callback_calls = 0;
/*========= [SHARED VARIABLES] =================================================*/

/*========= [TEST FUNCTION IMPLEMENTATION] =====================================*/

void test_LoadStruct(void) {
    static critical_ptrs_t critical_ptrs[NULL_QTY] = {
        [NULL_TIMER] = {
            .timer = NULL,
            .callback = TestTimerCallback,
            .holder_ptr = &holder_ptr,
        },
        [NULL_HOLDER_PTR] = {
            .timer = &timer,
            .callback = TestTimerCallback,
            .holder_ptr = NULL,
        },
        [NULL_CALLBACK] = {
            .timer = &timer,
            .callback = NULL,
            .holder_ptr = &holder_ptr,
        },
        [NULL_NOTHING] = {
            .timer = &timer,
            .callback = TestTimerCallback,
            .holder_ptr = &holder_ptr,
        },
    };
    for (fail_conditions_index_t i = 0; i < NULL_QTY; i++) {
        timer.callback.callback = NULL;
        timer.callback.context = NULL;
        timer.holder_ptr = NULL;
        timer.name = "empty";
        timer.repeat = FALSE;
        timer.time = OSAL_MAX_DELAY;
        bool_t res = OSAL_TIMERS_LoadStruct(critical_ptrs[i].timer, "test_name", OSAL_MS_TO_TICKS(1000), TRUE, critical_ptrs[i].callback, (void *)100, critical_ptrs[i].holder_ptr);
        if (i == NULL_NOTHING) {
            TEST_ASSERT_EQUAL_UINT8(TRUE, res);
            TEST_ASSERT_EQUAL_PTR(TestTimerCallback, timer.callback.callback);
            TEST_ASSERT_EQUAL_PTR((void *)100, timer.callback.context);
            TEST_ASSERT_EQUAL_PTR(&holder_ptr, timer.holder_ptr);
            TEST_ASSERT_EQUAL_CHAR_ARRAY("test_name", timer.name, 10);
            TEST_ASSERT_EQUAL_UINT8(TRUE, timer.repeat);
            TEST_ASSERT_EQUAL_UINT32(OSAL_MS_TO_TICKS(1000), timer.time);
        }
        else {
            TEST_ASSERT_EQUAL_UINT8(FALSE, res);
            TEST_ASSERT_EQUAL_PTR(NULL, timer.callback.callback);
            TEST_ASSERT_EQUAL_PTR(NULL, timer.callback.context);
            TEST_ASSERT_EQUAL_PTR(NULL, timer.holder_ptr);
            TEST_ASSERT_EQUAL_CHAR_ARRAY("empty", timer.name, 6);
            TEST_ASSERT_EQUAL_UINT8(FALSE, timer.repeat);
            TEST_ASSERT_EQUAL_UINT32(OSAL_MAX_DELAY, timer.time);
        }
    }
}

void test_Create(void) {
    timer.callback.callback = NULL;
    timer.callback.context = NULL;
    timer.holder_ptr = NULL;
    timer.name = "callback_null";
    timer.repeat = FALSE;
    timer.time = OSAL_MAX_DELAY;
    /* NULL CALLBACK FAIL*/
    TEST_ASSERT_EQUAL_UINT8(FALSE, OSAL_TIMERS_Create(&timer, TIMER_INDEX_TEST_1));

    /* NULL TIMER */
    TEST_ASSERT_EQUAL_UINT8(FALSE, OSAL_TIMERS_Create(NULL, TIMER_INDEX_TEST_1));

    /* index = TIMER_INDEX_QTY */
    timer.callback.callback = TestTimerCallback;
    timer.holder_ptr = &holder_ptr;
    TEST_ASSERT_EQUAL_UINT8(FALSE, OSAL_TIMERS_Create(&timer, TIMER_INDEX_QTY));

    /* Creation success */

    TEST_ASSERT_EQUAL_UINT8(TRUE, OSAL_TIMERS_Create(&timer, TIMER_INDEX_TEST_1));
    TEST_ASSERT_EQUAL_PTR(timer_handler[TIMER_INDEX_TEST_1], timer.handler);
}

void test_Callback(void) {
    for (timer_index = TIMER_INDEX_TEST_1; timer_index < TIMER_INDEX_QTY; timer_index++) {
        OSAL_TIMERS_LoadStruct(&timer, "test_name", OSAL_MS_TO_TICKS(1000), TRUE, TestTimerCallback, (void *)timer_index, &holder_ptr);
        OSAL_TIMERS_Create(&timer, timer_index);
        TimerCallback(timer_handler[timer_index]);
    }

    /* Invalid arguments */
    TimerCallback(NULL);

    extern port_timer_t timers_active[TIMER_INDEX_QTY];
    timers_active[TIMER_INDEX_TEST_1].callback = NULL;

    TimerCallback(timer_handler[TIMER_INDEX_TEST_1]);

    TEST_ASSERT_EQUAL_UINT8(TIMER_INDEX_QTY, callback_calls);
}

/*========= [SUPPORT FUNCTION] =================================================*/
TimerHandle_t xTimerCreateStatic(const char *const pcTimerName, const TickType_t xTimerPeriodInTicks, const BaseType_t xAutoReload, void *const pvTimerID, TimerCallbackFunction_t pxCallbackFunction, StaticTimer_t *pxTimerBuffer) {
    TimerHandle_t ret = NULL;
    if ((pxCallbackFunction != NULL) && (pxTimerBuffer != NULL)) {
        timer_handler[timer_index] = (TimerHandle_t)timer_index + 1;
        ret = timer_handler[timer_index];
    }
    return ret;
}

void TestTimerCallback(void *context) {
    printf("timer_index_called: %d\n", (timer_index_t)context);
    TEST_ASSERT_EQUAL_PTR((TimerHandle_t)timer_index + 1, timer_handler[(timer_index_t)context]);
    callback_calls++;
}
