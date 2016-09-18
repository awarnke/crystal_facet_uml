/* File: data_json_deserializer.c; Copyright and License: see below */

#include "serial/data_json_deserializer.h"
#include "trace.h"
#include "log.h"
#include <assert.h>

void data_json_deserializer_init ( data_json_deserializer_t *this_, const char *in_data )
{
    TRACE_BEGIN();

    data_json_tokenizer_init( &((*this_).tokenizer) );

    (*this_).in_data = in_data;
    (*this_).read_pos = 0;
    (*this_).after_first_array_entry = false;

    TRACE_END();
}

void data_json_deserializer_reinit ( data_json_deserializer_t *this_, const char *in_data )
{
    TRACE_BEGIN();

    data_json_deserializer_destroy( this_ );
    data_json_deserializer_init( this_, in_data );

    TRACE_END();
}

void data_json_deserializer_destroy ( data_json_deserializer_t *this_ )
{
    TRACE_BEGIN();

    data_json_tokenizer_destroy( &((*this_).tokenizer) );

    TRACE_END();
}

data_error_t data_json_deserializer_expect_begin_set ( data_json_deserializer_t *this_ )
{
    TRACE_BEGIN();
    data_error_t result = DATA_ERROR_NONE;
    char member_name_buf[4] = "";
    utf8stringbuf_t member_name = UTF8STRINGBUF( member_name_buf );

    result = data_json_tokenizer_expect_begin_object ( &((*this_).tokenizer), (*this_).in_data, &((*this_).read_pos) );

    if ( DATA_ERROR_NONE == result )
    {
        result = data_json_tokenizer_get_member_name ( &((*this_).tokenizer), (*this_).in_data, &((*this_).read_pos), member_name );
        if ( ! utf8stringbuf_equals_str( member_name, DATA_JSON_CONSTANTS_KEY_SET ) )
        {
            LOG_ERROR_INT( "unexpected object contents at character", (*this_).read_pos );
            result |= DATA_ERROR_PARSER_STRUCTURE;
        }
    }

    if ( DATA_ERROR_NONE == result )
    {
        result = data_json_tokenizer_expect_name_separator( &((*this_).tokenizer), (*this_).in_data, &((*this_).read_pos) );
    }

    if ( DATA_ERROR_NONE == result )
    {
        result = data_json_tokenizer_expect_begin_array( &((*this_).tokenizer), (*this_).in_data, &((*this_).read_pos) );
    }

    TRACE_END_ERR( result );
    return result;
}

data_error_t data_json_deserializer_expect_end_set ( data_json_deserializer_t *this_ )
{
    TRACE_BEGIN();
    data_error_t result = DATA_ERROR_NONE;
    bool condition;

    result = data_json_tokenizer_is_end_array ( &((*this_).tokenizer), (*this_).in_data, &((*this_).read_pos), &condition );
    if ( ! condition )
    {
        LOG_ERROR_INT( "unexpected array contents at character", (*this_).read_pos );
        result |= DATA_ERROR_PARSER_STRUCTURE;
    }

    if ( DATA_ERROR_NONE == result )
    {
        result = data_json_tokenizer_is_end_object ( &((*this_).tokenizer), (*this_).in_data, &((*this_).read_pos), &condition );
        if ( ! condition )
        {
            LOG_ERROR_INT( "unexpected object contents at character", (*this_).read_pos );
            result |= DATA_ERROR_PARSER_STRUCTURE;
        }
    }

    if ( DATA_ERROR_NONE == result )
    {
        result = data_json_tokenizer_expect_eof ( &((*this_).tokenizer), (*this_).in_data, &((*this_).read_pos) );
    }

    TRACE_END_ERR( result );
    return result;
}

data_error_t data_json_deserializer_get_type_of_next_element ( data_json_deserializer_t *this_, data_table_t *out_type )
{
    TRACE_BEGIN();
    assert ( NULL != out_type );
    data_error_t result = DATA_ERROR_NONE;

    uint32_t temp_read_pos;
    bool array_end;
    char member_name_buf[16];
    utf8stringbuf_t member_name = UTF8STRINGBUF( member_name_buf );

    temp_read_pos = (*this_).read_pos;
    result = data_json_tokenizer_is_end_array ( &((*this_).tokenizer), (*this_).in_data, &temp_read_pos, &array_end );

    if ( DATA_ERROR_NONE == result )
    {
        if ( array_end )
        {
            (*out_type) = DATA_TABLE_VOID;
        }
        else
        {
            if ( (*this_).after_first_array_entry )
            {
                result = data_json_tokenizer_expect_value_separator ( &((*this_).tokenizer), (*this_).in_data, &(*this_).read_pos );
                temp_read_pos = (*this_).read_pos;
            }
            if ( DATA_ERROR_NONE == result )
            {
                result = data_json_tokenizer_expect_begin_object ( &((*this_).tokenizer), (*this_).in_data, &temp_read_pos );
            }
            if ( DATA_ERROR_NONE == result )
            {
                result = data_json_tokenizer_get_member_name ( &((*this_).tokenizer), (*this_).in_data, &temp_read_pos, member_name );
            }
            if ( DATA_ERROR_NONE == result )
            {
                if ( utf8stringbuf_equals_str( member_name, DATA_JSON_CONSTANTS_KEY_CLASSIFIER ) )
                {
                    (*out_type) = DATA_TABLE_CLASSIFIER;
                }
                else if ( utf8stringbuf_equals_str( member_name, DATA_JSON_CONSTANTS_KEY_DIAGRAM ) )
                {
                    (*out_type) = DATA_TABLE_DIAGRAM;
                }
                else
                {
                    LOG_ERROR_INT( "unexpected token before character", temp_read_pos );
                    result = DATA_ERROR_PARSER_STRUCTURE;
                }
            }
        }
    }

    TRACE_END_ERR( result );
    return result;
}

data_error_t data_json_deserializer_get_next_classifier ( data_json_deserializer_t *this_, data_classifier_t *out_object )
{
    TRACE_BEGIN();
    assert ( NULL != out_object );
    data_error_t result = DATA_ERROR_NONE;

    result = DATA_ERROR_NOT_YET_IMPLEMENTED_ID;

    (*this_).after_first_array_entry = true;

    TRACE_END_ERR( result );
    return result;
}

data_error_t data_json_deserializer_get_next_diagram ( data_json_deserializer_t *this_, data_diagram_t *out_object )
{
    TRACE_BEGIN();
    assert ( NULL != out_object );
    data_error_t result = DATA_ERROR_NONE;

    result = DATA_ERROR_NOT_YET_IMPLEMENTED_ID;

    (*this_).after_first_array_entry = true;

    TRACE_END_ERR( result );
    return result;
}

void data_json_deserializer_get_read_pos ( data_json_deserializer_t *this_, uint32_t *out_read_pos )
{
    TRACE_BEGIN();
    assert ( NULL != out_read_pos );

    (*out_read_pos) = (*this_).read_pos;

    TRACE_END();
}


/*
Copyright 2016-2016 Andreas Warnke

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/
