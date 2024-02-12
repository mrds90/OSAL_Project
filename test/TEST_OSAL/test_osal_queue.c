/**
 * @file test_osal_queue.c
 * @author Marcos Dominguez
 * @brief Test module of osal queue
 * @version 0.1
 * @date 2023-12-19
 *
 * @copyright Copyright (c) 2023
 *
 */

/*========= [DEPENDENCIES] =====================================================*/

#include "unity.h"
#include "osal_queue.h"
#include "port_queue_freertos.h"
#include "FreeRTOS_queue_simulated.h"

/*========= [PRIVATE MACROS AND CONSTANTS] =====================================*/

#define TEST_OSAL_QUEUE_LENGTH   10

/*========= [PRIVATE DATA TYPES] ===============================================*/

typedef struct {
    osal_queue_t *queue_ptr;
    osal_queue_holder_t *holder_ptr;
    uint8_t *queue_storage;
    uint32_t data_size;
    uint16_t queue_length;
} test_osal_queue_t;

typedef enum {
    TEST_OSAL_NULL_QUEUE_PTR,
    TEST_OSAL_NULL_HOLDER_HANDLER_PTR,
    TEST_OSAL_NULL_QUEUE_STORAGE,
    TEST_OSAL_DATA_SIZE_0,
    TEST_OSAL_QUEUE_LENGTH_0,
    TEST_OSAL_SUCCESS,

    TEST_OSAL_QTY,
} test_osal_queue_cases_t;

/*========= [TASK DECLARATIONS] ================================================*/

/*========= [PRIVATE FUNCTION DECLARATIONS] ====================================*/

/*========= [PRIVATE FUNCTION DECLARATIONS] ====================================*/

/*========= [LOCAL VARIABLES] ==================================================*/

static osal_queue_t queue;

static osal_queue_holder_t queue_holder;

static uint8_t queue_buffer[TEST_OSAL_QUEUE_LENGTH];

static test_osal_queue_t test_osal_queue[TEST_OSAL_QTY] = {
    [TEST_OSAL_NULL_QUEUE_PTR] = {
        .data_size = sizeof(uint16_t),
        .holder_ptr = &queue_holder,
        .queue_length = TEST_OSAL_QUEUE_LENGTH,
        .queue_ptr = NULL,
        .queue_storage = queue_buffer,
    },
    [TEST_OSAL_NULL_HOLDER_HANDLER_PTR] = {
        .data_size = sizeof(uint16_t),
        .holder_ptr = NULL,
        .queue_length = TEST_OSAL_QUEUE_LENGTH,
        .queue_ptr = &queue,
        .queue_storage = queue_buffer,
    },
    [TEST_OSAL_NULL_QUEUE_STORAGE] = {
        .data_size = sizeof(uint16_t),
        .holder_ptr = &queue_holder,
        .queue_length = TEST_OSAL_QUEUE_LENGTH,
        .queue_ptr = &queue,
        .queue_storage = NULL,
    },
    [TEST_OSAL_DATA_SIZE_0] = {
        .data_size = 0,
        .holder_ptr = &queue_holder,
        .queue_length = TEST_OSAL_QUEUE_LENGTH,
        .queue_ptr = &queue,
        .queue_storage = queue_buffer,
    },
    [TEST_OSAL_QUEUE_LENGTH_0] = {
        .data_size = sizeof(uint16_t),
        .holder_ptr = &queue_holder,
        .queue_length = 0,
        .queue_ptr = &queue,
        .queue_storage = queue_buffer,
    },
    [TEST_OSAL_SUCCESS] = {
        .data_size = sizeof(uint16_t),
        .holder_ptr = &queue_holder,
        .queue_length = TEST_OSAL_QUEUE_LENGTH,
        .queue_ptr = &queue,
        .queue_storage = queue_buffer,
    },
};

static test_osal_queue_cases_t test_case;

static uint16_t data_to_send;

static uint16_t data_to_receive;


/*========= [SHARED VARIABLES] =================================================*/

/*========= [TEST FUNCTION IMPLEMENTATION] =====================================*/

