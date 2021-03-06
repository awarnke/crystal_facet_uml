/* File: data_database_reader.h; Copyright and License: see below */

#ifndef DATA_DATABASE_READER_H
#define DATA_DATABASE_READER_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Searches and reads records from the database
 */

#include "storage/data_database_listener.h"
#include "storage/data_database_listener_signal.h"
#include "storage/data_database_iterator_classifiers.h"
#include "storage/data_database.h"
#include "data_diagram.h"
#include "data_error.h"
#include "data_classifier.h"
#include "set/data_visible_classifier.h"
#include "set/data_small_set.h"
#include "data_feature.h"
#include "data_relationship.h"
#include "data_row_id.h"
#include <stdio.h>
#include <sqlite3.h>
#include <stdbool.h>
#include <stdint.h>

#define DATA_DATABASE_READER_PROVIDE_DEPRECATED_FKT 1

/*!
 *  \brief all data attributes needed for the database functions
 */
struct data_database_reader_struct {
    data_database_t *database;  /*!< pointer to external database */

    bool is_open;  /*!< the prepared statements are only initialized if the database is open */
    sqlite3_stmt *private_prepared_query_diagram_by_id;
    sqlite3_stmt *private_prepared_query_diagrams_by_parent_id;
    sqlite3_stmt *private_prepared_query_diagrams_by_parent_id_null;
#ifdef DATA_DATABASE_READER_PROVIDE_DEPRECATED_FKT
    sqlite3_stmt *private_prepared_query_diagrams_by_classifier_id;
#endif
    sqlite3_stmt *private_prepared_query_diagram_ids_by_parent_id;
    sqlite3_stmt *private_prepared_query_diagram_ids_by_parent_id_null;
    sqlite3_stmt *private_prepared_query_diagram_ids_by_classifier_id;
    sqlite3_stmt *private_prepared_query_classifier_by_id;
    sqlite3_stmt *private_prepared_query_classifier_by_name;
    sqlite3_stmt *private_prepared_query_classifiers_by_diagram_id;
    sqlite3_stmt *private_prepared_query_diagramelement_by_id;
    sqlite3_stmt *private_prepared_query_diagramelements_by_diagram_id;
    sqlite3_stmt *private_prepared_query_diagramelements_by_classifier_id;
    sqlite3_stmt *private_prepared_query_feature_by_id;
    sqlite3_stmt *private_prepared_query_features_by_classifier_id;
    sqlite3_stmt *private_prepared_query_features_by_diagram_id;
    sqlite3_stmt *private_prepared_query_relationship_by_id;
    sqlite3_stmt *private_prepared_query_relationships_by_classifier_id;
    sqlite3_stmt *private_prepared_query_relationships_by_feature_id;
    sqlite3_stmt *private_prepared_query_relationships_by_diagram_id;

    data_database_listener_t me_as_listener;  /*!< own instance of data_database_listener_t which wraps data_database_reader_db_change_callback */
};

typedef struct data_database_reader_struct data_database_reader_t;

/*!
 *  \brief initializes the data_database_reader_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \param database database which this reader uses
 *  \return DATA_ERROR_NONE in case of success
 */
data_error_t data_database_reader_init ( data_database_reader_t *this_, data_database_t *database );

/*!
 *  \brief destroys the data_database_reader_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \return DATA_ERROR_NONE in case of success
 */
data_error_t data_database_reader_destroy ( data_database_reader_t *this_ );

/*!
 *  \brief prepares a database change and re-initializes afterwards
 *
 *  \param this_ pointer to own object attributes
 *  \param signal_id state of database change
 */
void data_database_reader_db_change_callback ( data_database_reader_t *this_, data_database_listener_signal_t signal_id );

/*!
 *  \brief checks if the database reader is open
 *
 *  \param this_ pointer to own object attributes
 *  \return true if the database reader is open
 */
static inline bool data_database_reader_is_open( data_database_reader_t *this_ );

/* ================================ DIAGRAM ================================ */

