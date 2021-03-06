/* File: geometry_non_linear_scale_test.c; Copyright and License: see below */

#include "geometry_non_linear_scale_test.h"
#include "util/geometry/geometry_non_linear_scale.h"
#include "test_assert.h"

static void set_up(void);
static void tear_down(void);
static void test_scale_conversion(void);

test_suite_t geometry_non_linear_scale_test_get_list(void)
{
    test_suite_t result;
    test_suite_init( &result, "geometry_non_linear_scale_test_get_list", &set_up, &tear_down );
    test_suite_add_test_case( &result, "test_scale_conversion", &test_scale_conversion );
    return result;
}

static void set_up(void)
{
}

static void tear_down(void)
{
}

static void test_scale_conversion(void)
{
    geometry_non_linear_scale_t my_scale;
    double location;
    int32_t order;

    /* init and trace */

    geometry_non_linear_scale_init ( &my_scale, -3.14, 3.14 );

    geometry_non_linear_scale_add_order( &my_scale, -100 );

    geometry_non_linear_scale_add_order( &my_scale, 200 );

    geometry_non_linear_scale_add_order( &my_scale, 200 );

    geometry_non_linear_scale_trace ( &my_scale );

    /* test get_location */

    location = geometry_non_linear_scale_get_location( &my_scale, +200 );

    TEST_ASSERT( ( +1.0 < location ) && ( location < +1.1 ) );

    location = geometry_non_linear_scale_get_location( &my_scale, +100 );

    TEST_ASSERT( ( +0.3 < location ) && ( location < +0.4 ) );

    /* test get_order with snap */

    order = geometry_non_linear_scale_get_order ( &my_scale, -3.1, 0.1 );

    TEST_ASSERT_EQUAL_INT( INT32_MIN, order );

    /* test get_order without snap */

    order = geometry_non_linear_scale_get_order ( &my_scale, -3.1, 0.01 );

    TEST_ASSERT( ( INT32_MIN < order ) && ( order < -1000000000 ) );

    order = geometry_non_linear_scale_get_order ( &my_scale, -1.0, 0.01 );

    TEST_ASSERT( ( -100 < order ) && ( order < -80 ) );

    geometry_non_linear_scale_destroy ( &my_scale );
}


/*
 * Copyright 2016-2021 Andreas Warnke
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
