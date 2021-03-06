/* File: data_database_reader.c; Copyright and License: see below */

#include "storage/data_database_reader.h"
#include "trace.h"
#include "tslog.h"
#include "util/string/utf8stringbuf.h"
#include <sqlite3.h>
#include <assert.h>

data_error_t data_database_reader_init ( data_database_reader_t *this_, data_database_t *database )
{
    TRACE_BEGIN();
    assert( NULL != database );
    data_error_t result = DATA_ERROR_NONE;

    (*this_).database = database;
    (*this_).is_open = false;

    data_database_listener_init ( &((*this_).me_as_listener), this_, (void (*)(void*,data_database_listener_signal_t)) &data_database_reader_db_change_callback );
    data_database_add_db_listener( database, &((*this_).me_as_listener) );

    if ( data_database_is_open( database ) )
    {
        /* if the database is open, open also the reader */
        result |= data_database_reader_private_open( this_ );
    }

    TRACE_END_ERR(result);
    return result;
}

data_error_t data_database_reader_destroy ( data_database_reader_t *this_ )
{
    TRACE_BEGIN();
    data_error_t result = DATA_ERROR_NONE;

    if ( (*this_).is_open )
    {
        result |= data_database_reader_private_close( this_ );
    }

    data_database_remove_db_listener( (*this_).database, &((*this_).me_as_listener) );

    (*this_).database = NULL;

    TRACE_END_ERR(result);
    return result;
}

void data_database_reader_db_change_callback ( data_database_reader_t *this_, data_database_listener_signal_t signal_id )
{
    TRACE_BEGIN();
    data_error_t result = DATA_ERROR_NONE;

    switch ( signal_id )
    {
        case DATA_DATABASE_LISTENER_SIGNAL_PREPARE_CLOSE:
        {
            TRACE_INFO( "DATA_DATABASE_LISTENER_SIGNAL_PREPARE_CLOSE" );
            if ( (*this_).is_open )
            {
                result |= data_database_reader_private_close( this_ );
            }
        }
        break;

        case DATA_DATABASE_LISTENER_SIGNAL_DB_OPENED:
        {
            TRACE_INFO( "DATA_DATABASE_LISTENER_SIGNAL_DB_OPENED" );
            if ( (*this_).is_open )
            {
                result |= data_database_reader_private_close( this_ );
            }
            result |= data_database_reader_private_open( this_ );
        }
        break;

        default:
        {
            TSLOG_ERROR( "unexpected data_database_listener_signal_t" );
        }
    }

    TRACE_END();
}

/* ================================ DIAGRAM ================================ */

/*!
 *  \brief predefined search statement to find a diagram by id
 */
static const char DATA_DATABASE_READER_SELECT_DIAGRAM_BY_ID[] =
    "SELECT id,parent_id,diagram_type,name,description,list_order "
    "FROM diagrams WHERE id=?;";

/*!
 *  \brief predefined search statement to find diagrams by parent-id
 */
static const char DATA_DATABASE_READER_SELECT_DIAGRAMS_BY_PARENT_ID[] =
    "SELECT id,parent_id,diagram_type,name,description,list_order "
    "FROM diagrams WHERE parent_id=? ORDER BY list_order ASC;";

/*!
 *  \brief predefined search statement to find diagrams by NULL parent-id
 */
static const char DATA_DATABASE_READER_SELECT_DIAGRAMS_BY_PARENT_ID_NULL[] =
    "SELECT id,parent_id,diagram_type,name,description,list_order "
    "FROM diagrams WHERE parent_id IS NULL ORDER BY list_order ASC;";

#ifdef DATA_DATABASE_READER_PROVIDE_DEPRECATED_FKT
/*!
 *  \brief predefined search statement to find diagrams by classifier-id
 */
static const char DATA_DATABASE_READER_SELECT_DIAGRAMS_BY_CLASSIFIER_ID[] =
    "SELECT diagrams.id,diagrams.parent_id,diagrams.diagram_type,"
    "diagrams.name,diagrams.description,diagrams.list_order "
    "FROM diagrams "
    "INNER JOIN diagramelements ON diagramelements.diagram_id=diagrams.id "
    "WHERE diagramelements.classifier_id=? "
    "GROUP BY diagrams.id "  /* filter duplicates if a classifier exists twice in a diagram */
    "ORDER BY diagrams.list_order ASC;";
#endif

/*!
 *  \brief the column id of the result where this parameter is stored: id
 */
static const int RESULT_DIAGRAM_ID_COLUMN = 0;

/*!
 *  \brief the column id of the result where this parameter is stored: parent_id
 */
static const int RESULT_DIAGRAM_PARENT_ID_COLUMN = 1;

/*!
 *  \brief the column id of the result where this parameter is stored: type
 */
static const int RESULT_DIAGRAM_TYPE_COLUMN = 2;

/*!
 *  \brief the column id of the result where this parameter is stored: name
 */
static const int RESULT_DIAGRAM_NAME_COLUMN = 3;

/*!
 *  \brief the column id of the result where this parameter is stored: description
 */
static const int RESULT_DIAGRAM_DESCRIPTION_COLUMN = 4;

/*!
 *  \brief the column id of the result where this parameter is stored: list_order
 */
static const int RESULT_DIAGRAM_LIST_ORDER_COLUMN = 5;

/*!
 *  \brief predefined search statement to find diagram ids by parent-id
 */
static const char DATA_DATABASE_READER_SELECT_DIAGRAM_IDS_BY_PARENT_ID[] =
    "SELECT id "
    "FROM diagrams WHERE parent_id=? ORDER BY list_order ASC;";

/*!
 *  \brief predefined search statement to find diagram ids by NULL parent-id
 */
static const char DATA_DATABASE_READER_SELECT_DIAGRAM_IDS_BY_PARENT_ID_NULL[] =
    "SELECT id "
    "FROM diagrams WHERE parent_id IS NULL ORDER BY list_order ASC;";

/*!
 *  \brief predefined search statement to find diagram ids by classifier-id
 */
static const char DATA_DATABASE_READER_SELECT_DIAGRAM_IDS_BY_CLASSIFIER_ID[] =
    "SELECT diagrams.id "
    "FROM diagrams "
    "INNER JOIN diagramelements ON diagramelements.diagram_id=diagrams.id "
    "WHERE diagramelements.classifier_id=? "
    "GROUP BY diagrams.id "  /* filter duplicates if a classifier exists twice in a diagram */
    "ORDER BY diagrams.list_order ASC;";

data_error_t data_database_reader_get_diagram_by_id ( data_database_reader_t *this_, data_row_id_t id, data_diagram_t *out_diagram )
{
    TRACE_BEGIN();
    assert( NULL != out_diagram );
    data_error_t result = DATA_ERROR_NONE;
    int sqlite_err;
    sqlite3_stmt *prepared_statement;

    if ( (*this_).is_open )
    {
        prepared_statement = (*this_).private_prepared_query_diagram_by_id;

        result |= data_database_reader_private_bind_id_to_statement( this_, prepared_statement, id );

        TRACE_INFO( "sqlite3_step()" );
        sqlite_err = sqlite3_step( prepared_statement );
        if ( SQLITE_ROW != sqlite_err )
        {
            TSLOG_ANOMALY( "sqlite3_step did not find a row." );
            result |= DATA_ERROR_DB_STRUCTURE;
        }

        if ( SQLITE_ROW == sqlite_err )
        {
            result |= data_diagram_init( out_diagram,
                                         sqlite3_column_int64( prepared_statement, RESULT_DIAGRAM_ID_COLUMN ),
                                         sqlite3_column_int64( prepared_statement, RESULT_DIAGRAM_PARENT_ID_COLUMN ),
                                         sqlite3_column_int( prepared_statement, RESULT_DIAGRAM_TYPE_COLUMN ),
                                         (const char*) sqlite3_column_text( prepared_statement, RESULT_DIAGRAM_NAME_COLUMN ),
                                         (const char*) sqlite3_column_text( prepared_statement, RESULT_DIAGRAM_DESCRIPTION_COLUMN ),
                                         sqlite3_column_int( prepared_statement, RESULT_DIAGRAM_LIST_ORDER_COLUMN )
            );
            if ( SQLITE_NULL == sqlite3_column_type( prepared_statement, RESULT_DIAGRAM_PARENT_ID_COLUMN ) )
            {
                (*out_diagram).parent_id = DATA_ROW_ID_VOID;
            }

            data_diagram_trace( out_diagram );
        }

        sqlite_err = sqlite3_step( prepared_statement );
        if ( SQLITE_DONE != sqlite_err )
        {
            TSLOG_ERROR_INT( "sqlite3_step failed:", sqlite_err );
            result |= DATA_ERROR_DB_STRUCTURE;
        }
    }
    else
    {
        result |= DATA_ERROR_NO_DB;
        TRACE_INFO( "Database not open, cannot request data." );
    }

    TRACE_END_ERR( result );
    return result;
}

data_error_t data_database_reader_get_diagrams_by_parent_id ( data_database_reader_t *this_,
                                                              data_row_id_t parent_id,
                                                              uint32_t max_out_array_size,
                                                              data_diagram_t (*out_diagram)[],
                                                              uint32_t *out_diagram_count )
{
    TRACE_BEGIN();
    assert( NULL != out_diagram_count );
    assert( NULL != out_diagram );
    data_error_t result = DATA_ERROR_NONE;
    int sqlite_err;
    sqlite3_stmt *prepared_statement;

    if ( (*this_).is_open )
    {
        if ( DATA_ROW_ID_VOID == parent_id )
        {
            prepared_statement = (*this_).private_prepared_query_diagrams_by_parent_id_null;
            result |= data_database_reader_private_bind_void_to_statement( this_, prepared_statement );
        }
        else
        {
            prepared_statement = (*this_).private_prepared_query_diagrams_by_parent_id;
            result |= data_database_reader_private_bind_id_to_statement( this_, prepared_statement, parent_id );
        }

        *out_diagram_count = 0;
        sqlite_err = SQLITE_ROW;
        for ( uint32_t row_index = 0; (SQLITE_ROW == sqlite_err) && (row_index <= max_out_array_size); row_index ++ )
        {
            TRACE_INFO( "sqlite3_step()" );
            sqlite_err = sqlite3_step( prepared_statement );
            if (( SQLITE_ROW != sqlite_err )&&( SQLITE_DONE != sqlite_err ))
            {
                TSLOG_ERROR_INT( "sqlite3_step failed:", sqlite_err );
                result |= DATA_ERROR_AT_DB;
            }
            if (( SQLITE_ROW == sqlite_err )&&(row_index < max_out_array_size))
            {
                *out_diagram_count = row_index+1;
                data_diagram_t *current_diag = &((*out_diagram)[row_index]);

                result |= data_diagram_init( current_diag,
                                             sqlite3_column_int64( prepared_statement, RESULT_DIAGRAM_ID_COLUMN ),
                                             sqlite3_column_int64( prepared_statement, RESULT_DIAGRAM_PARENT_ID_COLUMN ),
                                             sqlite3_column_int( prepared_statement, RESULT_DIAGRAM_TYPE_COLUMN ),
                                             (const char*) sqlite3_column_text( prepared_statement, RESULT_DIAGRAM_NAME_COLUMN ),
                                             (const char*) sqlite3_column_text( prepared_statement, RESULT_DIAGRAM_DESCRIPTION_COLUMN ),
                                             sqlite3_column_int( prepared_statement, RESULT_DIAGRAM_LIST_ORDER_COLUMN )
                );
                if ( SQLITE_NULL == sqlite3_column_type( prepared_statement, RESULT_DIAGRAM_PARENT_ID_COLUMN ) )
                {
                    (*current_diag).parent_id = DATA_ROW_ID_VOID;
                }

                data_diagram_trace( current_diag );
            }
            if (( SQLITE_ROW == sqlite_err )&&(row_index >= max_out_array_size))
            {
                TSLOG_ANOMALY_INT( "out_diagram[] full:", (row_index+1) );
                result |= DATA_ERROR_ARRAY_BUFFER_EXCEEDED;
            }
            if ( SQLITE_DONE == sqlite_err )
            {
                TRACE_INFO( "sqlite3_step finished: SQLITE_DONE" );
            }
        }
    }
    else
    {
        result |= DATA_ERROR_NO_DB;
        TRACE_INFO( "Database not open, cannot request data." );
    }

    TRACE_END_ERR( result );
    return result;
}