/*!
 *  \brief reads a diagram from the database
 *
 *  \param this_ pointer to own object attributes
 *  \param id the diagram to be read from the database
 *  \param[out] out_diagram the diagram read from the database (in case of success)
 *  \return DATA_ERROR_NONE in case of success, an error code in case of error.
 *          E.g. DATA_ERROR_DB_STRUCTURE if id does not exist or DATA_ERROR_NO_DB if the database is not open.
 */
data_error_t data_database_reader_get_diagram_by_id ( data_database_reader_t *this_, data_row_id_t id, data_diagram_t *out_diagram );

/*!
 *  \brief reads all child-diagrams from the database
 *
 *  \param this_ pointer to own object attributes
 *  \param parent_id id of the parent diagram, DATA_ROW_ID_VOID to get all root diagrams
 *  \param max_out_array_size size of the array where to store the results. If size is too small for the actual result set, this is an error.
 *  \param[out] out_diagram array of diagrams read from the database (in case of success)
 *  \param[out] out_diagram_count number of diagram records stored in out_diagram
 *  \return DATA_ERROR_NONE in case of success, an error code in case of error.
 *          DATA_ERROR_NO_DB if the database is not open,
 *          DATA_ERROR_ARRAY_BUFFER_EXCEEDED if the provided out buffers are too small.
 */
data_error_t data_database_reader_get_diagrams_by_parent_id ( data_database_reader_t *this_,
                                                              data_row_id_t parent_id,
                                                              uint32_t max_out_array_size,
                                                              data_diagram_t (*out_diagram)[],
                                                              uint32_t *out_diagram_count
                                                            );

#ifdef DATA_DATABASE_READER_PROVIDE_DEPRECATED_FKT
/*!
 *  \brief reads all classifier-displaying diagrams from the database
 *
 *  If a diagram shows the classifier multiple times, the diagram is returned just once (DISTINCT).
 *
 *  \param this_ pointer to own object attributes
 *  \param classifier_id id of the classifier
 *  \param max_out_array_size size of the array where to store the results. If size is too small for the actual result set, this is an error.
 *  \param[out] out_diagram array of diagrams read from the database (in case of success)
 *  \param[out] out_diagram_count number of diagram records stored in out_diagram
 *  \return DATA_ERROR_NONE in case of success, an error code in case of error.
 *          DATA_ERROR_NO_DB if the database is not open,
 *          DATA_ERROR_ARRAY_BUFFER_EXCEEDED if the provided out buffers are too small.
 */
data_error_t data_database_reader_get_diagrams_by_classifier_id ( data_database_reader_t *this_,
                                                                  data_row_id_t classifier_id,
                                                                  uint32_t max_out_array_size,
                                                                  data_diagram_t (*out_diagram)[],
                                                                  uint32_t *out_diagram_count
                                                                );
#endif

/*!
 *  \brief reads all child-diagram ids from the database
 *
 *  \param this_ pointer to own object attributes
 *  \param parent_id id of the parent diagram, DATA_ROW_ID_VOID to get all root diagram ids
 *  \param[out] out_diagram_ids set of diagram ids read from the database (in case of success). The provided set shall be initialized.
 *  \return DATA_ERROR_NONE in case of success, an error code in case of error.
 *          E.g. DATA_ERROR_NO_DB if the database is not open.
 */
data_error_t data_database_reader_get_diagram_ids_by_parent_id ( data_database_reader_t *this_,
                                                                 data_row_id_t parent_id,
                                                                 data_small_set_t *out_diagram_ids
                                                               );

/*!
 *  \brief reads all classifier-displaying diagrams ids from the database
 *
 *  If a diagram shows the classifier multiple times, the diagram is returned just once (DISTINCT).
 *
 *  \param this_ pointer to own object attributes
 *  \param classifier_id id of the classifier
 *  \param[out] out_diagram_ids set of diagram ids read from the database (in case of success). The provided set shall be initialized.
 *  \return DATA_ERROR_NONE in case of success, an error code in case of error.
 *          E.g. DATA_ERROR_NO_DB if the database is not open.
 */
data_error_t data_database_reader_get_diagram_ids_by_classifier_id ( data_database_reader_t *this_,
                                                                     data_row_id_t classifier_id,
                                                                     data_small_set_t *out_diagram_ids
                                                                   );

