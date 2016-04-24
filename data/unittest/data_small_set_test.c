/* File: data_small_set_test.c; Copyright and License: see below */

#include "data_small_set_test.h"
#include "data_small_set.h"

static void set_up(void);
static void tear_down(void);
static void test_notifier_list_insert_and_remove(void);
static void test_notifier_list_full(void);

TestRef data_small_set_test_get_list(void)
{
    EMB_UNIT_TESTFIXTURES(fixtures) {
        new_TestFixture("test_notifier_list_insert_and_remove",test_notifier_list_insert_and_remove),
        new_TestFixture("test_notifier_list_full",test_notifier_list_full),
    };
    EMB_UNIT_TESTCALLER(result,"data_small_set_test_get_list",set_up,tear_down,fixtures);

    return (TestRef)&result;
}

static void set_up(void)
{
}

static void tear_down(void)
{
}

static void test_notifier_list_insert_and_remove(void)
{
}

static void test_notifier_list_full(void)
{
}


/*
 * Copyright 2016-2016 Andreas Warnke
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
