/* File: ctrl_consistency_checker.c; Copyright and License: see below */

#include "ctrl_consistency_checker.h"
#include "trace.h"
#include "tslog.h"
#include <assert.h>

void ctrl_consistency_checker_init ( ctrl_consistency_checker_t *this_, data_database_reader_t *db_reader, data_database_writer_t *db_writer )
{
    TRACE_BEGIN();
    assert( NULL != db_reader );
    assert( NULL != db_writer );

    (*this_).db_reader = db_reader;
    (*this_).db_writer = db_writer;

    TRACE_END();
}

void ctrl_consistency_checker_destroy ( ctrl_consistency_checker_t *this_ )
{
    TRACE_BEGIN();

    (*this_).db_reader = NULL;
    (*this_).db_writer = NULL;

    TRACE_END();
}

/*
 *  \brief checks and repairs the database
 *
 *  \param modify_db true if the database shall be repaired and modified
 *  \return CTRL_ERROR_NONE in case of success,
 *          CTRL_ERROR_NO_DB if database not open/loaded,
 *          CTRL_ERROR_DB_STRUCTURE if database was corrupted
 */
ctrl_error_t ctrl_consistency_checker_repair_database ( ctrl_consistency_checker_t *this_, bool modify_db )
{
    TRACE_BEGIN();
    ctrl_error_t err_result = CTRL_ERROR_NONE;

    TSLOG_ERROR( "not yet implemented" );

    TRACE_END_ERR( err_result );
}

/*
 *  \brief checks if a given classifier name is not yet existant in the database
 *
 *  \param classifier_name new classifier name
 *  \return CTRL_ERROR_NONE if the name is unique,
 *          CTRL_ERROR_DUPLICATE_NAME if the name already exists
 *          CTRL_ERROR_NO_DB if database not open/loaded
 */
ctrl_error_t ctrl_consistency_checker_is_classifier_name_unique ( ctrl_consistency_checker_t *this_, const char* classifier_name )
{
    TRACE_BEGIN();
    ctrl_error_t err_result = CTRL_ERROR_NONE;

    TSLOG_ERROR( "not yet implemented" );

    TRACE_END_ERR( err_result );
}

/*
 *  \brief proposes an alternative classifier name that is not yet existant in the database
 *
 *  \param classifier_name new classifier name which might not be unique
 *  \param result unique classifier name
 *  \return CTRL_ERROR_NONE in case of success,
 *          CTRL_ERROR_NO_DB if database not open/loaded
 */
ctrl_error_t ctrl_consistency_checker_propose_unique_classifier_name ( ctrl_consistency_checker_t* *this_, const char* classifier_name, utf8stringbuf_t result )
{
    TRACE_BEGIN();
    ctrl_error_t err_result = CTRL_ERROR_NONE;

    TSLOG_ERROR( "not yet implemented" );

    TRACE_END_ERR( err_result );
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
