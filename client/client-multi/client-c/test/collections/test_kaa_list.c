/*
 * Copyright 2014 CyberVision, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <time.h>

#include "../kaa_test.h"
#include "collections/kaa_list.h"
#include "utilities/kaa_log.h"
#include "utilities/kaa_mem.h"

static kaa_logger_t *logger = NULL;

void test_list_create()
{
    KAA_TRACE_IN(logger);

    kaa_list_t *list = kaa_list_create();
    ASSERT_NOT_NULL(list);

    ASSERT_EQUAL(kaa_list_get_size(list), 0);
    ASSERT_NULL(kaa_list_begin(list));
    ASSERT_NULL(kaa_list_back(list));

    kaa_list_destroy(list, NULL);

    KAA_TRACE_OUT(logger);
}

void test_list_push_front()
{
    KAA_TRACE_IN(logger);

    kaa_list_t *list = kaa_list_create();
    ASSERT_NOT_NULL(list);

    int32_t *number;
    int node_number = 2;
    for (int i = 0; i < node_number; ++i) {
        number = (int32_t *)KAA_MALLOC(sizeof(int32_t *));
        ASSERT_NOT_NULL(number);
        *number = rand();
        kaa_list_push_front(list, number);
    }

    ASSERT_EQUAL(kaa_list_get_size(list), node_number);

    ASSERT_NOT_NULL(kaa_list_begin(list));
    ASSERT_NULL(kaa_list_prev(kaa_list_begin(list)));

    ASSERT_EQUAL((*(int32_t *)kaa_list_get_data(kaa_list_begin(list))), *number);

    kaa_list_destroy(list, NULL);

    KAA_TRACE_OUT(logger);
}

void test_list_push_back()
{
    KAA_TRACE_IN(logger);

    kaa_list_t *list = kaa_list_create();
    ASSERT_NOT_NULL(list);

    int32_t *number;
    int node_number = 2;
    for (int i = 0; i < node_number; ++i) {
        number = (int32_t *)KAA_MALLOC(sizeof(int32_t *));
        ASSERT_NOT_NULL(number);
        *number = rand();
        kaa_list_push_back(list, number);
    }

    ASSERT_EQUAL(kaa_list_get_size(list), node_number);
    ASSERT_EQUAL((*(int32_t *)kaa_list_get_data(kaa_list_back(list))), *number);

    kaa_list_destroy(list, NULL);

    KAA_TRACE_OUT(logger);
}

void test_process_data(int32_t *value, int32_t *new_value)
{
    *value = *new_value;
}

void test_list_for_each()
{
    KAA_TRACE_IN(logger);

    kaa_list_t *list = kaa_list_create();
    ASSERT_NOT_NULL(list);

    int32_t *number1_ptr;
    int node_number = 4;
    for (int i = 0; i < node_number; ++i) {
        number1_ptr = (int32_t *)KAA_MALLOC(sizeof(int32_t *));
        ASSERT_NOT_NULL(number1_ptr);
        *number1_ptr = rand();
        kaa_list_push_back(list, number1_ptr);
    }

    int number2 = rand();
    kaa_list_for_each(kaa_list_begin(list), kaa_list_back(list), (process_data)&test_process_data, &number2);

    kaa_list_node_t *it = kaa_list_begin(list);
    while (it) {
        ASSERT_EQUAL(*(int32_t *)kaa_list_get_data(kaa_list_begin(list)), number2);
        it = kaa_list_next(it);
    }

    kaa_list_destroy(list, NULL);

    KAA_TRACE_OUT(logger);
}

int test_init()
{
    srand(time(NULL));

    kaa_error_t error = kaa_log_create(&logger, KAA_MAX_LOG_MESSAGE_LENGTH, KAA_MAX_LOG_LEVEL, NULL);
    if (error || !logger)
        return error;

    return 0;
}

int test_deinit(void)
{
    kaa_log_destroy(logger);

    return 0;
}

KAA_SUITE_MAIN(List, test_init, test_deinit
        ,
        KAA_TEST_CASE(list_create, test_list_create)
        KAA_TEST_CASE(list_push_front, test_list_push_front)
        KAA_TEST_CASE(list_push_back, test_list_push_back)
        KAA_TEST_CASE(list_for_each, test_list_for_each)

)