#ifdef DATA_DATABASE_READER_PROVIDE_DEPRECATED_FKT
data_error_t data_database_reader_get_diagrams_by_classifier_id ( data_database_reader_t *this_,
                                                                  data_row_id_t classifier_id,
                                                                  uint32_t max_out_array_size,
                                                                  data_diagram_t (*out_diagram)[],
                                                                  uint32_t *out_diagram_count )
{
    TRACE_BEGIN();
    assert( NULL != out_diagram_count );
    assert( NULL != out_diagram );
    data_error_t result = DATA_ERROR_NONE;
    int sqlite_err;
    sqlite3_stmt *prepared_statement;

    if ( (*this_).is_open )
    {
        prepared_statement = (*this_).private_prepared_query_diagrams_by_classifier_id;

        result |= data_database_reader_private_bind_id_to_statement( this_, prepared_statement, classifier_id );

        *out_diagram_count = 0;
        sqlite_err = SQLITE_ROW;
        for ( uint32_t row_index = 0; (SQLITE_ROW == sqlite_err) && (row_index <= max_out_array_size); row_index ++ )
        {
            TRACE_INFO( "sqlite3_step()" );
            sqlite_err = sqlite3_step( prepared_statement );
            if (( SQLITE_ROW != sqlite_err )&&( SQLITE_DONE != sqlite_err ))
            {
                TSLOG_ERROR_INT( "sqlite3_step failed:", sqlite_err );
                result |= DATA_ERROR_AT_DB;
            }
            if (( SQLITE_ROW == sqlite_err )&&(row_index < max_out_array_size))
            {
                *out_diagram_count = row_index+1;
                data_diagram_t *current_diag = &((*out_diagram)[row_index]);

                result |= data_diagram_init( current_diag,
                                             sqlite3_column_int64( prepared_statement, RESULT_DIAGRAM_ID_COLUMN ),
                                             sqlite3_column_int64( prepared_statement, RESULT_DIAGRAM_PARENT_ID_COLUMN ),
                                             sqlite3_column_int( prepared_statement, RESULT_DIAGRAM_TYPE_COLUMN ),
                                             (const char*) sqlite3_column_text( prepared_statement, RESULT_DIAGRAM_NAME_COLUMN ),
                                             (const char*) sqlite3_column_text( prepared_statement, RESULT_DIAGRAM_DESCRIPTION_COLUMN ),
                                             sqlite3_column_int( prepared_statement, RESULT_DIAGRAM_LIST_ORDER_COLUMN )
                );
                if ( SQLITE_NULL == sqlite3_column_type( prepared_statement, RESULT_DIAGRAM_PARENT_ID_COLUMN ) )
                {
                    (*current_diag).parent_id = DATA_ROW_ID_VOID;
                }

                data_diagram_trace( current_diag );
            }
            if (( SQLITE_ROW == sqlite_err )&&(row_index >= max_out_array_size))
            {
                TSLOG_ANOMALY_INT( "out_diagram[] full:", (row_index+1) );
                result |= DATA_ERROR_ARRAY_BUFFER_EXCEEDED;
            }
            if ( SQLITE_DONE == sqlite_err )
            {
                TRACE_INFO( "sqlite3_step finished: SQLITE_DONE" );
            }
        }
    }
    else
    {
        result |= DATA_ERROR_NO_DB;
        TRACE_INFO( "Database not open, cannot request data." );
    }

    TRACE_END_ERR( result );
    return result;
}
#endif

data_error_t data_database_reader_get_diagram_ids_by_parent_id ( data_database_reader_t *this_,
                                                                 data_row_id_t parent_id,
                                                                 data_small_set_t *out_diagram_ids )
{
    TRACE_BEGIN();
    assert( NULL != out_diagram_ids );
    data_error_t result = DATA_ERROR_NONE;
    int sqlite_err;
    sqlite3_stmt *prepared_statement;

    if ( (*this_).is_open )
    {
        if ( DATA_ROW_ID_VOID == parent_id )
        {
            prepared_statement = (*this_).private_prepared_query_diagram_ids_by_parent_id_null;
            result |= data_database_reader_private_bind_void_to_statement( this_, prepared_statement );
        }
        else
        {
            prepared_statement = (*this_).private_prepared_query_diagram_ids_by_parent_id;
            result |= data_database_reader_private_bind_id_to_statement( this_, prepared_statement, parent_id );
        }

        data_small_set_clear( out_diagram_ids );
        sqlite_err = SQLITE_ROW;
        for ( uint32_t row_index = 0; (SQLITE_ROW == sqlite_err) && (row_index <= DATA_SMALL_SET_MAX_SET_SIZE); row_index ++ )
        {
            TRACE_INFO( "sqlite3_step()" );
            sqlite_err = sqlite3_step( prepared_statement );
            if (( SQLITE_ROW != sqlite_err )&&( SQLITE_DONE != sqlite_err ))
            {
                TSLOG_ERROR_INT( "sqlite3_step failed:", sqlite_err );
                result |= DATA_ERROR_AT_DB;
            }
            if (( SQLITE_ROW == sqlite_err )&&(row_index < DATA_SMALL_SET_MAX_SET_SIZE))
            {
                data_id_t current_diag_id;
                data_id_init( &current_diag_id, DATA_TABLE_DIAGRAM, sqlite3_column_int64( prepared_statement, RESULT_DIAGRAM_ID_COLUMN ) );
                result |= data_small_set_add_obj( out_diagram_ids, current_diag_id );
            }
            if (( SQLITE_ROW == sqlite_err )&&(row_index >= DATA_SMALL_SET_MAX_SET_SIZE))
            {
                TSLOG_ANOMALY_INT( "out_diagram_ids[] full:", (row_index+1) );
                result |= DATA_ERROR_ARRAY_BUFFER_EXCEEDED;
            }
            if ( SQLITE_DONE == sqlite_err )
            {
                TRACE_INFO( "sqlite3_step finished: SQLITE_DONE" );
            }
        }
        data_small_set_trace( out_diagram_ids );
    }
    else
    {
        result |= DATA_ERROR_NO_DB;
        TRACE_INFO( "Database not open, cannot request data." );
    }

    TRACE_END_ERR( result );
    return result;
}

data_error_t data_database_reader_get_diagram_ids_by_classifier_id ( data_database_reader_t *this_,
                                                                     data_row_id_t classifier_id,
                                                                     data_small_set_t *out_diagram_ids )
{
    TRACE_BEGIN();
    assert( NULL != out_diagram_ids );
    data_error_t result = DATA_ERROR_NONE;
    int sqlite_err;
    sqlite3_stmt *prepared_statement;

    if ( (*this_).is_open )
    {
        prepared_statement = (*this_).private_prepared_query_diagram_ids_by_classifier_id;

        result |= data_database_reader_private_bind_id_to_statement( this_, prepared_statement, classifier_id );

        sqlite_err = SQLITE_ROW;
        for ( uint32_t row_index = 0; (SQLITE_ROW == sqlite_err) && (row_index <= DATA_SMALL_SET_MAX_SET_SIZE); row_index ++ )
        {
            TRACE_INFO( "sqlite3_step()" );
            sqlite_err = sqlite3_step( prepared_statement );
            if (( SQLITE_ROW != sqlite_err )&&( SQLITE_DONE != sqlite_err ))
            {
                TSLOG_ERROR_INT( "sqlite3_step failed:", sqlite_err );
                result |= DATA_ERROR_AT_DB;
            }
            if (( SQLITE_ROW == sqlite_err )&&(row_index < DATA_SMALL_SET_MAX_SET_SIZE))
            {
                data_id_t current_diag_id;
                data_id_init( &current_diag_id, DATA_TABLE_DIAGRAM, sqlite3_column_int64( prepared_statement, RESULT_DIAGRAM_ID_COLUMN ) );
                result |= data_small_set_add_obj( out_diagram_ids, current_diag_id );
                data_id_trace( &current_diag_id );
            }
            if (( SQLITE_ROW == sqlite_err )&&(row_index >= DATA_SMALL_SET_MAX_SET_SIZE))
            {
                TSLOG_ANOMALY_INT( "out_diagram_ids[] full:", (row_index+1) );
                result |= DATA_ERROR_ARRAY_BUFFER_EXCEEDED;
            }
            if ( SQLITE_DONE == sqlite_err )
            {
                TRACE_INFO( "sqlite3_step finished: SQLITE_DONE" );
            }
        }
    }
    else
    {
        result |= DATA_ERROR_NO_DB;
        TRACE_INFO( "Database not open, cannot request data." );
    }

    TRACE_END_ERR( result );
    return result;
}

/* ================================ CLASSIFIER ================================ */

/*!
 *  \brief predefined search statement to find a classifier by id
 */
static const char DATA_DATABASE_READER_SELECT_CLASSIFIER_BY_ID[] =
    "SELECT id,main_type,stereotype,name,description,x_order,y_order,list_order "
    "FROM classifiers WHERE id=?;";

/*!
 *  \brief predefined search statement to find a classifier by name
 */
static const char DATA_DATABASE_READER_SELECT_CLASSIFIER_BY_NAME[] =
    "SELECT id,main_type,stereotype,name,description,x_order,y_order,list_order "
    "FROM classifiers WHERE name=?;";

/*!
 *  \brief predefined search statement to find classifier by diagram-id
 */
static const char DATA_DATABASE_READER_SELECT_CLASSIFIERS_BY_DIAGRAM_ID[] =
    "SELECT classifiers.id,classifiers.main_type,classifiers.stereotype,"
    "classifiers.name,classifiers.description,classifiers.x_order,classifiers.y_order,classifiers.list_order,"
    "diagramelements.id,diagramelements.display_flags,diagramelements.focused_feature_id "
    "FROM classifiers "
    "INNER JOIN diagramelements ON diagramelements.classifier_id=classifiers.id "
    "WHERE diagramelements.diagram_id=? "
    "ORDER BY classifiers.list_order ASC;";

/*!
 *  \brief search statement to iterate over all classifiers sorted by number of parent-containers
 */