void test_LoadStruct(void) {
    for (test_case = TEST_OSAL_NULL_QUEUE_PTR; test_case < TEST_OSAL_QTY; test_case++) {
        bool_t res;
        res = OSAL_QUEUE_LoadStruct(test_osal_queue[test_case].queue_ptr, test_osal_queue[test_case].holder_ptr, test_osal_queue[test_case].queue_storage, test_osal_queue[test_case].data_size, test_osal_queue[test_case].queue_length);
        if (test_case == TEST_OSAL_SUCCESS) {
            TEST_ASSERT_EQUAL_UINT8(TRUE, res);
            TEST_ASSERT_EQUAL_UINT8(sizeof(uint16_t), queue.data_size);
            TEST_ASSERT_EQUAL_PTR(NULL, queue.handler);
            TEST_ASSERT_EQUAL_PTR(&queue_holder, queue.holder_ptr);
            TEST_ASSERT_EQUAL_UINT8(TEST_OSAL_QUEUE_LENGTH, queue.queue_length);
            TEST_ASSERT_EQUAL_PTR(&queue_buffer, queue.queue_storage);
        }
        else {
            TEST_ASSERT_EQUAL_UINT8(FALSE, res);
        }
    }
}

void test_Create(void) {
    for (test_case = TEST_OSAL_NULL_QUEUE_PTR; test_case < TEST_OSAL_QTY; test_case++) {
        bool_t res;
        if (test_case == TEST_OSAL_DATA_SIZE_0) {
            queue.data_size = 0;
        }
        else {
            queue.data_size = sizeof(uint16_t);
        }
        res = OSAL_QUEUE_Create(test_osal_queue[test_case].queue_ptr);
        if (test_case == TEST_OSAL_SUCCESS) {
            TEST_ASSERT_EQUAL_PTR(&queue_holder, queue.handler);
        }
        else {
            TEST_ASSERT_EQUAL_UINT8(FALSE, res);
            TEST_ASSERT_EQUAL_PTR(NULL, queue.handler);
        }
    }
}

void test_SendReceive(void) {
    bool_t res_receive;
    bool_t res_send;

    for (test_case = TEST_OSAL_NULL_QUEUE_PTR; test_case < TEST_OSAL_QTY; test_case++) {
        for (data_to_send = 0; data_to_send < 0xFFF; data_to_send += 200) {
            uint16_t last_data_to_receive = data_to_receive;
            switch (test_case) {
                case TEST_OSAL_NULL_QUEUE_PTR:
                    res_send = OSAL_QUEUE_Send(NULL, &data_to_send, OSAL_MS_TO_TICKS(100));
                    res_receive = OSAL_QUEUE_Receive(NULL, &data_to_receive, OSAL_MS_TO_TICKS(100));
                    break;

                case TEST_OSAL_NULL_HOLDER_HANDLER_PTR:
                    queue.handler = NULL;
                    res_send = OSAL_QUEUE_Send(&queue, &data_to_send, OSAL_MS_TO_TICKS(100));
                    res_receive = OSAL_QUEUE_Receive(&queue, &data_to_receive, OSAL_MS_TO_TICKS(100));
                    break;

                case TEST_OSAL_SUCCESS:
                    queue.handler = &queue_holder;
                    res_send = OSAL_QUEUE_Send(&queue, &data_to_send, OSAL_MS_TO_TICKS(100));
                    res_receive = OSAL_QUEUE_Receive(&queue, &data_to_receive, OSAL_MS_TO_TICKS(100));
                    break;

                default:
                    break;
            }
            if (test_case == TEST_OSAL_SUCCESS) {
                TEST_ASSERT_EQUAL_UINT16(data_to_receive, data_to_send);
                TEST_ASSERT_EQUAL_UINT8(TRUE, res_send);
                TEST_ASSERT_EQUAL_UINT8(TRUE, res_receive);
            }
            else {
                TEST_ASSERT_EQUAL_UINT16(last_data_to_receive, data_to_receive);
                TEST_ASSERT_EQUAL_UINT8(FALSE, res_send);
                TEST_ASSERT_EQUAL_UINT8(FALSE, res_receive);
            }
        }
    }
}

