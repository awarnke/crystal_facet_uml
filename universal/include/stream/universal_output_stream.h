/* File: universal_output_stream.h; Copyright and License: see below */

#ifndef UNIVERSAL_OUTPUT_STREAM_H
#define UNIVERSAL_OUTPUT_STREAM_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Defines a pair of a) a pointer to a concrete instance of an interface
 *  and b) a pointer to objectdata that implements the interface.
 */

#include "stream/universal_output_stream_if.h"

/*!
 *  \brief object (vmt+data) of a universal_output_stream_t.
 *
 */
struct universal_output_stream_struct {
    const universal_output_stream_if_t* interface;  /*!< set of interface functions to write to a stream */
    void* objectdata;  /*!< object that implements writing to a stream, used in interface functions as this_ parameter */
};

typedef struct universal_output_stream_struct universal_output_stream_t;

/*!
 *  \brief initializes the universal_output_stream_t
 *
 *  Note: While init only initializes the \c universal_output_stream_t struct,
 *  destroy also calls the destroy function in \c universal_output_stream_if_t.
 *
 *  \param this_ pointer to own object attributes
 *  \param interface set of interface functions to write to a stream
 *  \param objectdata object that implements writing to a stream
 */
static inline void universal_output_stream_init( universal_output_stream_t *this_,
                                                 const universal_output_stream_if_t *interface,
                                                 void* objectdata
                                               );

/*!
 *  \brief destroys the universal_output_stream_t.
 *
 *  This function does NOT call \c destroy on the \c interface.
 *
 *  \param this_ pointer to own object attributes
 *  \result returns 0 if success, -1 in case of error
 */
static inline int universal_output_stream_destroy( universal_output_stream_t *this_ );

/*!
 *  \brief gets the set of interface functions
 *
 *  \param this_ pointer to own object attributes
 *  \result the set of interface functions
 */
static inline const universal_output_stream_if_t* universal_output_stream_get_interface ( universal_output_stream_t *this_ );

/*!
 *  \brief gets the object that implements writing to a stream
 *
 *  \param this_ pointer to own object attributes
 *  \result the object data that implements the interface
 */
static inline void* universal_output_stream_get_objectdata ( universal_output_stream_t *this_ );

/*!
 *  \brief calls \c open on the \c interface
 *
 *  \param this_ pointer to own object attributes
 *  \param identifier identifier of the stream, e.g. a path in case of a file stream.
 *  \result returns 0 if success, -1 in case of error
 */
static inline int universal_output_stream_open (universal_output_stream_t* this_, const char* identifier);

/*!
 *  \brief calls \c write on the \c interface
 *
 *  \param this_ pointer to own object attributes
 *  \param start start address from where to write
 *  \param length length in bytes to write
 *  \result returns 0 if success, -1 in case of error
 */
static inline int universal_output_stream_write (universal_output_stream_t* this_, const void *start, size_t length);

/*!
 *  \brief calls \c flush on the \c interface
 *
 *  \param this_ pointer to own object attributes
 *  \result returns 0 if success, -1 in case of error
 */
static inline int universal_output_stream_flush (universal_output_stream_t* this_);

/*!
 *  \brief calls \c close on the \c interface
 *
 *  \param this_ pointer to own object attributes
 *  \result returns 0 if success, -1 in case of error
 */
static inline int universal_output_stream_close (universal_output_stream_t* this_);

#include "universal_output_stream.inl"

#endif  /* UNIVERSAL_OUTPUT_STREAM_H */


/*
Copyright 2020-2021 Andreas Warnke

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