static const char DATA_DATABASE_READER_SELECT_ALL_CLASSIFIERS[] =
    "SELECT id,main_type,stereotype,name,description,x_order,y_order,list_order,"
        "(SELECT count(*) FROM relationships "
        "WHERE (relationships.to_classifier_id=classifiers.id) AND (relationships.to_feature_id IS NULL) "
        "AND (relationships.main_type=300)) AS cnt "
    "FROM classifiers "
    "ORDER BY cnt ASC;";

/*!
 *  \brief the column id of the result where this parameter is stored: id
 */
static const int RESULT_CLASSIFIER_ID_COLUMN = 0;

/*!
 *  \brief the column id of the result where this parameter is stored: main_type
 */
static const int RESULT_CLASSIFIER_MAIN_TYPE_COLUMN = 1;

/*!
 *  \brief the column id of the result where this parameter is stored: stereotype
 */
static const int RESULT_CLASSIFIER_STEREOTYPE_COLUMN = 2;

/*!
 *  \brief the column id of the result where this parameter is stored: name
 */
static const int RESULT_CLASSIFIER_NAME_COLUMN = 3;

/*!
 *  \brief the column id of the result where this parameter is stored: description
 */
static const int RESULT_CLASSIFIER_DESCRIPTION_COLUMN = 4;

/*!
 *  \brief the column id of the result where this parameter is stored: x_order
 */
static const int RESULT_CLASSIFIER_X_ORDER_COLUMN = 5;

/*!
 *  \brief the column id of the result where this parameter is stored: y_order
 */
static const int RESULT_CLASSIFIER_Y_ORDER_COLUMN = 6;

/*!
 *  \brief the column id of the result where this parameter is stored: list_order
 */
static const int RESULT_CLASSIFIER_LIST_ORDER_COLUMN = 7;

/*!
 *  \brief the column id of the result where this parameter is stored: diagramelements.id
 */
static const int RESULT_CLASSIFIER_DIAGRAMELEMENT_ID_COLUMN = 8;

/*!
 *  \brief the column id of the result where this parameter is stored: diagramelements.display_flags
 */
static const int RESULT_CLASSIFIER_DISPLAY_FLAGS_COLUMN = 9;

/*!
 *  \brief the column id of the result where this parameter is stored: diagramelements.focused_feature_id
 */
static const int RESULT_CLASSIFIER_FOCUSED_FEATURE_ID_COLUMN = 10;

data_error_t data_database_reader_get_classifier_by_id ( data_database_reader_t *this_, data_row_id_t id, data_classifier_t *out_classifier )
{
    TRACE_BEGIN();
    assert( NULL != out_classifier );
    data_error_t result = DATA_ERROR_NONE;
    int sqlite_err;
    sqlite3_stmt *prepared_statement;

    if ( (*this_).is_open )
    {
        prepared_statement = (*this_).private_prepared_query_classifier_by_id;

        result |= data_database_reader_private_bind_id_to_statement( this_, prepared_statement, id );

        TRACE_INFO( "sqlite3_step()" );
        sqlite_err = sqlite3_step( prepared_statement );
        if ( SQLITE_ROW != sqlite_err )
        {
            TSLOG_ANOMALY( "sqlite3_step did not find a row." );
            result |= DATA_ERROR_DB_STRUCTURE;
        }

        if ( SQLITE_ROW == sqlite_err )
        {
            result |= data_classifier_init( out_classifier,
                                            sqlite3_column_int64( prepared_statement, RESULT_CLASSIFIER_ID_COLUMN ),
                                            sqlite3_column_int( prepared_statement, RESULT_CLASSIFIER_MAIN_TYPE_COLUMN ),
                                            (const char*) sqlite3_column_text( prepared_statement, RESULT_CLASSIFIER_STEREOTYPE_COLUMN ),
                                            (const char*) sqlite3_column_text( prepared_statement, RESULT_CLASSIFIER_NAME_COLUMN ),
                                            (const char*) sqlite3_column_text( prepared_statement, RESULT_CLASSIFIER_DESCRIPTION_COLUMN ),
                                            sqlite3_column_int( prepared_statement, RESULT_CLASSIFIER_X_ORDER_COLUMN ),
                                            sqlite3_column_int( prepared_statement, RESULT_CLASSIFIER_Y_ORDER_COLUMN ),
                                            sqlite3_column_int( prepared_statement, RESULT_CLASSIFIER_LIST_ORDER_COLUMN )
                                          );

            data_classifier_trace( out_classifier );
        }

        sqlite_err = sqlite3_step( prepared_statement );
        if ( SQLITE_DONE != sqlite_err )
        {
            TSLOG_ERROR_INT( "sqlite3_step failed:", sqlite_err );
            result |= DATA_ERROR_DB_STRUCTURE;
        }
    }
    else
    {
        result |= DATA_ERROR_NO_DB;
        TRACE_INFO( "Database not open, cannot request data." );
    }

    TRACE_END_ERR( result );
    return result;
}

data_error_t data_database_reader_get_classifier_by_name ( data_database_reader_t *this_, const char *name, data_classifier_t *out_classifier )
{
    TRACE_BEGIN();
    assert( NULL != out_classifier );
    assert( NULL != name );
    data_error_t result = DATA_ERROR_NONE;
    int sqlite_err;
    sqlite3_stmt *prepared_statement;

    if ( (*this_).is_open )
    {
        prepared_statement = (*this_).private_prepared_query_classifier_by_name;

        result |= data_database_reader_private_bind_text_to_statement( this_, prepared_statement, name );

        TRACE_INFO( "sqlite3_step()" );
        sqlite_err = sqlite3_step( prepared_statement );
        if ( SQLITE_ROW != sqlite_err )
        {
            TSLOG_EVENT( "sqlite3_step did not find a row." );
            result |= DATA_ERROR_DB_STRUCTURE;
        }

        if ( SQLITE_ROW == sqlite_err )
        {
            result |= data_classifier_init( out_classifier,
                                            sqlite3_column_int64( prepared_statement, RESULT_CLASSIFIER_ID_COLUMN ),
                                            sqlite3_column_int( prepared_statement, RESULT_CLASSIFIER_MAIN_TYPE_COLUMN ),
                                            (const char*) sqlite3_column_text( prepared_statement, RESULT_CLASSIFIER_STEREOTYPE_COLUMN ),
                                            (const char*) sqlite3_column_text( prepared_statement, RESULT_CLASSIFIER_NAME_COLUMN ),
                                            (const char*) sqlite3_column_text( prepared_statement, RESULT_CLASSIFIER_DESCRIPTION_COLUMN ),
                                            sqlite3_column_int( prepared_statement, RESULT_CLASSIFIER_X_ORDER_COLUMN ),
                                            sqlite3_column_int( prepared_statement, RESULT_CLASSIFIER_Y_ORDER_COLUMN ),
                                            sqlite3_column_int( prepared_statement, RESULT_CLASSIFIER_LIST_ORDER_COLUMN )
                                          );

            data_classifier_trace( out_classifier );
        }

        sqlite_err = sqlite3_step( prepared_statement );
        if ( SQLITE_DONE != sqlite_err )
        {
            TSLOG_ERROR_INT( "sqlite3_step failed:", sqlite_err );
            result |= DATA_ERROR_DB_STRUCTURE;
        }
    }
    else
    {
        result |= DATA_ERROR_NO_DB;
        TRACE_INFO( "Database not open, cannot request data." );
    }

    TRACE_END_ERR( result );
    return result;
}

data_error_t data_database_reader_get_classifiers_by_diagram_id ( data_database_reader_t *this_,
                                                                  data_row_id_t diagram_id,
                                                                  uint32_t max_out_array_size,
                                                                  data_visible_classifier_t (*out_visible_classifier)[],
                                                                  uint32_t *out_visible_classifier_count )
{
    TRACE_BEGIN();
    assert( NULL != out_visible_classifier_count );
    assert( NULL != out_visible_classifier );
    data_error_t result = DATA_ERROR_NONE;
    int sqlite_err;
    sqlite3_stmt *prepared_statement;

    if ( (*this_).is_open )
    {
        prepared_statement = (*this_).private_prepared_query_classifiers_by_diagram_id;

        result |= data_database_reader_private_bind_id_to_statement( this_, prepared_statement, diagram_id );

        *out_visible_classifier_count = 0;
        sqlite_err = SQLITE_ROW;
        for ( uint32_t row_index = 0; (SQLITE_ROW == sqlite_err) && (row_index <= max_out_array_size); row_index ++ )
        {
            TRACE_INFO( "sqlite3_step()" );
            sqlite_err = sqlite3_step( prepared_statement );
            if (( SQLITE_ROW != sqlite_err )&&( SQLITE_DONE != sqlite_err ))
            {
                TSLOG_ERROR_INT( "sqlite3_step failed:", sqlite_err );
                result |= DATA_ERROR_AT_DB;
            }
            if (( SQLITE_ROW == sqlite_err )&&(row_index < max_out_array_size))
            {
                *out_visible_classifier_count = row_index+1;
                data_visible_classifier_t *current_vis_classifier;
                current_vis_classifier = &((*out_visible_classifier)[row_index]);
                data_visible_classifier_init_empty( current_vis_classifier );

                data_classifier_t *current_classifier;
                current_classifier = data_visible_classifier_get_classifier_ptr( current_vis_classifier );
                data_row_id_t classifier_id = sqlite3_column_int64( prepared_statement, RESULT_CLASSIFIER_ID_COLUMN );
                result |= data_classifier_reinit( current_classifier,
                                                  classifier_id,
                                                  sqlite3_column_int( prepared_statement, RESULT_CLASSIFIER_MAIN_TYPE_COLUMN ),
                                                  (const char*) sqlite3_column_text( prepared_statement, RESULT_CLASSIFIER_STEREOTYPE_COLUMN ),
                                                  (const char*) sqlite3_column_text( prepared_statement, RESULT_CLASSIFIER_NAME_COLUMN ),
                                                  (const char*) sqlite3_column_text( prepared_statement, RESULT_CLASSIFIER_DESCRIPTION_COLUMN ),
                                                  sqlite3_column_int( prepared_statement, RESULT_CLASSIFIER_X_ORDER_COLUMN ),
                                                  sqlite3_column_int( prepared_statement, RESULT_CLASSIFIER_Y_ORDER_COLUMN ),
                                                  sqlite3_column_int( prepared_statement, RESULT_CLASSIFIER_LIST_ORDER_COLUMN )
                                                );

                data_diagramelement_t *current_diag_element;
                current_diag_element = data_visible_classifier_get_diagramelement_ptr( current_vis_classifier );
                data_diagramelement_reinit ( current_diag_element,
                                             sqlite3_column_int64( prepared_statement, RESULT_CLASSIFIER_DIAGRAMELEMENT_ID_COLUMN ),
                                             diagram_id,
                                             classifier_id,
                                             sqlite3_column_int64( prepared_statement, RESULT_CLASSIFIER_DISPLAY_FLAGS_COLUMN ),
                                             sqlite3_column_int64( prepared_statement, RESULT_CLASSIFIER_FOCUSED_FEATURE_ID_COLUMN )
                                           );
                if ( SQLITE_NULL == sqlite3_column_type( prepared_statement, RESULT_CLASSIFIER_FOCUSED_FEATURE_ID_COLUMN ) )
                {
                    data_diagramelement_set_focused_feature_row_id ( current_diag_element, DATA_ROW_ID_VOID );
                }

                data_classifier_trace( current_classifier );
                data_diagramelement_trace( current_diag_element );
            }
            if (( SQLITE_ROW == sqlite_err )&&(row_index >= max_out_array_size))
            {
                TSLOG_ANOMALY_INT( "out_visible_classifier[] full:", (row_index+1) );
                result |= DATA_ERROR_ARRAY_BUFFER_EXCEEDED;
            }
            if ( SQLITE_DONE == sqlite_err )
            {
                TRACE_INFO( "sqlite3_step finished: SQLITE_DONE" );
            }
        }
    }
    else
    {
        result |= DATA_ERROR_NO_DB;
        TRACE_INFO( "Database not open, cannot request data." );
    }

    TRACE_END_ERR( result );
    return result;
}