void test_SendReceiveFromISR(void) {
    bool_t res_receive;
    bool_t res_send;
    bool_t yield_need;

    for (test_case = TEST_OSAL_NULL_QUEUE_PTR; test_case < TEST_OSAL_QTY; test_case++) {
        for (data_to_send = 0; data_to_send < 0xFFF; data_to_send += 200) {
            uint16_t last_data_to_receive = data_to_receive;
            switch (test_case) {
                case TEST_OSAL_NULL_QUEUE_PTR:
                    res_send = OSAL_QUEUE_SendFromISR(NULL, &data_to_send, &yield_need);
                    TEST_ASSERT_EQUAL_UINT8(FALSE, yield_need);
                    res_receive = OSAL_QUEUE_ReceiveFromISR(NULL, &data_to_receive, &yield_need);
                    TEST_ASSERT_EQUAL_UINT8(FALSE, yield_need);
                    break;

                case TEST_OSAL_NULL_HOLDER_HANDLER_PTR:
                    queue.handler = NULL;
                    res_send = OSAL_QUEUE_SendFromISR(&queue, &data_to_send, &yield_need);
                    TEST_ASSERT_EQUAL_UINT8(FALSE, yield_need);
                    res_receive = OSAL_QUEUE_ReceiveFromISR(&queue, &data_to_receive, &yield_need);
                    TEST_ASSERT_EQUAL_UINT8(FALSE, yield_need);
                    break;

                case TEST_OSAL_SUCCESS:
                    queue.handler = &queue_holder;
                    res_send = OSAL_QUEUE_SendFromISR(&queue, &data_to_send, &yield_need);
                    TEST_ASSERT_EQUAL_UINT8(TRUE, yield_need);
                    res_receive = OSAL_QUEUE_ReceiveFromISR(&queue, &data_to_receive, &yield_need);
                    TEST_ASSERT_EQUAL_UINT8(FALSE, yield_need);
                    break;

                default:
                    break;
            }
            if (test_case == TEST_OSAL_SUCCESS) {
                TEST_ASSERT_EQUAL_UINT16(data_to_receive, data_to_send);
                TEST_ASSERT_EQUAL_UINT8(TRUE, res_send);
                TEST_ASSERT_EQUAL_UINT8(TRUE, res_receive);
            }
            else {
                TEST_ASSERT_EQUAL_UINT16(last_data_to_receive, data_to_receive);
                TEST_ASSERT_EQUAL_UINT8(FALSE, res_send);
                TEST_ASSERT_EQUAL_UINT8(FALSE, res_receive);
            }
        }
    }
}

/*========= [SUPPORT FUNCTION] =================================================*/

QueueHandle_t xQueueCreateStatic(const UBaseType_t queue_length, const UBaseType_t data_size, uint8_t *queue_storage, StaticQueue_t *queue_struct) {
    QueueHandle_t ret = NULL;
    if (test_case == TEST_OSAL_SUCCESS) {
        ret = queue_struct;
    }
    return ret;
}

UBaseType_t xQueueSend(QueueHandle_t handler, void *data, TickType_t wait_time) {
    UBaseType_t ret = pdFALSE;
    if (test_case == TEST_OSAL_SUCCESS) {
        ret = pdTRUE;
        data_to_receive = *((uint16_t *)data);
    }
    return ret;
}

UBaseType_t xQueueReceive(QueueHandle_t handler, void *data, TickType_t wait_time) {
    uint16_t *temp = (uint16_t *)data;
    UBaseType_t ret = pdFALSE;
    if (test_case == TEST_OSAL_SUCCESS) {
        ret = pdTRUE;
        *temp = data_to_receive;
    }
    return ret;
}

BaseType_t xQueueSendFromISR(QueueHandle_t xQueue, void *pvItemToQueue, BaseType_t *const pxHigherPriorityTaskWoken) {
    BaseType_t ret = pdFALSE;
    if (test_case == TEST_OSAL_SUCCESS) {
        ret = pdTRUE;
        data_to_receive = *((uint16_t *)pvItemToQueue);
        *pxHigherPriorityTaskWoken = pdTRUE;
    }
    return ret;
}

BaseType_t xQueueReceiveFromISR(QueueHandle_t xQueue, void *const pvBuffer, BaseType_t *const pxHigherPriorityTaskWoken) {
    uint16_t *temp = (uint16_t *)pvBuffer;
    BaseType_t ret = pdFALSE;
    if (test_case == TEST_OSAL_SUCCESS) {
        ret = pdTRUE;
        *temp = data_to_receive;
        *pxHigherPriorityTaskWoken = pdFALSE;
    }
    return ret;
}