/* ================================ CLASSIFIER ================================ */

/*!
 *  \brief reads a classifier from the database
 *
 *  \param this_ pointer to own object attributes
 *  \param id the classifier to be read from the database
 *  \param[out] out_classifier the classifier read from the database (in case of success)
 *  \return DATA_ERROR_NONE in case of success, an error code in case of error.
 *          E.g. DATA_ERROR_DB_STRUCTURE if id does not exist or DATA_ERROR_NO_DB if the database is not open.
 */
data_error_t data_database_reader_get_classifier_by_id ( data_database_reader_t *this_, data_row_id_t id, data_classifier_t *out_classifier );

/*!
 *  \brief reads a classifier from the database
 *
 *  \param this_ pointer to own object attributes
 *  \param name the classifier to be read from the database
 *  \param[out] out_classifier the classifier read from the database (in case of success)
 *  \return DATA_ERROR_NONE in case of success, an error code in case of error.
 *          E.g. DATA_ERROR_DB_STRUCTURE if name does not exist or DATA_ERROR_NO_DB if the database is not open.
 */
data_error_t data_database_reader_get_classifier_by_name ( data_database_reader_t *this_, const char *name, data_classifier_t *out_classifier );

/*!
 *  \brief reads all classifiers of a diagram from the database.
 *
 *  If a classifier is contained multiple times in a diagram, it is returned multiple times in the out_visible_classifier result list.
 *
 *  \param this_ pointer to own object attributes
 *  \param diagram_id id of the containing diagram
 *  \param max_out_array_size size of the array where to store the results. If size is too small for the actual result set, this is an error.
 *  \param[out] out_visible_classifier array of classifiers (and diagramelements) read from the database (in case of success)
 *  \param[out] out_visible_classifier_count number of classifier records stored in out_visible_classifier
 *  \return DATA_ERROR_NONE in case of success, an error code in case of error.
 *          DATA_ERROR_NO_DB if the database is not open,
 *          DATA_ERROR_ARRAY_BUFFER_EXCEEDED if the provided out buffers are too small.
 */
data_error_t data_database_reader_get_classifiers_by_diagram_id ( data_database_reader_t *this_,
                                                                  data_row_id_t diagram_id,
                                                                  uint32_t max_out_array_size,
                                                                  data_visible_classifier_t (*out_visible_classifier)[],
                                                                  uint32_t *out_visible_classifier_count
                                                                );

/*!
 *  \brief iterates over all classifiers from the database.
 *
 *  classifiers are sorted by number of containment-parents, ascending.
 * 
 *  \param this_ pointer to own object attributes
 *  \param io_classifier_iterator iterator over all classifiers. The caller is responsible for initializing before 
 *                                and destroying this object afterwards.
 *  \return DATA_ERROR_NONE in case of success, an error code in case of error.
 *          E.g. DATA_ERROR_NO_DB if the database is not open.
 */
data_error_t data_database_reader_get_all_classifiers_iterator ( data_database_reader_t *this_,
                                                                 data_database_iterator_classifiers_t *io_classifier_iterator
                                                               );

/* ================================ DIAGRAMELEMENT ================================ */

/*!
 *  \brief reads a diagramelement from the database
 *
 *  \param this_ pointer to own object attributes
 *  \param id the diagramelement to be read from the database
 *  \param[out] out_diagramelement the diagramelement read from the database (in case of success)
 *  \return DATA_ERROR_NONE in case of success, an error code in case of error.
 *          E.g. DATA_ERROR_DB_STRUCTURE if id does not exist or DATA_ERROR_NO_DB if the database is not open.
 */
data_error_t data_database_reader_get_diagramelement_by_id ( data_database_reader_t *this_, data_row_id_t id, data_diagramelement_t *out_diagramelement );

/*!
 *  \brief reads all diagramelements of a diagram from the database
 *
 *  \param this_ pointer to own object attributes
 *  \param diagram_id id of the diagram
 *  \param max_out_array_size size of the array where to store the results. If size is too small for the actual result set, this is an error.
 *  \param[out] out_diagramelement array of diagramelements read from the database (in case of success)
 *  \param[out] out_diagramelement_count number of diagramelement records stored in out_diagramelement
 *  \return DATA_ERROR_NONE in case of success, an error code in case of error.
 *          DATA_ERROR_NO_DB if the database is not open,
 *          DATA_ERROR_ARRAY_BUFFER_EXCEEDED if the provided out buffers are too small.
 */