data_error_t data_database_reader_get_all_classifiers_iterator ( data_database_reader_t *this_,
                                                                 data_database_iterator_classifiers_t *io_classifier_iterator
                                                               )
{
    TRACE_BEGIN();
    assert( NULL != io_classifier_iterator );
    data_error_t result = DATA_ERROR_NONE;

    if ( (*this_).is_open )
    {
        sqlite3_stmt *prepared_statement;
        /* prepare a statement */
        result |= data_database_reader_private_prepare_statement ( this_,
                                                                   DATA_DATABASE_READER_SELECT_ALL_CLASSIFIERS,
                                                                   sizeof( DATA_DATABASE_READER_SELECT_ALL_CLASSIFIERS ),
                                                                   &prepared_statement
                                                                 );
        if ( result == DATA_ERROR_NONE )
        {
            result |= data_database_iterator_classifiers_reinit ( io_classifier_iterator, (*this_).database, prepared_statement );
        }
    }
    else
    {
        result |= DATA_ERROR_NO_DB;
        TRACE_INFO( "Database not open, cannot request data." );
    }

    TRACE_END_ERR( result );
    return result;
}

/* ================================ DIAGRAMELEMENT ================================ */

/*!
 *  \brief predefined search statement to find a diagramelement by id
 */
static const char DATA_DATABASE_READER_SELECT_DIAGRAMELEMENT_BY_ID[] =
    "SELECT id,diagram_id,classifier_id,display_flags,focused_feature_id FROM diagramelements WHERE id=?;";

/*!
 *  \brief predefined search statement to find diagramelements by diagram id
 */
static const char DATA_DATABASE_READER_SELECT_DIAGRAMELEMENTS_BY_DIAGRAM_ID[] =
    "SELECT id,diagram_id,classifier_id,display_flags,focused_feature_id FROM diagramelements WHERE diagram_id=?;";

/*!
 *  \brief predefined search statement to find diagramelements by classifier id
 */
static const char DATA_DATABASE_READER_SELECT_DIAGRAMELEMENTS_BY_CLASSIFIER_ID[] =
    "SELECT id,diagram_id,classifier_id,display_flags,focused_feature_id FROM diagramelements WHERE classifier_id=?;";

/*!
 *  \brief the column id of the result where this parameter is stored: id
 */
static const int RESULT_DIAGRAMELEMENT_ID_COLUMN = 0;

/*!
 *  \brief the column id of the result where this parameter is stored: diagram_id
 */
static const int RESULT_DIAGRAMELEMENT_DIAGRAM_ID_COLUMN = 1;

/*!
 *  \brief the column id of the result where this parameter is stored: classifier_id
 */
static const int RESULT_DIAGRAMELEMENT_CLASSIFIER_ID_COLUMN = 2;

/*!
 *  \brief the column id of the result where this parameter is stored: display_flags
 */
static const int RESULT_DIAGRAMELEMENT_DISPLAY_FLAGS_COLUMN = 3;

/*!
 *  \brief the column id of the result where this parameter is stored: focused_feature_id
 */
static const int RESULT_DIAGRAMELEMENT_FOCUSED_FEATURE_ID_COLUMN = 4;

data_error_t data_database_reader_get_diagramelement_by_id ( data_database_reader_t *this_, data_row_id_t id, data_diagramelement_t *out_diagramelement )
{
    TRACE_BEGIN();
    assert( NULL != out_diagramelement );
    data_error_t result = DATA_ERROR_NONE;
    int sqlite_err;
    sqlite3_stmt *prepared_statement;

    if ( (*this_).is_open )
    {
        prepared_statement = (*this_).private_prepared_query_diagramelement_by_id;

        result |= data_database_reader_private_bind_id_to_statement( this_, prepared_statement, id );

        TRACE_INFO( "sqlite3_step()" );
        sqlite_err = sqlite3_step( prepared_statement );
        if ( SQLITE_ROW != sqlite_err )
        {
            TSLOG_ANOMALY( "sqlite3_step did not find a row." );
            result |= DATA_ERROR_DB_STRUCTURE;
        }

        if ( SQLITE_ROW == sqlite_err )
        {
            data_diagramelement_init( out_diagramelement,
                                      sqlite3_column_int64( prepared_statement, RESULT_DIAGRAMELEMENT_ID_COLUMN ),
                                      sqlite3_column_int64( prepared_statement, RESULT_DIAGRAMELEMENT_DIAGRAM_ID_COLUMN ),
                                      sqlite3_column_int64( prepared_statement, RESULT_DIAGRAMELEMENT_CLASSIFIER_ID_COLUMN ),
                                      sqlite3_column_int64( prepared_statement, RESULT_DIAGRAMELEMENT_DISPLAY_FLAGS_COLUMN ),
                                      sqlite3_column_int64( prepared_statement, RESULT_DIAGRAMELEMENT_FOCUSED_FEATURE_ID_COLUMN )
            );
            if ( SQLITE_NULL == sqlite3_column_type( prepared_statement, RESULT_DIAGRAMELEMENT_FOCUSED_FEATURE_ID_COLUMN ) )
            {
                data_diagramelement_set_focused_feature_row_id ( out_diagramelement, DATA_ROW_ID_VOID );
            }

            data_diagramelement_trace( out_diagramelement );
        }

        sqlite_err = sqlite3_step( prepared_statement );
        if ( SQLITE_DONE != sqlite_err )
        {
            TSLOG_ERROR_INT( "sqlite3_step failed:", sqlite_err );
            result |= DATA_ERROR_DB_STRUCTURE;
        }
    }
    else
    {
        result |= DATA_ERROR_NO_DB;
        TRACE_INFO( "Database not open, cannot request data." );
    }

    TRACE_END_ERR( result );
    return result;
}

data_error_t data_database_reader_get_diagramelements_by_diagram_id ( data_database_reader_t *this_,
                                                                      data_row_id_t diagram_id,
                                                                      uint32_t max_out_array_size,
                                                                      data_diagramelement_t (*out_diagramelement)[],
                                                                      uint32_t *out_diagramelement_count )
{
    TRACE_BEGIN();
    assert( NULL != out_diagramelement_count );
    assert( NULL != out_diagramelement );
    data_error_t result = DATA_ERROR_NONE;
    int sqlite_err;
    sqlite3_stmt *prepared_statement;

    if ( (*this_).is_open )
    {
        prepared_statement = (*this_).private_prepared_query_diagramelements_by_diagram_id;

        result |= data_database_reader_private_bind_id_to_statement( this_, prepared_statement, diagram_id );

        *out_diagramelement_count = 0;
        sqlite_err = SQLITE_ROW;
        for ( uint32_t row_index = 0; (SQLITE_ROW == sqlite_err) && (row_index <= max_out_array_size); row_index ++ )
        {
            TRACE_INFO( "sqlite3_step()" );
            sqlite_err = sqlite3_step( prepared_statement );
            if (( SQLITE_ROW != sqlite_err )&&( SQLITE_DONE != sqlite_err ))
            {
                TSLOG_ERROR_INT( "sqlite3_step failed:", sqlite_err );
                result |= DATA_ERROR_AT_DB;
            }
            if (( SQLITE_ROW == sqlite_err )&&(row_index < max_out_array_size))
            {
                *out_diagramelement_count = row_index+1;
                data_diagramelement_t *current_diagele = &((*out_diagramelement)[row_index]);

                data_diagramelement_init( current_diagele,
                                          sqlite3_column_int64( prepared_statement, RESULT_DIAGRAMELEMENT_ID_COLUMN ),
                                          sqlite3_column_int64( prepared_statement, RESULT_DIAGRAMELEMENT_DIAGRAM_ID_COLUMN ),
                                          sqlite3_column_int64( prepared_statement, RESULT_DIAGRAMELEMENT_CLASSIFIER_ID_COLUMN ),
                                          sqlite3_column_int64( prepared_statement, RESULT_DIAGRAMELEMENT_DISPLAY_FLAGS_COLUMN ),
                                          sqlite3_column_int64( prepared_statement, RESULT_DIAGRAMELEMENT_FOCUSED_FEATURE_ID_COLUMN )
                );
                if ( SQLITE_NULL == sqlite3_column_type( prepared_statement, RESULT_DIAGRAMELEMENT_FOCUSED_FEATURE_ID_COLUMN ) )
                {
                    data_diagramelement_set_focused_feature_row_id ( current_diagele, DATA_ROW_ID_VOID );
                }

                data_diagramelement_trace( current_diagele );
            }
            if (( SQLITE_ROW == sqlite_err )&&(row_index >= max_out_array_size))
            {
                TSLOG_ANOMALY_INT( "out_diagramelement[] full:", (row_index+1) );
                result |= DATA_ERROR_ARRAY_BUFFER_EXCEEDED;
            }
            if ( SQLITE_DONE == sqlite_err )
            {
                TRACE_INFO( "sqlite3_step finished: SQLITE_DONE" );
            }
        }
    }
    else
    {
        result |= DATA_ERROR_NO_DB;
        TRACE_INFO( "Database not open, cannot request data." );
    }

    TRACE_END_ERR( result );
    return result;
}

