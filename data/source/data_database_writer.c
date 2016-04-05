/* File: data_database_writer.c; Copyright and License: see below */

#include "data_database_writer.h"
#include "data_id.h"
#include "trace.h"
#include "log.h"
#include <sqlite3.h>

/*!
 *  \brief string constant to start a transaction
 *
 *  \see http://sqlite.org/lang.html
 */
static const char *DATA_DATABASE_WRITER_BEGIN_TRANSACTION =
    "BEGIN TRANSACTION;";

/*!
 *  \brief string constant to commit a transaction
 *
 *  \see http://sqlite.org/lang.html
 */
static const char *DATA_DATABASE_WRITER_COMMIT_TRANSACTION =
    "COMMIT TRANSACTION;";

/*!
 *  \brief prefix string constant to insert a diagram
 */
static const char *DATA_DATABASE_WRITER_INSERT_DIAGRAM_PREFIX =
    "INSERT INTO diagrams (parent_id,diagram_type,name,description,list_order) VALUES (";

/*!
 *  \brief value separator string constant to insert a diagram
 */
static const char *DATA_DATABASE_WRITER_INSERT_DIAGRAM_VALUE_SEPARATOR = ",";

/*!
 *  \brief string start marker string constant to insert/update a diagram
 */
static const char *DATA_DATABASE_WRITER_STRING_VALUE_START = "\'";

/*!
 *  \brief string end marker string constant to insert/update a diagram
 */
static const char *DATA_DATABASE_WRITER_STRING_VALUE_END = "\'";

/*!
 *  \brief postfix string constant to insert a diagram
 */
static const char *DATA_DATABASE_WRITER_INSERT_DIAGRAM_POSTFIX = ");";

/*!
 *  \brief prefix string constant to update a diagram
 */
static const char *DATA_DATABASE_WRITER_UPDATE_DIAGRAM_PREFIX = "UPDATE diagrams SET ";

/*!
 *  \brief field name string constant to be used for updating a diagram
 */
static const char *DATA_DATABASE_WRITER_UPDATE_DIAGRAM_COL_TYPE = "diagram_type=";

/*!
 *  \brief field name string constant to be used for updating a diagram
 */
static const char *DATA_DATABASE_WRITER_UPDATE_DIAGRAM_COL_NAME = "name=";

/*!
 *  \brief field name string constant to be used for updating a diagram
 */
static const char *DATA_DATABASE_WRITER_UPDATE_DIAGRAM_COL_DESCRIPTION = "description=";

/*!
 *  \brief infix string constant to update a diagram
 */
static const char *DATA_DATABASE_WRITER_UPDATE_DIAGRAM_INFIX = " WHERE id=";

/*!
 *  \brief postfix string constant to update a diagram
 */
static const char *DATA_DATABASE_WRITER_UPDATE_DIAGRAM_POSTFIX = " ;";

/*!
 *  \brief translation table to encode strings for usage in string literals
 *
 *  Note: This table is not suitable for searches using the LIKE operator because _ and % are not handled.
 */
const char *const DATA_DATABASE_WRITER_SQL_ENCODE[] = {
    "'", "''",
    NULL,
};

void data_database_writer_init ( data_database_writer_t *this_, data_database_t *database )
{
    TRACE_BEGIN();
    int perr;

    (*this_).database = database;
    (*this_).private_temp_stringbuf = utf8stringbuf_init( sizeof((*this_).private_temp_buffer), (*this_).private_temp_buffer );
    (*this_).private_sql_stringbuf = utf8stringbuf_init( sizeof((*this_).private_sql_buffer), (*this_).private_sql_buffer );

    perr = pthread_mutex_init ( &((*this_).private_lock), NULL );
    if ( perr != 0 )
    {
        LOG_ERROR_INT( "pthread_mutex_init() failed:", perr );
    }

    TRACE_END();
}

void data_database_writer_destroy ( data_database_writer_t *this_ )
{
    TRACE_BEGIN();
    int perr;

    perr = pthread_mutex_destroy ( &((*this_).private_lock) );
    if ( perr != 0 )
    {
        LOG_ERROR_INT( "pthread_mutex_destroy() failed:", perr );
    }

    TRACE_END();
}

