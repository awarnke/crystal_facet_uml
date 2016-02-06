/* File: ctrl_classifier.h; Copyright and License: see below */

#ifndef CTRL_CLASSIFIER_H
#define CTRL_CLASSIFIER_H

/* public file for the doxygen documentation: */
/*! \file */

#include <stdio.h>
#include <stdbool.h>

/*!
 *  \brief all data attributes needed for the classifier functions
 */
struct ctrl_classifier_struct {
    bool is_open;
};

typedef struct ctrl_classifier_struct ctrl_classifier_t;

/*!
 *  \brief initializes the ctrl_classifier_t struct
 */
void ctrl_classifier_init ( ctrl_classifier_t *this_ );

/*!
 *  \brief destroys the ctrl_classifier_t struct
 */
void ctrl_classifier_destroy ( ctrl_classifier_t *this_ );


#endif  /* CTRL_CLASSIFIER_H */


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