data_error_t data_database_reader_get_diagramelements_by_classifier_id ( data_database_reader_t *this_,
                                                                         data_row_id_t classifier_id,
                                                                         uint32_t max_out_array_size,
                                                                         data_diagramelement_t (*out_diagramelement)[],
                                                                         uint32_t *out_diagramelement_count )
{
    TRACE_BEGIN();
    assert( NULL != out_diagramelement_count );
    assert( NULL != out_diagramelement );
    data_error_t result = DATA_ERROR_NONE;
    int sqlite_err;
    sqlite3_stmt *prepared_statement;

    if ( (*this_).is_open )
    {
        prepared_statement = (*this_).private_prepared_query_diagramelements_by_classifier_id;

        result |= data_database_reader_private_bind_id_to_statement( this_, prepared_statement, classifier_id );

        *out_diagramelement_count = 0;
        sqlite_err = SQLITE_ROW;
        for ( uint32_t row_index = 0; (SQLITE_ROW == sqlite_err) && (row_index <= max_out_array_size); row_index ++ )
        {
            TRACE_INFO( "sqlite3_step()" );
            sqlite_err = sqlite3_step( prepared_statement );
            if (( SQLITE_ROW != sqlite_err )&&( SQLITE_DONE != sqlite_err ))
            {
                TSLOG_ERROR_INT( "sqlite3_step failed:", sqlite_err );
                result |= DATA_ERROR_AT_DB;
            }
            if (( SQLITE_ROW == sqlite_err )&&(row_index < max_out_array_size))
            {
                *out_diagramelement_count = row_index+1;
                data_diagramelement_t *current_diagele = &((*out_diagramelement)[row_index]);

                data_diagramelement_init( current_diagele,
                                          sqlite3_column_int64( prepared_statement, RESULT_DIAGRAMELEMENT_ID_COLUMN ),
                                          sqlite3_column_int64( prepared_statement, RESULT_DIAGRAMELEMENT_DIAGRAM_ID_COLUMN ),
                                          sqlite3_column_int64( prepared_statement, RESULT_DIAGRAMELEMENT_CLASSIFIER_ID_COLUMN ),
                                          sqlite3_column_int64( prepared_statement, RESULT_DIAGRAMELEMENT_DISPLAY_FLAGS_COLUMN ),
                                          sqlite3_column_int64( prepared_statement, RESULT_DIAGRAMELEMENT_FOCUSED_FEATURE_ID_COLUMN )
                );
                if ( SQLITE_NULL == sqlite3_column_type( prepared_statement, RESULT_DIAGRAMELEMENT_FOCUSED_FEATURE_ID_COLUMN ) )
                {
                    data_diagramelement_set_focused_feature_row_id ( current_diagele, DATA_ROW_ID_VOID );
                }

                data_diagramelement_trace( current_diagele );
            }
            if (( SQLITE_ROW == sqlite_err )&&(row_index >= max_out_array_size))
            {
                TSLOG_ANOMALY_INT( "out_diagramelement[] full:", (row_index+1) );
                result |= DATA_ERROR_ARRAY_BUFFER_EXCEEDED;
            }
            if ( SQLITE_DONE == sqlite_err )
            {
                TRACE_INFO( "sqlite3_step finished: SQLITE_DONE" );
            }
        }
    }
    else
    {
        result |= DATA_ERROR_NO_DB;
        TRACE_INFO( "Database not open, cannot request data." );
    }

    TRACE_END_ERR( result );
    return result;
}

/* ================================ FEATURE ================================ */

/*!
 *  \brief predefined search statement to find a feature by id
 */
static const char DATA_DATABASE_READER_SELECT_FEATURE_BY_ID[] =
    "SELECT id,main_type,classifier_id,key,value,description,list_order "
    "FROM features WHERE id=?;";

/*!
 *  \brief predefined search statement to find features by diagram-id
 */
static const char DATA_DATABASE_READER_SELECT_FEATURES_BY_DIAGRAM_ID[] =
    "SELECT features.id,features.main_type,features.classifier_id,"
    "features.key,features.value,features.description,features.list_order,"
    "diagramelements.id " /* diagramelements.id needed only for debugging */
    "FROM features INNER JOIN diagramelements ON diagramelements.classifier_id=features.classifier_id "
    "WHERE diagramelements.diagram_id=? GROUP BY features.id ORDER BY features.list_order ASC;";

/*!
 *  \brief predefined search statement to find features by classifier-id
 */
static const char DATA_DATABASE_READER_SELECT_FEATURES_BY_CLASSIFIER_ID[] =
    "SELECT id,main_type,classifier_id,key,value,description,list_order "
    "FROM features "
    "WHERE classifier_id=? ORDER BY list_order ASC;";

/*!
 *  \brief the column id of the result where this parameter is stored: id
 */
static const int RESULT_FEATURE_ID_COLUMN = 0;

/*!
 *  \brief the column id of the result where this parameter is stored: main_type
 */
static const int RESULT_FEATURE_MAIN_TYPE_COLUMN = 1;

/*!
 *  \brief the column id of the result where this parameter is stored: classifier_id
 */
static const int RESULT_FEATURE_CLASSIFIER_ID_COLUMN = 2;

/*!
 *  \brief the column id of the result where this parameter is stored: key
 */
static const int RESULT_FEATURE_KEY_COLUMN = 3;

/*!
 *  \brief the column id of the result where this parameter is stored: value
 */
static const int RESULT_FEATURE_VALUE_COLUMN = 4;

/*!
 *  \brief the column id of the result where this parameter is stored: description
 */
static const int RESULT_FEATURE_DESCRIPTION_COLUMN = 5;

/*!
 *  \brief the column id of the result where this parameter is stored: list_order
 */
static const int RESULT_FEATURE_LIST_ORDER_COLUMN = 6;

/*!
 *  \brief the column id of the result where this parameter is stored: diagramelements.id
 */
static const int RESULT_FEATURE_DIAGRAMELEMENTS_ID_COLUMN = 7;

data_error_t data_database_reader_get_feature_by_id ( data_database_reader_t *this_, data_row_id_t id, data_feature_t *out_feature )
{
    TRACE_BEGIN();
    assert( NULL != out_feature );
    data_error_t result = DATA_ERROR_NONE;
    int sqlite_err;
    sqlite3_stmt *prepared_statement;

    if ( (*this_).is_open )
    {
        prepared_statement = (*this_).private_prepared_query_feature_by_id;

        result |= data_database_reader_private_bind_id_to_statement( this_, prepared_statement, id );

        TRACE_INFO( "sqlite3_step()" );
        sqlite_err = sqlite3_step( prepared_statement );
        if ( SQLITE_ROW != sqlite_err )
        {
            TSLOG_ANOMALY( "sqlite3_step did not find a row." );
            result |= DATA_ERROR_DB_STRUCTURE;
        }

        if ( SQLITE_ROW == sqlite_err )
        {
            result |= data_feature_init( out_feature,
                                         sqlite3_column_int64( prepared_statement, RESULT_FEATURE_ID_COLUMN ),
                                         sqlite3_column_int( prepared_statement, RESULT_FEATURE_MAIN_TYPE_COLUMN ),
                                         sqlite3_column_int64( prepared_statement, RESULT_FEATURE_CLASSIFIER_ID_COLUMN ),
                                         (const char*) sqlite3_column_text( prepared_statement, RESULT_FEATURE_KEY_COLUMN ),
                                         (const char*) sqlite3_column_text( prepared_statement, RESULT_FEATURE_VALUE_COLUMN ),
                                         (const char*) sqlite3_column_text( prepared_statement, RESULT_FEATURE_DESCRIPTION_COLUMN ),
                                         sqlite3_column_int( prepared_statement, RESULT_FEATURE_LIST_ORDER_COLUMN )
            );

            data_feature_trace( out_feature );
        }

        sqlite_err = sqlite3_step( prepared_statement );
        if ( SQLITE_DONE != sqlite_err )
        {
            TSLOG_ERROR_INT( "sqlite3_step failed:", sqlite_err );
            result |= DATA_ERROR_DB_STRUCTURE;
        }
    }
    else
    {
        result |= DATA_ERROR_NO_DB;
        TRACE_INFO( "Database not open, cannot request data." );
    }

    TRACE_END_ERR( result );
    return result;
}

data_error_t data_database_reader_get_features_by_classifier_id ( data_database_reader_t *this_,
                                                                  data_row_id_t classifier_id,
                                                                  uint32_t max_out_array_size,
                                                                  data_feature_t (*out_feature)[],
                                                                  uint32_t *out_feature_count )
{
    TRACE_BEGIN();
    assert( NULL != out_feature_count );
    assert( NULL != out_feature );
    data_error_t result = DATA_ERROR_NONE;
    int sqlite_err;
    sqlite3_stmt *prepared_statement;

    if ( (*this_).is_open )
    {
        prepared_statement = (*this_).private_prepared_query_features_by_classifier_id;

        result |= data_database_reader_private_bind_id_to_statement( this_, prepared_statement, classifier_id );

        *out_feature_count = 0;
        sqlite_err = SQLITE_ROW;
        for ( uint32_t row_index = 0; (SQLITE_ROW == sqlite_err) && (row_index <= max_out_array_size); row_index ++ )
        {
            TRACE_INFO( "sqlite3_step()" );
            sqlite_err = sqlite3_step( prepared_statement );
            if (( SQLITE_ROW != sqlite_err )&&( SQLITE_DONE != sqlite_err ))
            {
                TSLOG_ERROR_INT( "sqlite3_step failed:", sqlite_err );
                result |= DATA_ERROR_AT_DB;
            }
            if (( SQLITE_ROW == sqlite_err )&&(row_index < max_out_array_size))
            {
                *out_feature_count = row_index+1;
                data_feature_t *current_feature;
                current_feature = &((*out_feature)[row_index]);

                result |= data_feature_init( current_feature,
                                             sqlite3_column_int64( prepared_statement, RESULT_FEATURE_ID_COLUMN ),
                                             sqlite3_column_int( prepared_statement, RESULT_FEATURE_MAIN_TYPE_COLUMN ),
                                             sqlite3_column_int64( prepared_statement, RESULT_FEATURE_CLASSIFIER_ID_COLUMN ),
                                             (const char*) sqlite3_column_text( prepared_statement, RESULT_FEATURE_KEY_COLUMN ),
                                             (const char*) sqlite3_column_text( prepared_statement, RESULT_FEATURE_VALUE_COLUMN ),
                                             (const char*) sqlite3_column_text( prepared_statement, RESULT_FEATURE_DESCRIPTION_COLUMN ),
                                             sqlite3_column_int( prepared_statement, RESULT_FEATURE_LIST_ORDER_COLUMN )
                );

                data_feature_trace( current_feature );
            }
            if (( SQLITE_ROW == sqlite_err )&&(row_index >= max_out_array_size))
            {
                TSLOG_ANOMALY_INT( "out_feature[] full:", (row_index+1) );
                result |= DATA_ERROR_ARRAY_BUFFER_EXCEEDED;
            }
            if ( SQLITE_DONE == sqlite_err )
            {
                TRACE_INFO( "sqlite3_step finished: SQLITE_DONE" );
            }
        }
    }
    else
    {
        result |= DATA_ERROR_NO_DB;
        TRACE_INFO( "Database not open, cannot request data." );
    }

    TRACE_END_ERR( result );
    return result;
}