data_error_t data_database_writer_private_build_create_diagram_command ( data_database_writer_t *this_, const data_diagram_t *diagram )
{
    TRACE_BEGIN();
    utf8error_t strerr = UTF8ERROR_SUCCESS;
    data_error_t result = DATA_ERROR_NONE;

    strerr |= utf8stringbuf_copy_str( (*this_).private_sql_stringbuf, DATA_DATABASE_WRITER_INSERT_DIAGRAM_PREFIX );
    strerr |= utf8stringbuf_append_int( (*this_).private_sql_stringbuf, (*diagram).parent_id );
    strerr |= utf8stringbuf_append_str( (*this_).private_sql_stringbuf, DATA_DATABASE_WRITER_INSERT_DIAGRAM_VALUE_SEPARATOR );
    strerr |= utf8stringbuf_append_int( (*this_).private_sql_stringbuf, (*diagram).diagram_type );
    strerr |= utf8stringbuf_append_str( (*this_).private_sql_stringbuf, DATA_DATABASE_WRITER_INSERT_DIAGRAM_VALUE_SEPARATOR );

    strerr |= utf8stringbuf_append_str( (*this_).private_sql_stringbuf, DATA_DATABASE_WRITER_STRING_VALUE_START );
    {
        /* prepare temp buf */
        strerr |= utf8stringbuf_copy_buf( (*this_).private_temp_stringbuf, (*diagram).name );
        strerr |= utf8stringbuf_replace_all( (*this_).private_temp_stringbuf, DATA_DATABASE_WRITER_SQL_ENCODE );
    }
    strerr |= utf8stringbuf_append_buf( (*this_).private_sql_stringbuf, (*this_).private_temp_stringbuf );
    strerr |= utf8stringbuf_append_str( (*this_).private_sql_stringbuf, DATA_DATABASE_WRITER_STRING_VALUE_END );

    strerr |= utf8stringbuf_append_str( (*this_).private_sql_stringbuf, DATA_DATABASE_WRITER_INSERT_DIAGRAM_VALUE_SEPARATOR );

    strerr |= utf8stringbuf_append_str( (*this_).private_sql_stringbuf, DATA_DATABASE_WRITER_STRING_VALUE_START );
    {
        /* prepare temp buf */
        strerr |= utf8stringbuf_copy_buf( (*this_).private_temp_stringbuf, (*diagram).description );
        strerr |= utf8stringbuf_replace_all( (*this_).private_temp_stringbuf, DATA_DATABASE_WRITER_SQL_ENCODE );
    }
    strerr |= utf8stringbuf_append_buf( (*this_).private_sql_stringbuf, (*this_).private_temp_stringbuf );
    strerr |= utf8stringbuf_append_str( (*this_).private_sql_stringbuf, DATA_DATABASE_WRITER_STRING_VALUE_END );

    strerr |= utf8stringbuf_append_str( (*this_).private_sql_stringbuf, DATA_DATABASE_WRITER_INSERT_DIAGRAM_VALUE_SEPARATOR );
    strerr |= utf8stringbuf_append_int( (*this_).private_sql_stringbuf, (*diagram).parent_id );
    strerr |= utf8stringbuf_append_str( (*this_).private_sql_stringbuf, DATA_DATABASE_WRITER_INSERT_DIAGRAM_POSTFIX );

    if ( strerr != UTF8ERROR_SUCCESS )
    {
        LOG_ERROR_HEX( "utf8stringbuf_xxx() failed:", strerr );
        result |= DATA_ERROR_STRING_BUFFER_EXCEEDED;
    }

    TRACE_END_ERR( result );
    return( result );
}

