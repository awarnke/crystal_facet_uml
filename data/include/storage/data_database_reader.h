/* File: data_database_reader.h; Copyright and License: see below */

#ifndef DATA_DATABASE_READER_H
#define DATA_DATABASE_READER_H

/* public file for the doxygen documentation: */
/*! \file */

#include "storage/data_database.h"
#include "data_diagram.h"
#include "data_error.h"
#include <stdio.h>
#include <sqlite3.h>
#include <stdbool.h>
#include <stdint.h>
#include <pthread.h>

/*!
 *  \brief all data attributes needed for the database functions
 */
struct data_database_reader_struct {
    data_database_t *database;  /*!< pointer to external database */
    pthread_mutex_t private_lock; /*!< lock to ensure that all private attributes are used by only one thread */
    sqlite3_stmt *private_prepared_query_diagram_by_id;
    sqlite3_stmt *private_prepared_query_diagrams_by_parent_id;
};

typedef struct data_database_reader_struct data_database_reader_t;

/*!
 *  \brief initializes the data_database_reader_t struct
 *
 *  \param this_ pointer to own object attributes
 */
void data_database_reader_init ( data_database_reader_t *this_, data_database_t *database );

/*!
 *  \brief destroys the data_database_reader_t struct
 *
 *  \param this_ pointer to own object attributes
 */
void data_database_reader_destroy ( data_database_reader_t *this_ );

/*!
 *  \brief reads a diagram from the database
 *
 *  \param this_ pointer to own object attributes
 *  \param id the diagram to be read from the database
 *  \param out_diagram the diagram read from the database (in case of success)
 *  \return DATA_ERROR_NONE in case of success, a negative value in case of error.
 */
data_error_t data_database_reader_get_diagram_by_id ( data_database_reader_t *this_, int64_t id, data_diagram_t *out_diagram );

/*!
 *  \brief reads all child-diagrams from the database
 *
 *  \param this_ pointer to own object attributes
 *  \param parent_id id of the parent diagram
 *  \param max_out_array_size size of the array where to store the results. If size is too small for the actual result set, this is an error.
 *  \param out_diagram_count number of diagram records stored in out_diagram
 *  \param out_diagram array of diagrams read from the database (in case of success)
 *  \return DATA_ERROR_NONE in case of success, a negative value in case of error.
 */
data_error_t data_database_reader_get_diagrams_by_parent_id ( data_database_reader_t *this_, int64_t parent_id, int32_t max_out_array_size, int32_t *out_diagram_count, data_diagram_t (*out_diagram)[] );

#endif  /* DATA_DATABASE_READER_H */


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