data_error_t data_database_reader_get_features_by_diagram_id ( data_database_reader_t *this_,
                                                               data_row_id_t diagram_id,
                                                               uint32_t max_out_array_size,
                                                               data_feature_t (*out_feature)[],
                                                               uint32_t *out_feature_count )
{
    TRACE_BEGIN();
    assert( NULL != out_feature_count );
    assert( NULL != out_feature );
    data_error_t result = DATA_ERROR_NONE;
    int sqlite_err;
    sqlite3_stmt *prepared_statement;

    if ( (*this_).is_open )
    {
        prepared_statement = (*this_).private_prepared_query_features_by_diagram_id;

        result |= data_database_reader_private_bind_id_to_statement( this_, prepared_statement, diagram_id );

        *out_feature_count = 0;
        sqlite_err = SQLITE_ROW;
        for ( uint32_t row_index = 0; (SQLITE_ROW == sqlite_err) && (row_index <= max_out_array_size); row_index ++ )
        {
            TRACE_INFO( "sqlite3_step()" );
            sqlite_err = sqlite3_step( prepared_statement );
            if (( SQLITE_ROW != sqlite_err )&&( SQLITE_DONE != sqlite_err ))
            {
                TSLOG_ERROR_INT( "sqlite3_step failed:", sqlite_err );
                result |= DATA_ERROR_AT_DB;
            }
            if (( SQLITE_ROW == sqlite_err )&&(row_index < max_out_array_size))
            {
                *out_feature_count = row_index+1;
                data_feature_t *current_feature;
                current_feature = &((*out_feature)[row_index]);

                result |= data_feature_init( current_feature,
                                             sqlite3_column_int64( prepared_statement, RESULT_FEATURE_ID_COLUMN ),
                                             sqlite3_column_int( prepared_statement, RESULT_FEATURE_MAIN_TYPE_COLUMN ),
                                             sqlite3_column_int64( prepared_statement, RESULT_FEATURE_CLASSIFIER_ID_COLUMN ),
                                             (const char*) sqlite3_column_text( prepared_statement, RESULT_FEATURE_KEY_COLUMN ),
                                             (const char*) sqlite3_column_text( prepared_statement, RESULT_FEATURE_VALUE_COLUMN ),
                                             (const char*) sqlite3_column_text( prepared_statement, RESULT_FEATURE_DESCRIPTION_COLUMN ),
                                             sqlite3_column_int( prepared_statement, RESULT_FEATURE_LIST_ORDER_COLUMN )
                );

                TRACE_INFO_INT( "diagramelements.id:", sqlite3_column_int64( prepared_statement, RESULT_FEATURE_DIAGRAMELEMENTS_ID_COLUMN ) );
                data_feature_trace( current_feature );
            }
            if (( SQLITE_ROW == sqlite_err )&&(row_index >= max_out_array_size))
            {
                TSLOG_ANOMALY_INT( "out_feature[] full:", (row_index+1) );
                result |= DATA_ERROR_ARRAY_BUFFER_EXCEEDED;
            }
            if ( SQLITE_DONE == sqlite_err )
            {
                TRACE_INFO( "sqlite3_step finished: SQLITE_DONE" );
            }
        }
    }
    else
    {
        result |= DATA_ERROR_NO_DB;
        TRACE_INFO( "Database not open, cannot request data." );
    }

    TRACE_END_ERR( result );
    return result;
}

/* ================================ RELATIONSHIP ================================ */

/*!
 *  \brief predefined search statement to find a relationship by id
 */
static const char DATA_DATABASE_READER_SELECT_RELATIONSHIP_BY_ID[] =
    "SELECT id,main_type,from_classifier_id,to_classifier_id,name,description,list_order,"
    "from_feature_id,to_feature_id "
    "FROM relationships WHERE id=?;";

/*!
 *  \brief predefined search statement to find relationships by diagram-id
 */
static const char DATA_DATABASE_READER_SELECT_RELATIONSHIPS_BY_DIAGRAM_ID[] =
    "SELECT relationships.id,relationships.main_type,relationships.from_classifier_id,relationships.to_classifier_id,"
    "relationships.name,relationships.description,relationships.list_order,"
    "relationships.from_feature_id,relationships.to_feature_id,"
    "source.id, dest.id " /* source.id, dest.id needed only for debugging */
    "FROM relationships "
    "INNER JOIN diagramelements AS source "
    "ON source.classifier_id=relationships.from_classifier_id "
    "INNER JOIN diagramelements AS dest "
    "ON (dest.classifier_id=relationships.to_classifier_id)AND(dest.diagram_id=source.diagram_id) "
    "WHERE source.diagram_id=? "
    "GROUP BY relationships.id "
    "ORDER BY relationships.list_order ASC;";

/*!
 *  \brief predefined search statement to find relationships by classifier-id
 */
static const char DATA_DATABASE_READER_SELECT_RELATIONSHIPS_BY_CLASSIFIER_ID[] =
    "SELECT id,main_type,from_classifier_id,to_classifier_id,name,description,list_order,"
    "from_feature_id,to_feature_id "
    "FROM relationships "
    "WHERE from_classifier_id=? OR to_classifier_id=?;";

/*!
 *  \brief predefined search statement to find relationships by feature-id
 */
static const char DATA_DATABASE_READER_SELECT_RELATIONSHIPS_BY_FEATURE_ID[] =
    "SELECT id,main_type,from_classifier_id,to_classifier_id,name,description,list_order,"
    "from_feature_id,to_feature_id "
    "FROM relationships "
    "WHERE from_feature_id=? OR to_feature_id=?;";

/*!
 *  \brief the column id of the result where this parameter is stored: id
 */
static const int RESULT_RELATIONSHIP_ID_COLUMN = 0;

/*!
 *  \brief the column id of the result where this parameter is stored: main_type
 */
static const int RESULT_RELATIONSHIP_MAIN_TYPE_COLUMN = 1;

/*!
 *  \brief the column id of the result where this parameter is stored: from_classifier_id
 */
static const int RESULT_RELATIONSHIP_FROM_CLASSIFIER_ID_COLUMN = 2;

/*!
 *  \brief the column id of the result where this parameter is stored: to_classifier_id
 */
static const int RESULT_RELATIONSHIP_TO_CLASSIFIER_ID_COLUMN = 3;

/*!
 *  \brief the column id of the result where this parameter is stored: name
 */
static const int RESULT_RELATIONSHIP_NAME_COLUMN = 4;

/*!
 *  \brief the column id of the result where this parameter is stored: description
 */
static const int RESULT_RELATIONSHIP_DESCRIPTION_COLUMN = 5;

/*!
 *  \brief the column id of the result where this parameter is stored: list_order
 */
static const int RESULT_RELATIONSHIP_LIST_ORDER_COLUMN = 6;

/*!
 *  \brief the column id of the result where this parameter is stored: from_feature_id
 */
static const int RESULT_RELATIONSHIP_FROM_FEATURE_ID_COLUMN = 7;

/*!
 *  \brief the column id of the result where this parameter is stored: to_feature_id
 */
static const int RESULT_RELATIONSHIP_TO_FEATURE_ID_COLUMN = 8;

/*!
 *  \brief the column id of the result where this parameter is stored: source diagramelements.id
 */
static const int RESULT_RELATIONSHIP_SOURCE_DIAGRAMELEMENTS_ID_COLUMN = 9;

/*!
 *  \brief the column id of the result where this parameter is stored: dest diagramelements.id
 */
static const int RESULT_RELATIONSHIP_DEST_DIAGRAMELEMENTS_ID_COLUMN = 10;

data_error_t data_database_reader_get_relationship_by_id ( data_database_reader_t *this_, data_row_id_t id, data_relationship_t *out_relationship )
{
    TRACE_BEGIN();
    assert( NULL != out_relationship );
    data_error_t result = DATA_ERROR_NONE;
    int sqlite_err;
    sqlite3_stmt *prepared_statement;

    if ( (*this_).is_open )
    {
        prepared_statement = (*this_).private_prepared_query_relationship_by_id;

        result |= data_database_reader_private_bind_id_to_statement( this_, prepared_statement, id );

        TRACE_INFO( "sqlite3_step()" );
        sqlite_err = sqlite3_step( prepared_statement );
        if ( SQLITE_ROW != sqlite_err )
        {
            TSLOG_ANOMALY( "sqlite3_step did not find a row." );
            result |= DATA_ERROR_DB_STRUCTURE;
        }

        if ( SQLITE_ROW == sqlite_err )
        {
            result |= data_relationship_init( out_relationship,
                                              sqlite3_column_int64( prepared_statement, RESULT_RELATIONSHIP_ID_COLUMN ),
                                              sqlite3_column_int( prepared_statement, RESULT_RELATIONSHIP_MAIN_TYPE_COLUMN ),
                                              sqlite3_column_int64( prepared_statement, RESULT_RELATIONSHIP_FROM_CLASSIFIER_ID_COLUMN ),
                                              sqlite3_column_int64( prepared_statement, RESULT_RELATIONSHIP_TO_CLASSIFIER_ID_COLUMN ),
                                              (const char*) sqlite3_column_text( prepared_statement, RESULT_RELATIONSHIP_NAME_COLUMN ),
                                              (const char*) sqlite3_column_text( prepared_statement, RESULT_RELATIONSHIP_DESCRIPTION_COLUMN ),
                                              sqlite3_column_int( prepared_statement, RESULT_RELATIONSHIP_LIST_ORDER_COLUMN ),
                                              sqlite3_column_int64( prepared_statement, RESULT_RELATIONSHIP_FROM_FEATURE_ID_COLUMN ),
                                              sqlite3_column_int64( prepared_statement, RESULT_RELATIONSHIP_TO_FEATURE_ID_COLUMN )
            );
            if ( SQLITE_NULL == sqlite3_column_type( prepared_statement, RESULT_RELATIONSHIP_FROM_FEATURE_ID_COLUMN ) )
            {
                data_relationship_set_from_feature_row_id ( out_relationship, DATA_ROW_ID_VOID );
            }
            if ( SQLITE_NULL == sqlite3_column_type( prepared_statement, RESULT_RELATIONSHIP_TO_FEATURE_ID_COLUMN ) )
            {
                data_relationship_set_to_feature_row_id ( out_relationship, DATA_ROW_ID_VOID );
            }

            data_relationship_trace( out_relationship );
        }

        sqlite_err = sqlite3_step( prepared_statement );
        if ( SQLITE_DONE != sqlite_err )
        {
            TSLOG_ERROR_INT( "sqlite3_step failed:", sqlite_err );
            result |= DATA_ERROR_DB_STRUCTURE;
        }
    }
    else
    {
        result |= DATA_ERROR_NO_DB;
        TRACE_INFO( "Database not open, cannot request data." );
    }

    TRACE_END_ERR( result );
    return result;
}

