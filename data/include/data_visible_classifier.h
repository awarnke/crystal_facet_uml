/* File: data_visible_classifier.h; Copyright and License: see below */

#ifndef DATA_VISIBLE_CLASSIFIER_H
#define DATA_VISIBLE_CLASSIFIER_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Defines a pair of classifier and diagramelement
 */

#include "data_classifier.h"
#include "data_diagramelement.h"

/*!
 *  \brief the pair of classifier and diagrameleemnt, which provide all information to display and modify and delete a classifier
 */
struct data_visible_classifier_struct {
    data_classifier_t classifier;  /*!< the classifier */
    data_diagramelement_t diagramelement;  /*!< the diagramelement */
};

typedef struct data_visible_classifier_struct data_visible_classifier_t;

/*!
 *  \brief initializes the data_visible_classifier_t struct.
 *
 *  \param this_ pointer to own object attributes
 */
static inline void data_visible_classifier_init_empty ( data_visible_classifier_t *this_ );

/*!
 *  \brief destroys the data_visible_classifier_t struct
 *
 *  \param this_ pointer to own object attributes
 */
static inline void data_visible_classifier_destroy ( data_visible_classifier_t *this_ );

/*!
 *  \brief gets the pointer to the classifier
 *
 *  \param this_ pointer to own object attributes
 */
static inline data_classifier_t *data_visible_classifier_get_classifier_ptr ( data_visible_classifier_t *this_ );

/*!
 *  \brief gets the pointer to the diagramelement
 *
 *  \param this_ pointer to own object attributes
 */
static inline data_diagramelement_t *data_visible_classifier_get_diagramelement_ptr ( data_visible_classifier_t *this_ );

#include "data_visible_classifier.inl"

#endif  /* DATA_VISIBLE_CLASSIFIER_H */


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