data_error_t data_database_writer_private_build_update_diagram_name_cmd ( data_database_writer_t *this_, int64_t diagram_id, const char *new_diagram_name )
{
    TRACE_BEGIN();
    utf8error_t strerr = UTF8ERROR_SUCCESS;
    data_error_t result = DATA_ERROR_NONE;

    strerr |= utf8stringbuf_copy_str( (*this_).private_sql_stringbuf, DATA_DATABASE_WRITER_UPDATE_DIAGRAM_PREFIX );
    strerr |= utf8stringbuf_append_str( (*this_).private_sql_stringbuf, DATA_DATABASE_WRITER_UPDATE_DIAGRAM_COL_NAME );

    strerr |= utf8stringbuf_append_str( (*this_).private_sql_stringbuf, DATA_DATABASE_WRITER_STRING_VALUE_START );
    {
        /* prepare temp buf */
        strerr |= utf8stringbuf_copy_str( (*this_).private_temp_stringbuf, new_diagram_name );
        strerr |= utf8stringbuf_replace_all( (*this_).private_temp_stringbuf, DATA_DATABASE_WRITER_SQL_ENCODE );
    }
    strerr |= utf8stringbuf_append_buf( (*this_).private_sql_stringbuf, (*this_).private_temp_stringbuf );
    strerr |= utf8stringbuf_append_str( (*this_).private_sql_stringbuf, DATA_DATABASE_WRITER_STRING_VALUE_END );

    strerr |= utf8stringbuf_append_str( (*this_).private_sql_stringbuf, DATA_DATABASE_WRITER_UPDATE_DIAGRAM_INFIX );

    strerr |= utf8stringbuf_append_int( (*this_).private_sql_stringbuf, diagram_id );
    strerr |= utf8stringbuf_append_str( (*this_).private_sql_stringbuf, DATA_DATABASE_WRITER_UPDATE_DIAGRAM_POSTFIX );

    if ( strerr != UTF8ERROR_SUCCESS )
    {
        LOG_ERROR_HEX( "utf8stringbuf_xxx() failed:", strerr );
        result |= DATA_ERROR_STRING_BUFFER_EXCEEDED;
    }

    TRACE_END_ERR( result );
    return( result );
}

data_error_t data_database_writer_private_build_update_diagram_description_cmd ( data_database_writer_t *this_, int64_t diagram_id, const char *new_diagram_description )
{
    TRACE_BEGIN();
    utf8error_t strerr = UTF8ERROR_SUCCESS;
    data_error_t result = DATA_ERROR_NONE;

    strerr |= utf8stringbuf_copy_str( (*this_).private_sql_stringbuf, DATA_DATABASE_WRITER_UPDATE_DIAGRAM_PREFIX );
    strerr |= utf8stringbuf_append_str( (*this_).private_sql_stringbuf, DATA_DATABASE_WRITER_UPDATE_DIAGRAM_COL_DESCRIPTION );

    strerr |= utf8stringbuf_append_str( (*this_).private_sql_stringbuf, DATA_DATABASE_WRITER_STRING_VALUE_START );
    {
        /* prepare temp buf */
        strerr |= utf8stringbuf_copy_str( (*this_).private_temp_stringbuf, new_diagram_description );
        strerr |= utf8stringbuf_replace_all( (*this_).private_temp_stringbuf, DATA_DATABASE_WRITER_SQL_ENCODE );
    }
    strerr |= utf8stringbuf_append_buf( (*this_).private_sql_stringbuf, (*this_).private_temp_stringbuf );
    strerr |= utf8stringbuf_append_str( (*this_).private_sql_stringbuf, DATA_DATABASE_WRITER_STRING_VALUE_END );

    strerr |= utf8stringbuf_append_str( (*this_).private_sql_stringbuf, DATA_DATABASE_WRITER_UPDATE_DIAGRAM_INFIX );

    strerr |= utf8stringbuf_append_int( (*this_).private_sql_stringbuf, diagram_id );
    strerr |= utf8stringbuf_append_str( (*this_).private_sql_stringbuf, DATA_DATABASE_WRITER_UPDATE_DIAGRAM_POSTFIX );

    if ( strerr != UTF8ERROR_SUCCESS )
    {
        LOG_ERROR_HEX( "utf8stringbuf_xxx() failed:", strerr );
        result |= DATA_ERROR_STRING_BUFFER_EXCEEDED;
    }

    TRACE_END_ERR( result );
    return( result );
}

