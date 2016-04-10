/* File: ctrl_diagram_controller.c; Copyright and License: see below */

#include "ctrl_diagram_controller.h"
#include "trace.h"
#include "log.h"

void ctrl_diagram_controller_init ( ctrl_diagram_controller_t *this_, data_database_t *database )
{
    TRACE_BEGIN();

    (*this_).database = database;
    data_database_writer_init( &((*this_).db_writer), database );
    data_database_reader_init( &((*this_).db_reader), database );

    TRACE_END();
}

void ctrl_diagram_controller_destroy ( ctrl_diagram_controller_t *this_ )
{
    TRACE_BEGIN();

    data_database_writer_destroy( &((*this_).db_writer) );
    data_database_reader_destroy( &((*this_).db_reader) );

    TRACE_END();
}

ctrl_error_t ctrl_diagram_controller_create_diagram ( ctrl_diagram_controller_t *this_, int64_t parent_diagram_id, data_diagram_type_t diagram_type, const char* diagram_name, int64_t* out_new_id )
{
    TRACE_BEGIN();
    data_diagram_t to_be_created;
    ctrl_error_t result = CTRL_ERROR_NONE;
    data_error_t data_result;
    int64_t new_id;

    data_diagram_init_new( &to_be_created, parent_diagram_id, diagram_type, diagram_name, "" );

    data_result = data_database_writer_create_diagram( &((*this_).db_writer), &to_be_created, &new_id );
    if ( DATA_ERROR_NONE == data_result )
    {
        if ( NULL != out_new_id )
        {
            *out_new_id = new_id;
        }
    }
    result = (ctrl_error_t) data_result;

    data_diagram_destroy( &to_be_created );

    TRACE_END_ERR( result );
    return result;
}

ctrl_error_t ctrl_diagram_controller_create_root_diagram_if_not_exists ( ctrl_diagram_controller_t *this_, data_diagram_type_t diagram_type, const char* diagram_name, int64_t* out_new_id )
{
    TRACE_BEGIN();
    ctrl_error_t result = CTRL_ERROR_NONE;
    data_error_t data_result;
    data_diagram_t root_diag_buf[1];

    /* load all without parent */
    int32_t count;
    data_result = data_database_reader_get_diagrams_by_parent_id( &((*this_).db_reader),
                                                                  DATA_DIAGRAM_ID_VOID_ID,
                                                                  1,
                                                                  &count,
                                                                  &(root_diag_buf) );
    if ( DATA_ERROR_NONE != data_result )
    {
        result = (ctrl_error_t) data_result;
    }
    else
    {
        if ( 0 == count )
        {
            /* no root diagram exists */
            result = ctrl_diagram_controller_create_diagram( this_, DATA_DIAGRAM_ID_VOID_ID, diagram_type, diagram_name, out_new_id );
        }
        else
        {
            if ( NULL != out_new_id )
            {
                *out_new_id = DATA_DIAGRAM_ID_VOID_ID;
            }
        }
    }

    TRACE_END_ERR( result );
    return result;
}

ctrl_error_t ctrl_diagram_controller_update_diagram_description ( ctrl_diagram_controller_t *this_, int64_t diagram_id, const char* new_diagram_description )
{
    TRACE_BEGIN();
    ctrl_error_t result = CTRL_ERROR_NONE;
    data_error_t data_result;

    data_result = data_database_writer_update_diagram_description( &((*this_).db_writer), diagram_id, new_diagram_description );
    result = (ctrl_error_t) data_result;

    TRACE_END_ERR( result );
    return result;
}

ctrl_error_t ctrl_diagram_controller_update_diagram_name ( ctrl_diagram_controller_t *this_, int64_t diagram_id, const char* new_diagram_name )
{
    TRACE_BEGIN();
    ctrl_error_t result = CTRL_ERROR_NONE;
    data_error_t data_result;

    data_result = data_database_writer_update_diagram_name( &((*this_).db_writer), diagram_id, new_diagram_name );
    result = (ctrl_error_t) data_result;

    TRACE_END_ERR( result );
    return result;
}

ctrl_error_t ctrl_diagram_controller_update_diagram_type ( ctrl_diagram_controller_t *this_, int64_t diagram_id, data_diagram_type_t new_diagram_type )
{
    TRACE_BEGIN();
    ctrl_error_t result = CTRL_ERROR_NONE;
    data_error_t data_result;

    data_result = data_database_writer_update_diagram_type( &((*this_).db_writer), diagram_id, new_diagram_type );
    result = (ctrl_error_t) data_result;

    TRACE_END_ERR( result );
    return result;
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