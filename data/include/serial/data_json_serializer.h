/* File: data_json_serializer.h; Copyright and License: see below */

#ifndef DATA_JSON_SERIALIZER_H
#define DATA_JSON_SERIALIZER_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Serializes data objects to json format.
 */

#include "data_error.h"
#include "data_classifier.h"
#include "data_diagram.h"
#include "util/string/utf8stringbuf.h"
#include <stdbool.h>

/*!
 *  \brief all data attributes needed for serializing data objects
 */
struct data_json_serializer_struct {
    bool in_array;  /*!< true if begin_array() was called and end_array() is not yet called. */
};

typedef struct data_json_serializer_struct data_json_serializer_t;

/*!
 *  \brief initializes the data_json_serializer_t struct
 *
 *  \param this_ pointer to own object attributes
 */
void data_json_serializer_init ( data_json_serializer_t *this_ );

/*!
 *  \brief destroys the data_json_serializer_t struct
 *
 *  \param this_ pointer to own object attributes
 */
void data_json_serializer_destroy ( data_json_serializer_t *this_ );

/*!
 *  \brief begins a json object containing an array of objects
 *
 *  \param this_ pointer to own object attributes
 *  \param out stringbuffer where to write the result to
 *  \return DATA_ERROR_NONE in case of success,
 *          DATA_ERROR_STRING_BUFFER_EXCEEDED if stringbuffer exceeded,
 *          DATA_ERROR_INVALID_REQUEST if data_json_serializer_begin_array() was already called.
 */
data_error_t data_json_serializer_begin_array ( data_json_serializer_t *this_, utf8stringbuf_t out );

/*!
 *  \brief ends a json object containing an array of objects
 *
 *  \param this_ pointer to own object attributes
 *  \param out stringbuffer where to write the result to
 *  \return DATA_ERROR_NONE in case of success,
 *          DATA_ERROR_STRING_BUFFER_EXCEEDED if stringbuffer exceeded,
 *          DATA_ERROR_INVALID_REQUEST if data_json_serializer_end_array() was already called.
 */
data_error_t data_json_serializer_end_array ( data_json_serializer_t *this_, utf8stringbuf_t out );

/*!
 *  \brief appends a classifier to the stringbuffer
 *
 *  \param this_ pointer to own object attributes
 *  \param object the object to serialize
 *  \param out stringbuffer where to write the result to
 *  \return DATA_ERROR_NONE in case of success,
 *          DATA_ERROR_STRING_BUFFER_EXCEEDED if stringbuffer exceeded,
 *          DATA_ERROR_INVALID_REQUEST if data_json_serializer_begin_array() was not yet called.
 */
data_error_t data_json_serializer_append_classifier ( data_json_serializer_t *this_, data_classifier_t object, utf8stringbuf_t out );

/*!
 *  \brief appends a diagram to the stringbuffer
 *
 *  \param this_ pointer to own object attributes
 *  \param object the object to serialize
 *  \param out stringbuffer where to write the result to
 *  \return DATA_ERROR_NONE in case of success,
 *          DATA_ERROR_STRING_BUFFER_EXCEEDED if stringbuffer exceeded,
 *          DATA_ERROR_INVALID_REQUEST if data_json_serializer_begin_array() was not yet called.
 */
data_error_t data_json_serializer_append_diagram ( data_json_serializer_t *this_, data_diagram_t object, utf8stringbuf_t out );

#endif  /* DATA_JSON_SERIALIZER_H */


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