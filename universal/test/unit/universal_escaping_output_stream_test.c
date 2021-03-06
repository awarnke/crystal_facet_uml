/* File: universal_escaping_output_stream_test.c; Copyright and License: see below */

#include "universal_escaping_output_stream_test.h"
#include "stream/universal_escaping_output_stream.h"
#include "stream/universal_memory_output_stream.h"
#include "test_assert.h"
#include <string.h>
#include <assert.h>

static void set_up(void);
static void tear_down(void);
static void test_write_regular(void);
static void test_write_border_cases(void);

static char my_out_buffer[16];
static universal_memory_output_stream_t my_mem_out_stream;
static universal_escaping_output_stream_t my_esc_out_stream;
const char *const ((my_patterns_and_replacements)[][2]) = {{"&","&amp;"},{"--","- - "},{"\n","  \n"},{NULL,NULL}};

test_suite_t universal_escaping_output_stream_test_get_list(void)
{
    test_suite_t result;
    test_suite_init( &result, "universal_escaping_output_stream_test_get_list", &set_up, &tear_down );
    test_suite_add_test_case( &result, "test_write_regular", &test_write_regular );
    test_suite_add_test_case( &result, "test_write_border_cases", &test_write_border_cases );
    return result;
}

static void set_up(void)
{
    memset( &my_out_buffer, '\0', sizeof(my_out_buffer) );
    universal_memory_output_stream_init( &my_mem_out_stream, &my_out_buffer, sizeof(my_out_buffer) );
    universal_escaping_output_stream_init( &my_esc_out_stream,
                                           &my_patterns_and_replacements,
                                           universal_memory_output_stream_get_output_stream( &my_mem_out_stream )
                                         );
}

static void tear_down(void)
{
    int err;
    err = universal_escaping_output_stream_destroy( &my_esc_out_stream );
    err = universal_memory_output_stream_destroy( &my_mem_out_stream );
    TEST_ENVIRONMENT_ASSERT( err == 0 );
}

static void test_write_regular(void)
{
    int err;

    /* get universal_output_stream_t */
    universal_output_stream_t *my_out_stream;
    my_out_stream = universal_escaping_output_stream_get_output_stream( &my_esc_out_stream );
    TEST_ASSERT( my_out_stream != NULL );

    /* open */
    err = universal_output_stream_open ( my_out_stream, "identifier" );
    TEST_ASSERT_EQUAL_INT( 0, err );

    /* write */
    const char test_1[] = "&";
    err = universal_output_stream_write ( my_out_stream, test_1, strlen(test_1) );
    TEST_ASSERT_EQUAL_INT( 0, err );
    TEST_ASSERT_EQUAL_INT( 0, strcmp( &(my_out_buffer[0]), "&amp;" ) );

    /* flush */
    err = universal_output_stream_flush (my_out_stream);
    TEST_ASSERT_EQUAL_INT( 0, err );

    /* write */
    const char test_2[] = "---4\n";
    err = universal_output_stream_write ( my_out_stream, test_2, strlen(test_2) );
    TEST_ASSERT_EQUAL_INT( 0, err );
    TEST_ASSERT_EQUAL_INT( 0, strcmp( &(my_out_buffer[0]), "&amp;- - -4  \n" ) );

    /* write */
    const char test_3[] = "";
    err = universal_output_stream_write ( my_out_stream, test_3, strlen(test_3) );
    TEST_ASSERT_EQUAL_INT( 0, err );
    TEST_ASSERT_EQUAL_INT( 0, strcmp( &(my_out_buffer[0]), "&amp;- - -4  \n" ) );
    
    /* close */
    err = universal_output_stream_close (my_out_stream);
    TEST_ASSERT_EQUAL_INT( 0, err );
}

static void test_write_border_cases(void)
{
    int err;

    /* get universal_output_stream_t */
    universal_output_stream_t *my_out_stream;
    my_out_stream = universal_escaping_output_stream_get_output_stream( &my_esc_out_stream );
    TEST_ASSERT( my_out_stream != NULL );

    /* write */
    const char test_1[] = "0123456789abcdef";
    err = universal_output_stream_write ( my_out_stream, test_1, sizeof(test_1) );
    TEST_ASSERT_EQUAL_INT( -1, err );
    TEST_ASSERT_EQUAL_INT( 0, memcmp( &(my_out_buffer[0]), test_1, sizeof(my_out_buffer) ) );

    /* close */
    err = universal_output_stream_close (my_out_stream);
    TEST_ASSERT_EQUAL_INT( 0, err );

    /* open */
    err = universal_output_stream_open ( my_out_stream, "identifier" );
    TEST_ASSERT_EQUAL_INT( 0, err );
    
    /* write */
    const char test_2[] = "&&-----";
    err = universal_output_stream_write ( my_out_stream, test_2, strlen(test_2) );
    TEST_ASSERT_EQUAL_INT( -1, err );
    TEST_ASSERT_EQUAL_INT( 0, memcmp( &(my_out_buffer[0]), "&amp;&amp;- - - ", sizeof(my_out_buffer) ) );

    /* write */
    const char test_3[] = "\n";
    err = universal_output_stream_write ( my_out_stream, test_3, sizeof(test_3) );
    TEST_ASSERT_EQUAL_INT( -1, err );
    TEST_ASSERT_EQUAL_INT( 0, memcmp( &(my_out_buffer[0]), "&amp;&amp;- - - ", sizeof(my_out_buffer) ) );
}


/*
 * Copyright 2020-2021 Andreas Warnke
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