data_error_t data_database_reader_get_diagramelements_by_diagram_id ( data_database_reader_t *this_,
                                                                      data_row_id_t diagram_id,
                                                                      uint32_t max_out_array_size,
                                                                      data_diagramelement_t (*out_diagramelement)[],
                                                                      uint32_t *out_diagramelement_count
                                                                    );

/*!
 *  \brief reads all diagramelements of a classifier from the database
 *
 *  \param this_ pointer to own object attributes
 *  \param classifier_id id of the diagram
 *  \param max_out_array_size size of the array where to store the results. If size is too small for the actual result set, this is an error.
 *  \param[out] out_diagramelement array of diagramelements read from the database (in case of success)
 *  \param[out] out_diagramelement_count number of diagramelement records stored in out_diagramelement
 *  \return DATA_ERROR_NONE in case of success, an error code in case of error.
 *          DATA_ERROR_NO_DB if the database is not open,
 *          DATA_ERROR_ARRAY_BUFFER_EXCEEDED if the provided out buffers are too small.
 */
data_error_t data_database_reader_get_diagramelements_by_classifier_id ( data_database_reader_t *this_,
                                                                         data_row_id_t classifier_id,
                                                                         uint32_t max_out_array_size,
                                                                         data_diagramelement_t (*out_diagramelement)[],
                                                                         uint32_t *out_diagramelement_count
                                                                       );

/* ================================ FEATURE ================================ */

/*!
 *  \brief reads a feature from the database
 *
 *  \param this_ pointer to own object attributes
 *  \param id the feature to be read from the database
 *  \param[out] out_feature the feature read from the database (in case of success)
 *  \return DATA_ERROR_NONE in case of success, an error code in case of error.
 *          E.g. DATA_ERROR_DB_STRUCTURE if id does not exist or DATA_ERROR_NO_DB if the database is not open.
 */
data_error_t data_database_reader_get_feature_by_id ( data_database_reader_t *this_, data_row_id_t id, data_feature_t *out_feature );

/*!
 *  \brief reads all features of a classifier from the database
 *
 *  \param this_ pointer to own object attributes
 *  \param classifier_id id of the parent classifier
 *  \param max_out_array_size size of the array where to store the results. If size is too small for the actual result set, this is an error.
 *  \param[out] out_feature array of features read from the database (in case of success)
 *  \param[out] out_feature_count number of feature records stored in out_feature
 *  \return DATA_ERROR_NONE in case of success, an error code in case of error.
 *          DATA_ERROR_NO_DB if the database is not open,
 *          DATA_ERROR_ARRAY_BUFFER_EXCEEDED if the provided out buffers are too small.
 */
data_error_t data_database_reader_get_features_by_classifier_id ( data_database_reader_t *this_,
                                                                  data_row_id_t classifier_id,
                                                                  uint32_t max_out_array_size,
                                                                  data_feature_t (*out_feature)[],
                                                                  uint32_t *out_feature_count
                                                                );

/*!
 *  \brief reads all features of a diagram from the database
 *
 *  If a classifier is contained multiple times in a diagram, the classifiers associated features are returned only once nonetheless (DISTINCT).
 *
 *  \param this_ pointer to own object attributes
 *  \param diagram_id id of the containing diagram
 *  \param max_out_array_size size of the array where to store the results. If size is too small for the actual result set, this is an error.
 *  \param[out] out_feature array of features read from the database (in case of success)
 *  \param[out] out_feature_count number of feature records stored in out_feature
 *  \return DATA_ERROR_NONE in case of success, an error code in case of error.
 *          DATA_ERROR_NO_DB if the database is not open,
 *          DATA_ERROR_ARRAY_BUFFER_EXCEEDED if the provided out buffers are too small.
 */
