/* File: data_diagram.inl; Copyright and License: see below */

#include "trace.h"
#include "tslog.h"
#include <assert.h>

static inline data_error_t data_diagram_init_new ( data_diagram_t *this_, int64_t parent_diagram_id, data_diagram_type_t diagram_type, const char* diagram_name, const char* diagram_description, int32_t list_order )
{
    utf8error_t strerr;
    data_error_t result = DATA_ERROR_NONE;

    (*this_).id = DATA_ID_VOID_ID;
    (*this_).parent_id = parent_diagram_id;
    (*this_).diagram_type = diagram_type;

    (*this_).name = utf8stringbuf_init( sizeof((*this_).private_name_buffer), (*this_).private_name_buffer );
    strerr = utf8stringbuf_copy_str( (*this_).name, diagram_name );
    if ( strerr != UTF8ERROR_SUCCESS )
    {
        TSLOG_ERROR_INT( "utf8stringbuf_copy_str() failed:", strerr );
        result |= DATA_ERROR_STRING_BUFFER_EXCEEDED;
    }

    (*this_).description = utf8stringbuf_init( sizeof((*this_).private_description_buffer), (*this_).private_description_buffer );
    strerr = utf8stringbuf_copy_str( (*this_).description, diagram_description );
    if ( strerr != UTF8ERROR_SUCCESS )
    {
        TSLOG_ERROR_INT( "utf8stringbuf_copy_str() failed:", strerr );
        result |= DATA_ERROR_STRING_BUFFER_EXCEEDED;
    }

    (*this_).list_order = list_order;

    return result;
}

static inline void data_diagram_init_empty ( data_diagram_t *this_ )
{
    (*this_).id = DATA_ID_VOID_ID;
    (*this_).parent_id = DATA_ID_VOID_ID;
    (*this_).diagram_type = DATA_DIAGRAM_TYPE_LIST;

    (*this_).name = utf8stringbuf_init( sizeof((*this_).private_name_buffer), (*this_).private_name_buffer );
    utf8stringbuf_clear( (*this_).name );
    (*this_).description = utf8stringbuf_init( sizeof((*this_).private_description_buffer), (*this_).private_description_buffer );
    utf8stringbuf_clear( (*this_).description );

    (*this_).list_order = 0;
}

static inline void data_diagram_reinit_empty ( data_diagram_t *this_ )
{
    /* data_diagram_destroy( this_ );  -- not necessary */
    data_diagram_init_empty( this_ );
}

static inline data_error_t data_diagram_init ( data_diagram_t *this_, int64_t diagram_id, int64_t parent_diagram_id, data_diagram_type_t diagram_type, const char* diagram_name, const char* diagram_description, int32_t list_order )
{
    utf8error_t strerr;
    data_error_t result = DATA_ERROR_NONE;

    (*this_).id = diagram_id;
    (*this_).parent_id = parent_diagram_id;
    (*this_).diagram_type = diagram_type;

    (*this_).name = utf8stringbuf_init( sizeof((*this_).private_name_buffer), (*this_).private_name_buffer );
    strerr = utf8stringbuf_copy_str( (*this_).name, diagram_name );
    if ( strerr != UTF8ERROR_SUCCESS )
    {
        TSLOG_ERROR_INT( "utf8stringbuf_copy_str() failed:", strerr );
        result |= DATA_ERROR_STRING_BUFFER_EXCEEDED;
    }

    (*this_).description = utf8stringbuf_init( sizeof((*this_).private_description_buffer), (*this_).private_description_buffer );
    strerr = utf8stringbuf_copy_str( (*this_).description, diagram_description );
    if ( strerr != UTF8ERROR_SUCCESS )
    {
        TSLOG_ERROR_INT( "utf8stringbuf_copy_str() failed:", strerr );
        result |= DATA_ERROR_STRING_BUFFER_EXCEEDED;
    }
    (*this_).list_order = list_order;

    return result;
}

static inline void data_diagram_copy ( data_diagram_t *this_, const data_diagram_t *original )
{
    (*this_) = (*original);
    /* repair the overwritten pointers */
    (*this_).name = utf8stringbuf_init( sizeof((*this_).private_name_buffer), (*this_).private_name_buffer );
    (*this_).description = utf8stringbuf_init( sizeof((*this_).private_description_buffer), (*this_).private_description_buffer );
}

