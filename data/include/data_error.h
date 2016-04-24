/* File: data_error.h; Copyright and License: see below */

#ifndef DATA_ERROR_H
#define DATA_ERROR_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Notifies errors in the data module
 */

/*!
 *  \brief error constants which explain errors which occurred in the data module
 *
 *  To be able to collect multiple errors over several statements,
 *  and evaluate multiple errors only once at the end of a block, the pattern for values is
 *  0xff000020 where all unused higher bits are set and only one of the lower 6*4=24 bits is set.
 *
 */
enum data_error_enum {
    DATA_ERROR_NONE = (0x00),  /*!< success */
    DATA_ERROR_NO_DB = (~(0x00fffffe)),  /*!< database not open/loaded */
    DATA_ERROR_DB_STRUCTURE = (~(0x00fffffd)),  /*!< the structure of the database is corrupted */
    DATA_ERROR_STRING_BUFFER_EXCEEDED = (~(0x00fffffb)),  /*!< a string does not fit to the string storage buffer */
    DATA_ERROR_ARRAY_BUFFER_EXCEEDED = (~(0x00fffff7)),  /*!< a set ob objects does not fit to the array storage buffer */
    DATA_ERROR_INVALID_REQUEST = (~(0x00ffffef)),  /*!< function call not allowed or parameters wrong */
    DATA_ERROR_AT_MUTEX = (~(0x00feffff)),  /*!< unexpected internal error at mutex */
    DATA_ERROR_AT_DB = (~(0x00fdffff)),  /*!< unexpected internal error at database */
    DATA_ERROR_DUPLICATE_ID = (~(0x00fbffff)),  /*!< nexpected internal error: an id is used twice */
    DATA_ERROR_MASK = (0x00ffffff),  /*!< a mask to filter error bits after collecting possibly multiple errors */
};

typedef enum data_error_enum data_error_t;

#endif  /* DATA_ERROR_H */


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
