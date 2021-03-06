/* File: utf8stringview.h; Copyright and License: see below */

#ifndef UTF8STRINGVIEW_H_
#define UTF8STRINGVIEW_H_

/*!
 *  \file utf8stringview.h
 *  \brief utf8stringview provides functions to search and compare non-terminated char sequences.
 *
 *  utf8stringview provides a struct consisting of
 *  \li a pointer to an character array (non null-terminated) and
 *  \li the length of the array.
 *
 *  \note License: Use this code according to the license: Apache 2.0.
 *  \author Copyright 2021-2021 A.Warnke; Email-contact: utf8stringbuf-at-andreaswarnke-dot-de
 */

#include "util/string/utf8codepoint.h"
#include "util/string/utf8error.h"
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/*!
 *  \def UTF8STRINGVIEW(start_param,length_param)
 *  \brief Macro to facilitate static initialisation of an utf8stringview_t
 *
 *  \n
 *  \note Performance-Rating: [x]single-operation   [ ]fast   [ ]medium   [ ]slow ;   Performance-Class: O(1)
 */
#define UTF8STRINGVIEW(start_param,length_param) (utf8stringview_t){.start=start_param,.length=length_param}

/*!
 *  \def UTF8STRINGVIEW_STR(string_param)
 *  \brief Macro to facilitate static initialisation of an utf8stringview_t from a null-terminated string
 *
 *  \n
 *  \note Performance-Rating: [x]single-operation   [ ]fast   [ ]medium   [ ]slow ;   Performance-Class: O(1)
 */
#define UTF8STRINGVIEW_STR(string_param) (utf8stringview_t){.start=string_param,.length=(string_param==NULL)?0:strlen(string_param)}

/*!
 *  \def UTF8STRINGVIEW_NULL
 *  \brief Macro to facilitate static initialisation of an utf8stringview_t
 *
 *  \n
 *  \note Performance-Rating: [x]single-operation   [ ]fast   [ ]medium   [ ]slow ;   Performance-Class: O(1)
 */
#define UTF8STRINGVIEW_NULL (utf8stringview_t){.start=NULL,.length=0}

/*!
 *  \brief A string view is a pair of start pointer and length in bytes.
 *
 *  \invariant if length!=0 then start!=NULL.
 */
struct utf8stringview_struct {
    const char* start;
    size_t length;
};

/*!
 *  \typedef utf8stringview_t
 *  \brief The string view object
 * 
 *  It represents a non-null terminated immutable sequence of utf8 characters.
 */
typedef struct utf8stringview_struct utf8stringview_t;

/*!
 * \brief utf8stringview_init returns a stringview struct
 *
 * \note Performance-Rating: [x]single-operation   [ ]fast   [ ]medium   [ ]slow ;   Performance-Class: O(1)
 * \param start pointer to a byte array.
 * \param length length of the byte array. In case start is NULL, length must be 0.
 * \return A valid utf8stringview_t struct.
 */
static inline utf8stringview_t utf8stringview_init( const char* start, size_t length );

/*!
 * \brief utf8stringview_init returns a stringview struct
 *
 * \note Performance-Rating: [ ]single-operation   [x]fast   [ ]medium   [ ]slow ;   Performance-Class: O(n), n:strlen
 * \param cstring a 0-terminated string.
 * \return A valid utf8stringview_t struct.
 */
static inline utf8stringview_t utf8stringview_init_str( const char* cstring );

/*!
 * \brief utf8stringview_init returns a stringview struct
 *
 * \note Performance-Rating: [x]single-operation   [ ]fast   [ ]medium   [ ]slow ;   Performance-Class: O(1)
 * \param string a pointer to a character array. NULL is not allowed.
 * \param start_idx the start index from where the stringview shall start.
 * \param length length of the stringview.
 * \return A valid utf8stringview_t struct.
 */
static inline utf8stringview_t utf8stringview_init_region( const char* string, size_t start_idx, size_t length );

/*!
 * \brief Gets the pointer to the start of the character array
 * \note Performance-Rating: [x]single-operation   [ ]fast   [ ]medium   [ ]slow ;   Performance-Class: O(1)
 * \param this_ The string view object
 * \return Pointer to the start of the character array
 */
static inline const char* utf8stringview_get_start( const utf8stringview_t this_ );

/*!
 * \brief Gets the length of the character array
 * \note Performance-Rating: [x]single-operation   [ ]fast   [ ]medium   [ ]slow ;   Performance-Class: O(1)
 * \param this_ The string view object
 * \return Length of the character array.
 */
static inline size_t utf8stringview_get_length( const utf8stringview_t this_ );

/*!
 * \brief Searches a pattern within a stringview
 * \note Performance-Rating: [ ]single-operation   [ ]fast   [x]medium   [ ]slow ;   Performance-Class: O(n*m), n:strlen, m:patternlen
 * \param this_ The 0-terminated string within which to search
 * \param pattern The 0-terminated string to search
 * \return Index of the first occurrence within the stringview.
 *         -1 if there is no match.
 */
static inline int utf8stringview_find_first_str( const utf8stringview_t this_, const char *pattern );

#ifdef __cplusplus
}
#endif

#include "util/string/utf8stringview.inl"

#endif /*UTF8STRINGVIEW_H_*/


/*
 * Copyright 2021-2021 Andreas Warnke
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