static inline void data_diagram_replace ( data_diagram_t *this_, const data_diagram_t *that )
{
    (*this_) = (*that);
    /* repair the overwritten pointers */
    (*this_).name = utf8stringbuf_init( sizeof((*this_).private_name_buffer), (*this_).private_name_buffer );
    (*this_).description = utf8stringbuf_init( sizeof((*this_).private_description_buffer), (*this_).private_description_buffer );
}

static inline void data_diagram_destroy ( data_diagram_t *this_ )
{
    (*this_).id = DATA_ID_VOID_ID;
    (*this_).parent_id = DATA_ID_VOID_ID;
}

static inline void data_diagram_trace ( data_diagram_t *this_ )
{
    TRACE_INFO( "data_diagram_t" );
    TRACE_INFO_INT( "- id:", (*this_).id );
    TRACE_INFO_INT( "- parent_id:", (*this_).parent_id );
    TRACE_INFO_INT( "- diagram_type:", (*this_).diagram_type );
    TRACE_INFO_STR( "- name:", utf8stringbuf_get_string((*this_).name) );
    TRACE_INFO_STR( "- description:", utf8stringbuf_get_string((*this_).description) );
    TRACE_INFO_INT( "- list_order:", (*this_).list_order );
}

static inline int64_t data_diagram_get_id ( data_diagram_t *this_ )
{
    return (*this_).id;
}

static inline void data_diagram_set_id ( data_diagram_t *this_, int64_t id )
{
    (*this_).id = id;
}

static inline int64_t data_diagram_get_parent_id ( data_diagram_t *this_ )
{
    return (*this_).parent_id;
}

static inline void data_diagram_set_parent_id ( data_diagram_t *this_, int64_t parent_id )
{
    (*this_).parent_id = parent_id;
}

static inline data_diagram_type_t data_diagram_get_diagram_type ( data_diagram_t *this_ )
{
    return (*this_).diagram_type;
}

static inline void data_diagram_set_diagram_type ( data_diagram_t *this_, data_diagram_type_t diagram_type )
{
    (*this_).diagram_type = diagram_type;
}

static inline const char *data_diagram_get_name_ptr ( data_diagram_t *this_ )
{
    return utf8stringbuf_get_string( (*this_).name );
}

static inline utf8stringbuf_t data_diagram_get_name_buf_ptr ( data_diagram_t *this_ )
{
    return (*this_).name;
}

static inline data_error_t data_diagram_set_name ( data_diagram_t *this_, const char *name )
{
    data_error_t result = DATA_ERROR_NONE;
    utf8error_t strerr;
    strerr = utf8stringbuf_copy_str( (*this_).name, name );
    if ( strerr != UTF8ERROR_SUCCESS )
    {
        TSLOG_ERROR_INT( "utf8stringbuf_copy_str() failed:", strerr );
        result = DATA_ERROR_STRING_BUFFER_EXCEEDED;
    }
    return result;
}

static inline const char *data_diagram_get_description_ptr ( data_diagram_t *this_ )
{
    return utf8stringbuf_get_string( (*this_).description );
}

static inline utf8stringbuf_t data_diagram_get_description_buf_ptr ( data_diagram_t *this_ )
{
    return (*this_).description;
}

static inline data_error_t data_diagram_set_description ( data_diagram_t *this_, const char *description )
{
    data_error_t result = DATA_ERROR_NONE;
    utf8error_t strerr;
    strerr = utf8stringbuf_copy_str( (*this_).description, description );
    if ( strerr != UTF8ERROR_SUCCESS )
    {
        TSLOG_ERROR_INT( "utf8stringbuf_copy_str() failed:", strerr );
        result = DATA_ERROR_STRING_BUFFER_EXCEEDED;
    }
    return result;
}

static inline int32_t data_diagram_get_list_order ( data_diagram_t *this_ )
{
    return (*this_).list_order;
}

static inline void data_diagram_set_list_order ( data_diagram_t *this_, int32_t list_order )
{
    (*this_).list_order = list_order;
}

static inline bool data_diagram_is_valid ( data_diagram_t *this_ )
{
    return ( DATA_ID_VOID_ID != (*this_).id );
}


/*
Copyright 2016-2017 Andreas Warnke

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
