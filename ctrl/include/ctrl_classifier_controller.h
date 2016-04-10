/* File: ctrl_classifier_controller.h; Copyright and License: see below */

#ifndef CTRL_CLASSIFIER_CONTROLLER_H
#define CTRL_CLASSIFIER_CONTROLLER_H

/* public file for the doxygen documentation: */
/*! \file */

#include "ctrl_error.h"
#include "storage/data_database.h"
#include "storage/data_database_writer.h"
#include "storage/data_database_reader.h"
#include "data_classifier.h"
#include "data_classifier_type.h"
#include "data_diagram.h"
#include <stdio.h>
#include <stdbool.h>

/*!
 *  \brief all data attributes needed for the classifier functions
 */
struct ctrl_classifier_controller_struct {
    data_database_t *database;  /*!< pointer to external database */
    data_database_writer_t db_writer;  /*!< own instance of a database writer */
    data_database_reader_t db_reader;  /*!< own instance of a database reader */
};

typedef struct ctrl_classifier_controller_struct ctrl_classifier_controller_t;

/*!
 *  \brief initializes the ctrl_classifier_controller_t struct
 *
 *  \param this_ pointer to own object attributes
 */
void ctrl_classifier_controller_init ( ctrl_classifier_controller_t *this_, data_database_t *database );

/*!
 *  \brief destroys the ctrl_classifier_controller_t struct
 *
 *  \param this_ pointer to own object attributes
 */
void ctrl_classifier_controller_destroy ( ctrl_classifier_controller_t *this_ );

/*!
 *  \brief creates a new classifier or package or other object and attaches it to a diagram
 *
 *  \param this_ pointer to own object attributes
 *  \param out_new_id of the newly created object
 *  \return error id in case of an error, CTRL_ERROR_NONE otherwise
 */
ctrl_error_t ctrl_classifier_controller_create_classifier_in_diagram ( ctrl_classifier_controller_t *this_, int64_t diagram_id, data_classifier_type_t classifier_type, const char* classifier_name, int64_t* out_new_id );

#endif  /* CTRL_CLASSIFIER_CONTROLLER_H */


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