data_error_t data_database_writer_private_build_update_diagram_type_cmd ( data_database_writer_t *this_, int64_t diagram_id, data_diagram_type_t new_diagram_type )
{
    TRACE_BEGIN();
    utf8error_t strerr = UTF8ERROR_SUCCESS;
    data_error_t result = DATA_ERROR_NONE;

    strerr |= utf8stringbuf_copy_str( (*this_).private_sql_stringbuf, DATA_DATABASE_WRITER_UPDATE_DIAGRAM_PREFIX );
    strerr |= utf8stringbuf_append_str( (*this_).private_sql_stringbuf, DATA_DATABASE_WRITER_UPDATE_DIAGRAM_COL_TYPE );
    strerr |= utf8stringbuf_append_int( (*this_).private_sql_stringbuf, new_diagram_type );

    strerr |= utf8stringbuf_append_str( (*this_).private_sql_stringbuf, DATA_DATABASE_WRITER_UPDATE_DIAGRAM_INFIX );

    strerr |= utf8stringbuf_append_int( (*this_).private_sql_stringbuf, diagram_id );
    strerr |= utf8stringbuf_append_str( (*this_).private_sql_stringbuf, DATA_DATABASE_WRITER_UPDATE_DIAGRAM_POSTFIX );

    if ( strerr != UTF8ERROR_SUCCESS )
    {
        LOG_ERROR_HEX( "utf8stringbuf_xxx() failed:", strerr );
        result |= DATA_ERROR_STRING_BUFFER_EXCEEDED;
    }

    TRACE_END_ERR( result );
    return( result );
}

data_error_t data_database_writer_private_execute_single_command ( data_database_writer_t *this_, const char* sql_statement, bool fetch_new_id, int64_t* out_new_id )
{
    TRACE_BEGIN();
    int64_t new_id;
    data_error_t result = DATA_ERROR_NONE;
    int sqlite_err;
    char *error_msg = NULL;
    sqlite3 *db = data_database_get_database_ptr( (*this_).database );
    int perr;

    perr = pthread_mutex_lock ( &((*this_).private_lock) );
    if ( perr != 0 )
    {
        LOG_ERROR_INT( "pthread_mutex_lock() failed:", perr );
        result |= DATA_ERROR_AT_MUTEX;
    }

    LOG_EVENT_STR( "sqlite3_exec:", DATA_DATABASE_WRITER_BEGIN_TRANSACTION );
    sqlite_err = sqlite3_exec( db, DATA_DATABASE_WRITER_BEGIN_TRANSACTION, NULL, NULL, &error_msg );
    if ( SQLITE_OK != sqlite_err )
    {
        LOG_ERROR_STR( "sqlite3_exec() failed:", DATA_DATABASE_WRITER_BEGIN_TRANSACTION );
        LOG_ERROR_INT( "sqlite3_exec() failed:", sqlite_err );
        result |= DATA_ERROR_AT_DB;
    }
    if ( error_msg != NULL )
    {
        LOG_ERROR_STR( "sqlite3_exec() failed:", error_msg );
        sqlite3_free( error_msg );
        error_msg = NULL;
    }

    LOG_EVENT_STR( "sqlite3_exec:", sql_statement );
    sqlite_err = sqlite3_exec( db, sql_statement, NULL, NULL, &error_msg );
    if ( SQLITE_OK != sqlite_err )
    {
        LOG_ERROR_STR( "sqlite3_exec() failed:", sql_statement );
        LOG_ERROR_INT( "sqlite3_exec() failed:", sqlite_err );
        result |= DATA_ERROR_AT_DB;
    }
    if ( error_msg != NULL )
    {
        LOG_ERROR_STR( "sqlite3_exec() failed:", error_msg );
        sqlite3_free( error_msg );
        error_msg = NULL;
    }

    if ( fetch_new_id && ( NULL != out_new_id ))
    {
        if ( SQLITE_OK == sqlite_err )
        {
            new_id = sqlite3_last_insert_rowid(db);
            LOG_EVENT_INT( "sqlite3_last_insert_rowid():", new_id );
            *out_new_id = new_id;
        }
    }

    LOG_EVENT_STR( "sqlite3_exec:", DATA_DATABASE_WRITER_COMMIT_TRANSACTION );
    sqlite_err = sqlite3_exec( db, DATA_DATABASE_WRITER_COMMIT_TRANSACTION, NULL, NULL, &error_msg );
    if ( SQLITE_OK != sqlite_err )
    {
        LOG_ERROR_STR( "sqlite3_exec() failed:", DATA_DATABASE_WRITER_COMMIT_TRANSACTION );
        LOG_ERROR_INT( "sqlite3_exec() failed:", sqlite_err );
        result |= DATA_ERROR_AT_DB;
    }
    if ( error_msg != NULL )
    {
        LOG_ERROR_STR( "sqlite3_exec() failed:", error_msg );
        sqlite3_free( error_msg );
        error_msg = NULL;
    }

    perr = pthread_mutex_unlock ( &((*this_).private_lock) );
    if ( perr != 0 )
    {
        LOG_ERROR_INT( "pthread_mutex_unlock() failed:", perr );
        result |= DATA_ERROR_AT_MUTEX;
    }

    data_change_notifier_emit_signal( data_database_get_notifier_ptr( (*this_).database ) );

    TRACE_END_ERR( result );
    return result;
}