data_error_t data_database_reader_get_relationships_by_classifier_id ( data_database_reader_t *this_,
                                                                       data_row_id_t classifier_id,
                                                                       uint32_t max_out_array_size,
                                                                       data_relationship_t (*out_relationship)[],
                                                                       uint32_t *out_relationship_count )
{
    TRACE_BEGIN();
    assert( NULL != out_relationship_count );
    assert( NULL != out_relationship );
    data_error_t result = DATA_ERROR_NONE;
    int sqlite_err;
    sqlite3_stmt *prepared_statement;

    if ( (*this_).is_open )
    {
        prepared_statement = (*this_).private_prepared_query_relationships_by_classifier_id;

        result |= data_database_reader_private_bind_two_ids_to_statement( this_, prepared_statement, classifier_id, classifier_id );

        *out_relationship_count = 0;
        sqlite_err = SQLITE_ROW;
        for ( uint32_t row_index = 0; (SQLITE_ROW == sqlite_err) && (row_index <= max_out_array_size); row_index ++ )
        {
            TRACE_INFO( "sqlite3_step()" );
            sqlite_err = sqlite3_step( prepared_statement );
            if (( SQLITE_ROW != sqlite_err )&&( SQLITE_DONE != sqlite_err ))
            {
                TSLOG_ERROR_INT( "sqlite3_step failed:", sqlite_err );
                result |= DATA_ERROR_AT_DB;
            }
            if (( SQLITE_ROW == sqlite_err )&&(row_index < max_out_array_size))
            {
                *out_relationship_count = row_index+1;
                data_relationship_t *current_relation;
                current_relation = &((*out_relationship)[row_index]);

                result |= data_relationship_init( current_relation,
                                                  sqlite3_column_int64( prepared_statement, RESULT_RELATIONSHIP_ID_COLUMN ),
                                                  sqlite3_column_int( prepared_statement, RESULT_RELATIONSHIP_MAIN_TYPE_COLUMN ),
                                                  sqlite3_column_int64( prepared_statement, RESULT_RELATIONSHIP_FROM_CLASSIFIER_ID_COLUMN ),
                                                  sqlite3_column_int64( prepared_statement, RESULT_RELATIONSHIP_TO_CLASSIFIER_ID_COLUMN ),
                                                  (const char*) sqlite3_column_text( prepared_statement, RESULT_RELATIONSHIP_NAME_COLUMN ),
                                                  (const char*) sqlite3_column_text( prepared_statement, RESULT_RELATIONSHIP_DESCRIPTION_COLUMN ),
                                                  sqlite3_column_int( prepared_statement, RESULT_RELATIONSHIP_LIST_ORDER_COLUMN ),
                                                  sqlite3_column_int64( prepared_statement, RESULT_RELATIONSHIP_FROM_FEATURE_ID_COLUMN ),
                                                  sqlite3_column_int64( prepared_statement, RESULT_RELATIONSHIP_TO_FEATURE_ID_COLUMN )
                                                );
                if ( SQLITE_NULL == sqlite3_column_type( prepared_statement, RESULT_RELATIONSHIP_FROM_FEATURE_ID_COLUMN ) )
                {
                    data_relationship_set_from_feature_row_id ( current_relation, DATA_ROW_ID_VOID );
                }
                if ( SQLITE_NULL == sqlite3_column_type( prepared_statement, RESULT_RELATIONSHIP_TO_FEATURE_ID_COLUMN ) )
                {
                    data_relationship_set_to_feature_row_id ( current_relation, DATA_ROW_ID_VOID );
                }

                data_relationship_trace( current_relation );
            }
            if (( SQLITE_ROW == sqlite_err )&&(row_index >= max_out_array_size))
            {
                TSLOG_ANOMALY_INT( "out_relationship[] full:", (row_index+1) );
                result |= DATA_ERROR_ARRAY_BUFFER_EXCEEDED;
            }
            if ( SQLITE_DONE == sqlite_err )
            {
                TRACE_INFO( "sqlite3_step finished: SQLITE_DONE" );
            }
        }
    }
    else
    {
        result |= DATA_ERROR_NO_DB;
        TRACE_INFO( "Database not open, cannot request data." );
    }

    TRACE_END_ERR( result );
    return result;
}

data_error_t data_database_reader_get_relationships_by_feature_id ( data_database_reader_t *this_,
                                                                    data_row_id_t feature_id,
                                                                    uint32_t max_out_array_size,
                                                                    data_relationship_t (*out_relationship)[],
                                                                    uint32_t *out_relationship_count )
{
    TRACE_BEGIN();
    assert( NULL != out_relationship_count );
    assert( NULL != out_relationship );
    data_error_t result = DATA_ERROR_NONE;
    int sqlite_err;
    sqlite3_stmt *prepared_statement;

    if ( (*this_).is_open )
    {
        prepared_statement = (*this_).private_prepared_query_relationships_by_feature_id;

        result |= data_database_reader_private_bind_two_ids_to_statement( this_, prepared_statement, feature_id, feature_id );

        *out_relationship_count = 0;
        sqlite_err = SQLITE_ROW;
        for ( uint32_t row_index = 0; (SQLITE_ROW == sqlite_err) && (row_index <= max_out_array_size); row_index ++ )
        {
            TRACE_INFO( "sqlite3_step()" );
            sqlite_err = sqlite3_step( prepared_statement );
            if (( SQLITE_ROW != sqlite_err )&&( SQLITE_DONE != sqlite_err ))
            {
                TSLOG_ERROR_INT( "sqlite3_step failed:", sqlite_err );
                result |= DATA_ERROR_AT_DB;
            }
            if (( SQLITE_ROW == sqlite_err )&&(row_index < max_out_array_size))
            {
                *out_relationship_count = row_index+1;
                data_relationship_t *current_relation;
                current_relation = &((*out_relationship)[row_index]);

                result |= data_relationship_init( current_relation,
                                                  sqlite3_column_int64( prepared_statement, RESULT_RELATIONSHIP_ID_COLUMN ),
                                                  sqlite3_column_int( prepared_statement, RESULT_RELATIONSHIP_MAIN_TYPE_COLUMN ),
                                                  sqlite3_column_int64( prepared_statement, RESULT_RELATIONSHIP_FROM_CLASSIFIER_ID_COLUMN ),
                                                  sqlite3_column_int64( prepared_statement, RESULT_RELATIONSHIP_TO_CLASSIFIER_ID_COLUMN ),
                                                  (const char*) sqlite3_column_text( prepared_statement, RESULT_RELATIONSHIP_NAME_COLUMN ),
                                                  (const char*) sqlite3_column_text( prepared_statement, RESULT_RELATIONSHIP_DESCRIPTION_COLUMN ),
                                                  sqlite3_column_int( prepared_statement, RESULT_RELATIONSHIP_LIST_ORDER_COLUMN ),
                                                  sqlite3_column_int64( prepared_statement, RESULT_RELATIONSHIP_FROM_FEATURE_ID_COLUMN ),
                                                  sqlite3_column_int64( prepared_statement, RESULT_RELATIONSHIP_TO_FEATURE_ID_COLUMN )
                );
                if ( SQLITE_NULL == sqlite3_column_type( prepared_statement, RESULT_RELATIONSHIP_FROM_FEATURE_ID_COLUMN ) )
                {
                    data_relationship_set_from_feature_row_id ( current_relation, DATA_ROW_ID_VOID );
                }
                if ( SQLITE_NULL == sqlite3_column_type( prepared_statement, RESULT_RELATIONSHIP_TO_FEATURE_ID_COLUMN ) )
                {
                    data_relationship_set_to_feature_row_id ( current_relation, DATA_ROW_ID_VOID );
                }

                data_relationship_trace( current_relation );
            }
            if (( SQLITE_ROW == sqlite_err )&&(row_index >= max_out_array_size))
            {
                TSLOG_ANOMALY_INT( "out_relationship[] full:", (row_index+1) );
                result |= DATA_ERROR_ARRAY_BUFFER_EXCEEDED;
            }
            if ( SQLITE_DONE == sqlite_err )
            {
                TRACE_INFO( "sqlite3_step finished: SQLITE_DONE" );
            }
        }
    }
    else
    {
        result |= DATA_ERROR_NO_DB;
        TRACE_INFO( "Database not open, cannot request data." );
    }

    TRACE_END_ERR( result );
    return result;
}

data_error_t data_database_reader_get_relationships_by_diagram_id ( data_database_reader_t *this_,
                                                                    data_row_id_t diagram_id,
                                                                    uint32_t max_out_array_size,
                                                                    data_relationship_t (*out_relationship)[],
                                                                    uint32_t *out_relationship_count )
{
    TRACE_BEGIN();
    assert( NULL != out_relationship_count );
    assert( NULL != out_relationship );
    data_error_t result = DATA_ERROR_NONE;
    int sqlite_err;
    sqlite3_stmt *prepared_statement;

    if ( (*this_).is_open )
    {
        prepared_statement = (*this_).private_prepared_query_relationships_by_diagram_id;

        result |= data_database_reader_private_bind_id_to_statement( this_, prepared_statement, diagram_id );

        *out_relationship_count = 0;
        sqlite_err = SQLITE_ROW;
        for ( uint32_t row_index = 0; (SQLITE_ROW == sqlite_err) && (row_index <= max_out_array_size); row_index ++ )
        {
            TRACE_INFO( "sqlite3_step()" );
            sqlite_err = sqlite3_step( prepared_statement );
            if (( SQLITE_ROW != sqlite_err )&&( SQLITE_DONE != sqlite_err ))
            {
                TSLOG_ERROR_INT( "sqlite3_step failed:", sqlite_err );
                result |= DATA_ERROR_AT_DB;
            }
            if (( SQLITE_ROW == sqlite_err )&&(row_index < max_out_array_size))
            {
                *out_relationship_count = row_index+1;
                data_relationship_t *current_relation;
                current_relation = &((*out_relationship)[row_index]);

                result |= data_relationship_init( current_relation,
                                                  sqlite3_column_int64( prepared_statement, RESULT_RELATIONSHIP_ID_COLUMN ),
                                                  sqlite3_column_int( prepared_statement, RESULT_RELATIONSHIP_MAIN_TYPE_COLUMN ),
                                                  sqlite3_column_int64( prepared_statement, RESULT_RELATIONSHIP_FROM_CLASSIFIER_ID_COLUMN ),
                                                  sqlite3_column_int64( prepared_statement, RESULT_RELATIONSHIP_TO_CLASSIFIER_ID_COLUMN ),
                                                  (const char*) sqlite3_column_text( prepared_statement, RESULT_RELATIONSHIP_NAME_COLUMN ),
                                                  (const char*) sqlite3_column_text( prepared_statement, RESULT_RELATIONSHIP_DESCRIPTION_COLUMN ),
                                                  sqlite3_column_int( prepared_statement, RESULT_RELATIONSHIP_LIST_ORDER_COLUMN ),
                                                  sqlite3_column_int64( prepared_statement, RESULT_RELATIONSHIP_FROM_FEATURE_ID_COLUMN ),
                                                  sqlite3_column_int64( prepared_statement, RESULT_RELATIONSHIP_TO_FEATURE_ID_COLUMN )
                                                );
                if ( SQLITE_NULL == sqlite3_column_type( prepared_statement, RESULT_RELATIONSHIP_FROM_FEATURE_ID_COLUMN ) )
                {
                    data_relationship_set_from_feature_row_id ( current_relation, DATA_ROW_ID_VOID );
                }
                if ( SQLITE_NULL == sqlite3_column_type( prepared_statement, RESULT_RELATIONSHIP_TO_FEATURE_ID_COLUMN ) )
                {
                    data_relationship_set_to_feature_row_id ( current_relation, DATA_ROW_ID_VOID );
                }

                TRACE_INFO_INT( "(source)diagramelements.id:", sqlite3_column_int64( prepared_statement, RESULT_RELATIONSHIP_SOURCE_DIAGRAMELEMENTS_ID_COLUMN ) );
                TRACE_INFO_INT( "(dest)diagramelements.id:", sqlite3_column_int64( prepared_statement, RESULT_RELATIONSHIP_DEST_DIAGRAMELEMENTS_ID_COLUMN ) );
                data_relationship_trace( current_relation );
            }
            if (( SQLITE_ROW == sqlite_err )&&(row_index >= max_out_array_size))
            {
                TSLOG_ANOMALY_INT( "out_relationship[] full:", (row_index+1) );
                result |= DATA_ERROR_ARRAY_BUFFER_EXCEEDED;
            }
            if ( SQLITE_DONE == sqlite_err )
            {
                TRACE_INFO( "sqlite3_step finished: SQLITE_DONE" );
            }
        }
    }
    else
    {
        result |= DATA_ERROR_NO_DB;
        TRACE_INFO( "Database not open, cannot request data." );
    }

    TRACE_END_ERR( result );
    return result;
}

/* ================================ private ================================ */

