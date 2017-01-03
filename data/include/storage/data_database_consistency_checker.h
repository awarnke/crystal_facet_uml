/* File: data_database_consistency_checker.h; Copyright and License: see below */

#ifndef DATA_DATABASE_CONSISTENCY_CHECKER_H
#define DATA_DATABASE_CONSISTENCY_CHECKER_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief checks and repairs a database
 */

#include "storage/data_database.h"
#include "data_diagram.h"
#include "data_error.h"
#include "data_classifier.h"
#include "data_visible_classifier.h"
#include "data_small_set.h"
#include <stdio.h>
#include <sqlite3.h>
#include <stdbool.h>
#include <stdint.h>

/*!
 *  \brief all data attributes needed for the database consistency checker functions
 */
struct data_database_consistency_checker_struct {
    data_database_t *database;  /*!< pointer to external database */
};

typedef struct data_database_consistency_checker_struct data_database_consistency_checker_t;

/*!
 *  \brief initializes the data_database_consistency_checker_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \param database database which this checker uses
 *  \return DATA_ERROR_NONE in case of success
 */
data_error_t data_database_consistency_checker_init ( data_database_consistency_checker_t *this_, data_database_t *database );

/*!
 *  \brief destroys the data_database_consistency_checker_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \return DATA_ERROR_NONE in case of success
 */
data_error_t data_database_consistency_checker_destroy ( data_database_consistency_checker_t *this_ );

/*!
 *  \brief reads the set of unreferenced diagrams from the database
 *
 *  \param this_ pointer to own object attributes
 *  \param out_total_count total number of diagrams in the database, must not be NULL
 *  \param io_set the set of unreferenced diagrams (invalid diagrams.parent_id).
 *                io_set must not be NULL. io_set shall be initialized already.
 *  \return DATA_ERROR_NONE in case of success, a negative value in case of error (e.g. DATA_ERROR_NO_DB if database not open).
 */
data_error_t data_database_consistency_checker_find_unreferenced_diagrams ( data_database_consistency_checker_t *this_,
                                                                            uint32_t *out_total_count,
                                                                            data_small_set_t *io_set
                                                                          );

/*!
 *  \brief reads the set of nonreferencing diagramelements from the database
 *
 *  \param this_ pointer to own object attributes
 *  \param io_set the set of unreferenced diagramelements (invalid diagramelements.diagram_id or diagramelements.classifier_id).
 *                io_set must not be NULL. io_set shall be initialized already.
 *  \return DATA_ERROR_NONE in case of success, a negative value in case of error (e.g. DATA_ERROR_NO_DB if database not open).
 */
data_error_t data_database_consistency_checker_find_nonreferencing_diagramelements ( data_database_consistency_checker_t *this_, data_small_set_t *io_set );

/*!
 *  \brief reads the set of unreferenced classifiers from the database
 *
 *  \param this_ pointer to own object attributes
 *  \param io_set the set of unreferenced classifiers (no references from diagramelements.classifier_id).
 *                io_set must not be NULL. io_set shall be initialized already.
 *  \return DATA_ERROR_NONE in case of success, a negative value in case of error (e.g. DATA_ERROR_NO_DB if database not open).
 */
data_error_t data_database_consistency_checker_find_unreferenced_classifiers ( data_database_consistency_checker_t *this_, data_small_set_t *io_set );

/*!
 *  \brief reads the set of unreferenced features from the database
 *
 *  \param this_ pointer to own object attributes
 *  \param io_set the set of unreferenced features (invalid features.classifier_id).
 *                io_set must not be NULL. io_set shall be initialized already.
 *  \return DATA_ERROR_NONE in case of success, a negative value in case of error (e.g. DATA_ERROR_NO_DB if database not open).
 */
data_error_t data_database_consistency_checker_find_unreferenced_features ( data_database_consistency_checker_t *this_, data_small_set_t *io_set );

/*!
 *  \brief reads the set of unreferenced relationships from the database
 *
 *  \param this_ pointer to own object attributes
 *  \param io_set the set of unreferenced relationships (invalid relationships.from_classifier_id or relationships.to_classifier_id).
 *                io_set must not be NULL. io_set shall be initialized already.
 *  \return DATA_ERROR_NONE in case of success, a negative value in case of error (e.g. DATA_ERROR_NO_DB if database not open).
 */
data_error_t data_database_consistency_checker_find_unreferenced_relationships ( data_database_consistency_checker_t *this_, data_small_set_t *io_set );

#endif  /* DATA_DATABASE_CONSISTENCY_CHECKER_H */


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