data_error_t data_database_writer_create_diagram ( data_database_writer_t *this_, const data_diagram_t *diagram, int64_t* out_new_id )
{
    TRACE_BEGIN();
    data_error_t result = DATA_ERROR_NONE;

    result |= data_database_writer_private_build_create_diagram_command( this_, diagram );

    result |= data_database_writer_private_execute_single_command( this_, utf8stringbuf_get_string( (*this_).private_sql_stringbuf ), true, out_new_id );

    data_change_notifier_emit_signal( data_database_get_notifier_ptr( (*this_).database ) );

    TRACE_END_ERR( result );
    return result;
}

data_error_t data_database_writer_update_diagram_description ( data_database_writer_t *this_, int64_t diagram_id, const char* new_diagram_description )
{
    TRACE_BEGIN();
    data_error_t result = DATA_ERROR_NONE;

    result |= data_database_writer_private_build_update_diagram_description_cmd( this_, diagram_id, new_diagram_description );

    result |= data_database_writer_private_execute_single_command( this_, utf8stringbuf_get_string( (*this_).private_sql_stringbuf ), false, NULL );

    data_change_notifier_emit_signal( data_database_get_notifier_ptr( (*this_).database ) );

    TRACE_END_ERR( result );
    return result;
}

data_error_t data_database_writer_update_diagram_name ( data_database_writer_t *this_, int64_t diagram_id, const char* new_diagram_name )
{
    TRACE_BEGIN();
    data_error_t result = DATA_ERROR_NONE;

    result |= data_database_writer_private_build_update_diagram_name_cmd( this_, diagram_id, new_diagram_name );

    result |= data_database_writer_private_execute_single_command( this_, utf8stringbuf_get_string( (*this_).private_sql_stringbuf ), false, NULL );

    data_change_notifier_emit_signal( data_database_get_notifier_ptr( (*this_).database ) );

    TRACE_END_ERR( result );
    return result;
}

data_error_t data_database_writer_update_diagram_type ( data_database_writer_t *this_, int64_t diagram_id, data_diagram_type_t new_diagram_type )
{
    TRACE_BEGIN();
    data_error_t result = DATA_ERROR_NONE;

    result |= data_database_writer_private_build_update_diagram_type_cmd( this_, diagram_id, new_diagram_type );

    result |= data_database_writer_private_execute_single_command( this_, utf8stringbuf_get_string( (*this_).private_sql_stringbuf ), false, NULL );

    data_change_notifier_emit_signal( data_database_get_notifier_ptr( (*this_).database ) );

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