data_error_t data_database_reader_get_features_by_diagram_id ( data_database_reader_t *this_,
                                                               data_row_id_t diagram_id,
                                                               uint32_t max_out_array_size,
                                                               data_feature_t (*out_feature)[],
                                                               uint32_t *out_feature_count
                                                             );

/* ================================ RELATIONSHIP ================================ */

/*!
 *  \brief reads a relationship from the database
 *
 *  \param this_ pointer to own object attributes
 *  \param id the relationship to be read from the database
 *  \param[out] out_relationship the relationship read from the database (in case of success)
 *  \return DATA_ERROR_NONE in case of success, an error code in case of error.
 *          E.g. DATA_ERROR_DB_STRUCTURE if id does not exist or DATA_ERROR_NO_DB if the database is not open.
 */
data_error_t data_database_reader_get_relationship_by_id ( data_database_reader_t *this_, data_row_id_t id, data_relationship_t *out_relationship );

/*!
 *  \brief reads all relationships of a classifier from the database
 *
 *  This includes relationships where the classifier is source-only, destination-only or both.
 *
 *  \param this_ pointer to own object attributes
 *  \param classifier_id id of the source(from) or destination(to) classifier
 *  \param max_out_array_size size of the array where to store the results. If size is too small for the actual result set, this is an error.
 *  \param[out] out_relationship array of relationships read from the database (in case of success)
 *  \param[out] out_relationship_count number of relationship records stored in out_relationship
 *  \return DATA_ERROR_NONE in case of success, an error code in case of error.
 *          DATA_ERROR_NO_DB if the database is not open,
 *          DATA_ERROR_ARRAY_BUFFER_EXCEEDED if the provided out buffers are too small.
 */
data_error_t data_database_reader_get_relationships_by_classifier_id ( data_database_reader_t *this_,
                                                                       data_row_id_t classifier_id,
                                                                       uint32_t max_out_array_size,
                                                                       data_relationship_t (*out_relationship)[],
                                                                       uint32_t *out_relationship_count
                                                                     );

/*!
 *  \brief reads all relationships of a feature from the database
 *
 *  This includes relationships where the feature is source-only, destination-only or both.
 *
 *  \param this_ pointer to own object attributes
 *  \param feature_id id of the source(from) or destination(to) feature; must not be DATA_ROW_ID_VOID.
 *  \param max_out_array_size size of the array where to store the results. If size is too small for the actual result set, this is an error.
 *  \param[out] out_relationship array of relationships read from the database (in case of success)
 *  \param[out] out_relationship_count number of relationship records stored in out_relationship
 *  \return DATA_ERROR_NONE in case of success, an error code in case of error.
 *          DATA_ERROR_NO_DB if the database is not open,
 *          DATA_ERROR_ARRAY_BUFFER_EXCEEDED if the provided out buffers are too small.
 */
data_error_t data_database_reader_get_relationships_by_feature_id ( data_database_reader_t *this_,
                                                                    data_row_id_t feature_id,
                                                                    uint32_t max_out_array_size,
                                                                    data_relationship_t (*out_relationship)[],
                                                                    uint32_t *out_relationship_count
                                                                  );

/*!
 *  \brief reads all relationships of a diagram from the database
 *
 *  If classifiers are contained multiple times in a diagram, each relationship is returned only once nonetheless (DISTINCT).
 *
 *  \param this_ pointer to own object attributes
 *  \param diagram_id id of the containing diagram
 *  \param max_out_array_size size of the array where to store the results. If size is too small for the actual result set, this is an error.
 *  \param[out] out_relationship array of relationships read from the database (in case of success)
 *  \param[out] out_relationship_count number of relationship records stored in out_relationship
 *  \return DATA_ERROR_NONE in case of success, an error code in case of error.
 *          DATA_ERROR_NO_DB if the database is not open,
 *          DATA_ERROR_ARRAY_BUFFER_EXCEEDED if the provided out buffers are too small.
 */
data_error_t data_database_reader_get_relationships_by_diagram_id ( data_database_reader_t *this_,
                                                                    data_row_id_t diagram_id,
                                                                    uint32_t max_out_array_size,
                                                                    data_relationship_t (*out_relationship)[],
                                                                    uint32_t *out_relationship_count
                                                                  );

/* ================================ private ================================ */