data_error_t data_database_reader_private_open ( data_database_reader_t *this_ )
{
    TRACE_BEGIN();
    data_error_t result = DATA_ERROR_NONE;

    if ( ! (*this_).is_open )
    {
        result |= data_database_reader_private_prepare_statement ( this_,
                                                                   DATA_DATABASE_READER_SELECT_DIAGRAM_BY_ID,
                                                                   sizeof( DATA_DATABASE_READER_SELECT_DIAGRAM_BY_ID ),
                                                                   &((*this_).private_prepared_query_diagram_by_id)
                                                                 );

        result |= data_database_reader_private_prepare_statement ( this_,
                                                                   DATA_DATABASE_READER_SELECT_DIAGRAMS_BY_PARENT_ID,
                                                                   sizeof( DATA_DATABASE_READER_SELECT_DIAGRAMS_BY_PARENT_ID ),
                                                                   &((*this_).private_prepared_query_diagrams_by_parent_id)
                                                                 );

        result |= data_database_reader_private_prepare_statement ( this_,
                                                                   DATA_DATABASE_READER_SELECT_DIAGRAMS_BY_PARENT_ID_NULL,
                                                                   sizeof( DATA_DATABASE_READER_SELECT_DIAGRAMS_BY_PARENT_ID_NULL ),
                                                                   &((*this_).private_prepared_query_diagrams_by_parent_id_null)
                                                                 );

#ifdef DATA_DATABASE_READER_PROVIDE_DEPRECATED_FKT
        result |= data_database_reader_private_prepare_statement ( this_,
                                                                   DATA_DATABASE_READER_SELECT_DIAGRAMS_BY_CLASSIFIER_ID,
                                                                   sizeof( DATA_DATABASE_READER_SELECT_DIAGRAMS_BY_CLASSIFIER_ID ),
                                                                   &((*this_).private_prepared_query_diagrams_by_classifier_id)
                                                                 );
#endif

        result |= data_database_reader_private_prepare_statement ( this_,
                                                                   DATA_DATABASE_READER_SELECT_DIAGRAM_IDS_BY_PARENT_ID,
                                                                   sizeof( DATA_DATABASE_READER_SELECT_DIAGRAM_IDS_BY_PARENT_ID ),
                                                                   &((*this_).private_prepared_query_diagram_ids_by_parent_id)
                                                                 );

        result |= data_database_reader_private_prepare_statement ( this_,
                                                                   DATA_DATABASE_READER_SELECT_DIAGRAM_IDS_BY_PARENT_ID_NULL,
                                                                   sizeof( DATA_DATABASE_READER_SELECT_DIAGRAM_IDS_BY_PARENT_ID_NULL ),
                                                                   &((*this_).private_prepared_query_diagram_ids_by_parent_id_null)
                                                                 );

        result |= data_database_reader_private_prepare_statement ( this_,
                                                                   DATA_DATABASE_READER_SELECT_DIAGRAM_IDS_BY_CLASSIFIER_ID,
                                                                   sizeof( DATA_DATABASE_READER_SELECT_DIAGRAM_IDS_BY_CLASSIFIER_ID ),
                                                                   &((*this_).private_prepared_query_diagram_ids_by_classifier_id)
                                                                 );

        result |= data_database_reader_private_prepare_statement ( this_,
                                                                   DATA_DATABASE_READER_SELECT_CLASSIFIER_BY_ID,
                                                                   sizeof( DATA_DATABASE_READER_SELECT_CLASSIFIER_BY_ID ),
                                                                   &((*this_).private_prepared_query_classifier_by_id)
                                                                 );

        result |= data_database_reader_private_prepare_statement ( this_,
                                                                   DATA_DATABASE_READER_SELECT_CLASSIFIER_BY_NAME,
                                                                   sizeof( DATA_DATABASE_READER_SELECT_CLASSIFIER_BY_NAME ),
                                                                   &((*this_).private_prepared_query_classifier_by_name)
                                                                 );

        result |= data_database_reader_private_prepare_statement ( this_,
                                                                   DATA_DATABASE_READER_SELECT_CLASSIFIERS_BY_DIAGRAM_ID,
                                                                   sizeof( DATA_DATABASE_READER_SELECT_CLASSIFIERS_BY_DIAGRAM_ID ),
                                                                   &((*this_).private_prepared_query_classifiers_by_diagram_id)
                                                                 );

        result |= data_database_reader_private_prepare_statement ( this_,
                                                                   DATA_DATABASE_READER_SELECT_DIAGRAMELEMENT_BY_ID,
                                                                   sizeof( DATA_DATABASE_READER_SELECT_DIAGRAMELEMENT_BY_ID ),
                                                                   &((*this_).private_prepared_query_diagramelement_by_id)
                                                                 );

        result |= data_database_reader_private_prepare_statement ( this_,
                                                                   DATA_DATABASE_READER_SELECT_DIAGRAMELEMENTS_BY_DIAGRAM_ID,
                                                                   sizeof( DATA_DATABASE_READER_SELECT_DIAGRAMELEMENTS_BY_DIAGRAM_ID ),
                                                                   &((*this_).private_prepared_query_diagramelements_by_diagram_id)
                                                                 );

        result |= data_database_reader_private_prepare_statement ( this_,
                                                                   DATA_DATABASE_READER_SELECT_DIAGRAMELEMENTS_BY_CLASSIFIER_ID,
                                                                   sizeof( DATA_DATABASE_READER_SELECT_DIAGRAMELEMENTS_BY_CLASSIFIER_ID ),
                                                                   &((*this_).private_prepared_query_diagramelements_by_classifier_id)
                                                                 );

        result |= data_database_reader_private_prepare_statement ( this_,
                                                                   DATA_DATABASE_READER_SELECT_FEATURE_BY_ID,
                                                                   sizeof( DATA_DATABASE_READER_SELECT_FEATURE_BY_ID ),
                                                                   &((*this_).private_prepared_query_feature_by_id)
                                                                 );

        result |= data_database_reader_private_prepare_statement ( this_,
                                                                   DATA_DATABASE_READER_SELECT_FEATURES_BY_CLASSIFIER_ID,
                                                                   sizeof( DATA_DATABASE_READER_SELECT_FEATURES_BY_CLASSIFIER_ID ),
                                                                   &((*this_).private_prepared_query_features_by_classifier_id)
                                                                 );

        result |= data_database_reader_private_prepare_statement ( this_,
                                                                   DATA_DATABASE_READER_SELECT_FEATURES_BY_DIAGRAM_ID,
                                                                   sizeof( DATA_DATABASE_READER_SELECT_FEATURES_BY_DIAGRAM_ID ),
                                                                   &((*this_).private_prepared_query_features_by_diagram_id)
                                                                 );

        result |= data_database_reader_private_prepare_statement ( this_,
                                                                   DATA_DATABASE_READER_SELECT_RELATIONSHIP_BY_ID,
                                                                   sizeof( DATA_DATABASE_READER_SELECT_RELATIONSHIP_BY_ID ),
                                                                   &((*this_).private_prepared_query_relationship_by_id)
                                                                 );

        result |= data_database_reader_private_prepare_statement ( this_,
                                                                   DATA_DATABASE_READER_SELECT_RELATIONSHIPS_BY_CLASSIFIER_ID,
                                                                   sizeof( DATA_DATABASE_READER_SELECT_RELATIONSHIPS_BY_CLASSIFIER_ID ),
                                                                   &((*this_).private_prepared_query_relationships_by_classifier_id)
                                                                 );

        result |= data_database_reader_private_prepare_statement ( this_,
                                                                   DATA_DATABASE_READER_SELECT_RELATIONSHIPS_BY_FEATURE_ID,
                                                                   sizeof( DATA_DATABASE_READER_SELECT_RELATIONSHIPS_BY_FEATURE_ID ),
                                                                   &((*this_).private_prepared_query_relationships_by_feature_id)
                                                                 );

        result |= data_database_reader_private_prepare_statement ( this_,
                                                                   DATA_DATABASE_READER_SELECT_RELATIONSHIPS_BY_DIAGRAM_ID,
                                                                   sizeof( DATA_DATABASE_READER_SELECT_RELATIONSHIPS_BY_DIAGRAM_ID ),
                                                                   &((*this_).private_prepared_query_relationships_by_diagram_id)
                                                                 );

        (*this_).is_open = true;
    }
    else
    {
        result |= DATA_ERROR_INVALID_REQUEST;
        TSLOG_WARNING( "Database is already open." );
    }

    TRACE_END_ERR(result);
    return result;
}

data_error_t data_database_reader_private_close ( data_database_reader_t *this_ )
{
    TRACE_BEGIN();
    data_error_t result = DATA_ERROR_NONE;

    if ( (*this_).is_open )
    {
        result |= data_database_reader_private_finalize_statement( this_, (*this_).private_prepared_query_diagram_by_id );

        result |= data_database_reader_private_finalize_statement( this_, (*this_).private_prepared_query_diagrams_by_parent_id );

        result |= data_database_reader_private_finalize_statement( this_, (*this_).private_prepared_query_diagrams_by_parent_id_null );

#ifdef DATA_DATABASE_READER_PROVIDE_DEPRECATED_FKT
        result |= data_database_reader_private_finalize_statement( this_, (*this_).private_prepared_query_diagrams_by_classifier_id );
#endif

        result |= data_database_reader_private_finalize_statement( this_, (*this_).private_prepared_query_diagram_ids_by_parent_id );

        result |= data_database_reader_private_finalize_statement( this_, (*this_).private_prepared_query_diagram_ids_by_parent_id_null );

        result |= data_database_reader_private_finalize_statement( this_, (*this_).private_prepared_query_diagram_ids_by_classifier_id );

        result |= data_database_reader_private_finalize_statement( this_, (*this_).private_prepared_query_classifier_by_id );

        result |= data_database_reader_private_finalize_statement( this_, (*this_).private_prepared_query_classifier_by_name );

        result |= data_database_reader_private_finalize_statement( this_, (*this_).private_prepared_query_classifiers_by_diagram_id );

        result |= data_database_reader_private_finalize_statement( this_, (*this_).private_prepared_query_diagramelement_by_id );

        result |= data_database_reader_private_finalize_statement( this_, (*this_).private_prepared_query_diagramelements_by_diagram_id );

        result |= data_database_reader_private_finalize_statement( this_, (*this_).private_prepared_query_diagramelements_by_classifier_id );

        result |= data_database_reader_private_finalize_statement( this_, (*this_).private_prepared_query_feature_by_id );

        result |= data_database_reader_private_finalize_statement( this_, (*this_).private_prepared_query_features_by_classifier_id );

        result |= data_database_reader_private_finalize_statement( this_, (*this_).private_prepared_query_features_by_diagram_id );

        result |= data_database_reader_private_finalize_statement( this_, (*this_).private_prepared_query_relationship_by_id );

        result |= data_database_reader_private_finalize_statement( this_, (*this_).private_prepared_query_relationships_by_classifier_id );

        result |= data_database_reader_private_finalize_statement( this_, (*this_).private_prepared_query_relationships_by_feature_id );

        result |= data_database_reader_private_finalize_statement( this_, (*this_).private_prepared_query_relationships_by_diagram_id );

        (*this_).is_open = false;
    }
    else
    {
        result |= DATA_ERROR_INVALID_REQUEST;
        TSLOG_WARNING( "Database was not open." );
    }

    TRACE_END_ERR(result);
    return result;
}


/*
Copyright 2016-2021 Andreas Warnke

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
