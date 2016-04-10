/*!
 *  \file utf8string.inl
 * 
 *  \note License: Use this code according to the license: Apache 2.0.
 *  \author (c) 2012-2016 A.Warnke; Email-contact: utf8stringbuf-at-andreaswarnke-dot-de
 */ 

/*#ifndef UTF8STRING_INL_*/
/*#define UTF8STRING_INL_*/

#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include "util/string/utf8codepoint.h"
#include "util/string/utf8stringbuf.h"

#ifdef __cplusplus
extern "C" {
#endif
    
/*!
 *  \enum utf8string_bool_enum
 *  \private
 */
/*  enumeration for true and false or success and failure */
enum utf8string_bool_enum {UTF8STRING_FALSE=0, UTF8STRING_TRUE=1,};

/*!
 *  \enum utf8string_search_enum
 *  \private
 */
/*  enumeration for search pattern not found */
enum utf8string_search_enum {UTF8STRING_NOT_FOUND=-1,};

static inline unsigned int utf8string_get_size( const char *this_ ) {
    unsigned int sizeResult = 0;
    if ( this_ != NULL ) {
        sizeResult = strlen( this_ ) + 1; /* 1 for size of terminating 0 character */
    }
    return sizeResult;
}

static inline unsigned int utf8string_get_length( const char *this_ ) {
    unsigned int lenResult = 0;
    if ( this_ != NULL ) {
        lenResult = strlen( this_ );
    }
    return lenResult;
}

static inline int utf8string_equals_str( const char *this_, const char *that ) {
    int cmpResult = -1;
    if (( this_ != NULL ) && ( that != NULL )) {
        cmpResult = strcmp( this_, that );
    }
    return ( cmpResult == 0 ) ? UTF8STRING_TRUE : UTF8STRING_FALSE;
}

static inline int utf8string_equals_buf( const char *this_, const utf8stringbuf_t that ) {
    int cmpResult = -1;
    if ( this_ != NULL ) {
        cmpResult = strcmp( this_, that.buf );
    }
    return ( cmpResult == 0 ) ? UTF8STRING_TRUE : UTF8STRING_FALSE;
}

static inline int utf8string_equals_region_str( const char *this_, int start, const char *that ) {
    int cmpResult = -1;
    if (( this_ != NULL ) && ( that != NULL )) {
        int thisLen = strlen(this_);
        int thatLen = strlen(that);
        if (( 0 <= start )&&( start+thatLen <= thisLen )) {
            cmpResult = memcmp( &(this_[start]), that, thatLen );
        }
    }
    return ( cmpResult == 0 ) ? UTF8STRINGBUF_TRUE : UTF8STRINGBUF_FALSE;
}

static inline int utf8string_equals_region_buf( const char *this_, int start, const utf8stringbuf_t that ) {
    int cmpResult = -1;
    if ( this_ != NULL ) {
        int thisLen = strlen(this_);
        int thatLen = strlen(that.buf);
        if (( 0 <= start )&&( start+thatLen <= thisLen )) {
            cmpResult = memcmp( &(this_[start]), that.buf, thatLen );
        }
    }
    return ( cmpResult == 0 ) ? UTF8STRINGBUF_TRUE : UTF8STRINGBUF_FALSE;
}

static inline int utf8string_starts_with_str( const char *this_, const char *that ) {
    int cmpResult = -1;
    if (( this_ != NULL )&&( that != NULL )) {
        unsigned int thatLen = strlen( that );
        cmpResult = strncmp( this_, that, thatLen );
    }
    return ( cmpResult == 0 ) ? UTF8STRING_TRUE : UTF8STRING_FALSE;
}

static inline int utf8string_starts_with_buf( const char *this_, const utf8stringbuf_t that ) {
    int cmpResult = -1;
    if ( this_ != NULL ) {
        unsigned int thatLen = strlen( that.buf );
        cmpResult = strncmp( this_, that.buf, thatLen );
    }
    return ( cmpResult == 0 ) ? UTF8STRING_TRUE : UTF8STRING_FALSE;
}

static inline int utf8string_ends_with_str( const char *this_, const char *that ) {
    int cmpResult = -1;
    if (( this_ != NULL )&&( that != NULL )) {
        unsigned int thatLen = strlen( that );
        unsigned int thisLen = strlen( this_ );
        if ( thatLen <= thisLen ) {
            cmpResult = memcmp( &(this_[thisLen-thatLen]), that, thatLen );
        }
    }
    return ( cmpResult == 0 ) ? UTF8STRING_TRUE : UTF8STRING_FALSE;
}

static inline int utf8string_ends_with_buf( const char *this_, const utf8stringbuf_t that ) {
    int cmpResult = -1;
    if ( this_ != NULL ) {
        unsigned int thatLen = strlen( that.buf );
        unsigned int thisLen = strlen( this_ );
        if ( thatLen <= thisLen ) {
            cmpResult = memcmp( &(this_[thisLen-thatLen]), that.buf, thatLen );
        }
    }
    return ( cmpResult == 0 ) ? UTF8STRING_TRUE : UTF8STRING_FALSE;
}

static inline int utf8string_find_first_buf( const char *this_, const utf8stringbuf_t pattern ) {
    int result = UTF8STRING_NOT_FOUND;
    if ( this_ != NULL ) {
        const char *ptrResult = strstr( this_, pattern.buf );
        if ( ptrResult != NULL ) {
            result = (int) (ptrResult - this_);
        }
    }
    return result;
}

static inline int utf8string_find_first_str( const char *this_, const char *pattern ) {
    int result = UTF8STRING_NOT_FOUND;
    if (( pattern != NULL )&&( this_ != NULL )) {
        const char *ptrResult = strstr( this_, pattern );
        if ( ptrResult != NULL ) {
            result = (int) (ptrResult - this_);
        }
    }
    return result;
}

static inline int utf8string_find_last_buf( const char *this_, const utf8stringbuf_t pattern ) {
    int result = UTF8STRING_NOT_FOUND;
    if ( this_ != NULL ) {
        int thisLen = strlen( this_ );
        int patternLen = strlen( pattern.buf );
        if ( patternLen <= thisLen ) {
            for ( int probeIdx = (thisLen-patternLen); probeIdx >= 0; probeIdx --) {
                if ( 0 == memcmp( &(this_[probeIdx]), pattern.buf, patternLen )) {
                    /* last occurrence found! */
                    result = probeIdx;
                    break;
                }
            }
        }
    }
    return result;
}

static inline int utf8string_find_last_str( const char *this_, const char *pattern ) {
    int result = UTF8STRING_NOT_FOUND;
    if (( pattern != NULL )&&( this_ != NULL )) {
        int thisLen = strlen( this_ );
        int patternLen = strlen( pattern );
        if ( patternLen <= thisLen ) {
            for ( int probeIdx = (thisLen-patternLen); probeIdx >= 0; probeIdx --) {
                if ( 0 == memcmp( &(this_[probeIdx]), pattern, patternLen )) {
                    /* last occurrence found! */
                    result = probeIdx;
                    break;
                }
            }
        }
    }
    return result;
}

static inline int utf8string_find_next_buf( const char *this_, const utf8stringbuf_t pattern, int start_index ) {
    int result = UTF8STRING_NOT_FOUND;
    unsigned int thisSize = utf8string_get_size( this_ );
    if (( start_index >= 0 ) && ( start_index < thisSize )) {
        const char *ptrResult = strstr( &(this_[start_index]), pattern.buf );
        if ( ptrResult != NULL ) {
            result = (int) (ptrResult - this_);
        }
    }
    return result;
}

static inline int utf8string_find_next_str( const char *this_, const char *pattern, int start_index ) {
    int result = UTF8STRING_NOT_FOUND;
    unsigned int thisSize = utf8string_get_size( this_ );
    if (( pattern != NULL ) && ( start_index >= 0 ) && ( start_index < thisSize )) {
        const char *ptrResult = strstr( &(this_[start_index]), pattern );
        if ( ptrResult != NULL ) {
            result = (int) (ptrResult - this_);
        }
    }
    return result;
}

static inline utf8codepoint_t utf8string_get_char_at( const char *this_, unsigned int byte_index ) {
    utf8codepoint_t result = UTF8CODEPOINT_INVAL_CHAR;
    unsigned int thisSize = utf8string_get_size( this_ );
    if ( byte_index < thisSize ) {
        result = utf8codepoint_init( &(this_[byte_index]), thisSize-byte_index );
    }
    return result;
}

#ifdef __cplusplus
}
#endif

/*#endif*/ /*UTF8STRING_INL_*/