/*!
 *  \brief initializes the data_database_reader_t struct and allows access to the database after the database is opened
 *
 *  \param this_ pointer to own object attributes
 *  \return DATA_ERROR_NONE in case of success
 */
data_error_t data_database_reader_private_open ( data_database_reader_t *this_ );

/*!
 *  \brief closes the data_database_reader_t struct (prohibits access to the database) before the database is closed
 *
 *  \param this_ pointer to own object attributes
 *  \return DATA_ERROR_NONE in case of success
 */
data_error_t data_database_reader_private_close ( data_database_reader_t *this_ );

/*!
 *  \brief creates a prepared statement.
 *
 *  \param this_ pointer to own object attributes
 *  \param string_statement statement as string to be prepared
 *  \param string_size size of string_statement in bytes, including the terminating zero
 *  \param[out] out_statement_ptr address of a pointer. The pointer is modifies as to point to a statement object.
 *  \return DATA_ERROR_NONE in case of success, an error code in case of error.
 */
static inline data_error_t data_database_reader_private_prepare_statement ( data_database_reader_t *this_,
                                                                            const char *string_statement,
                                                                            int string_size,
                                                                            sqlite3_stmt **out_statement_ptr
                                                                          );

/*!
 *  \brief finalizes a prepared statement.
 *
 *  \param this_ pointer to own object attributes
 *  \param statement_ptr pointer to a statement object
 *  \return DATA_ERROR_NONE in case of success, an error code in case of error.
 */
static inline data_error_t data_database_reader_private_finalize_statement ( data_database_reader_t *this_, sqlite3_stmt *statement_ptr );

/*!
 *  \brief resets a statement only, without binding.
 *
 *  \param this_ pointer to own object attributes
 *  \param statement_ptr pointer to a statement object
 *  \return DATA_ERROR_NONE in case of success, an error code in case of error.
 */
static inline data_error_t data_database_reader_private_bind_void_to_statement ( data_database_reader_t *this_, sqlite3_stmt *statement_ptr );

/*!
 *  \brief binds a single integer to a prepared statement (after reset).
 *
 *  The prepared statement shall have only one variable of type integer.
 *
 *  \param this_ pointer to own object attributes
 *  \param statement_ptr pointer to a statement object
 *  \param id integer to bind to the prepared statement. DATA_ROW_ID_VOID does not work because VOID is mapped to NULL and cannot be selected by the = operator.
 *  \return DATA_ERROR_NONE in case of success, an error code in case of error.
 */
static inline data_error_t data_database_reader_private_bind_id_to_statement ( data_database_reader_t *this_, sqlite3_stmt *statement_ptr, data_row_id_t id );

/*!
 *  \brief binds two integers to a prepared statement (after reset).
 *
 *  The prepared statement shall have exactly two variables of type integer.
 *
 *  \param this_ pointer to own object attributes
 *  \param statement_ptr pointer to a statement object
 *  \param id1 first integer to bind to the prepared statement. DATA_ROW_ID_VOID does not work because VOID is mapped to NULL and cannot be selected by the = operator.
 *  \param id2 second integer to bind to the prepared statement. DATA_ROW_ID_VOID does not work because VOID is mapped to NULL and cannot be selected by the = operator.
 *  \return DATA_ERROR_NONE in case of success, an error code in case of error.
 */
static inline data_error_t data_database_reader_private_bind_two_ids_to_statement ( data_database_reader_t *this_, sqlite3_stmt *statement_ptr, data_row_id_t id1, data_row_id_t id2 );

/*!
 *  \brief binds a single string to a prepared statement (after reset).
 *
 *  The prepared statement shall have only one variable of type string.
 *
 *  \param this_ pointer to own object attributes
 *  \param statement_ptr pointer to a statement object
 *  \param text char sequence to bind to the prepared statement.
 *  \return DATA_ERROR_NONE in case of success, an error code in case of error.
 */
static inline data_error_t data_database_reader_private_bind_text_to_statement ( data_database_reader_t *this_, sqlite3_stmt *statement_ptr, const char *text );

#include "storage/data_database_reader.inl"

#endif  /* DATA_DATABASE_READER_H */


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
