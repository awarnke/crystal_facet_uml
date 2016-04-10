/*!
 *  \file utf8stringbuf.c
 *
 *  \note License: Use this code according to the license: Apache 2.0.
 *  \author (c) 2012-2016 A.Warnke; Email-contact: utf8stringbuf-at-andreaswarnke-dot-de
 */

#include <inttypes.h>
#include "util/string/utf8stringbuf.h"

/* utf8stringbuf_private_empty_buf is constantly 0, but may be overwritten by a 0 - therefore stored in a read-writeable memory page */
char utf8stringbuf_private_empty_buf[1] = "";

const char *utf8stringbuf_private_format_signed_64_bit_int = "%" PRIi64;

const char *utf8stringbuf_private_format_64_bit_hex = "%" PRIx64;

/*!
 *  \fn utf8stringbuf_private_write_char( char *destination, unsigned int max_size, const uint32_t source )
 *  \private
 */
/* function to write a code point as utf8, returns the number of bytes written or an error code */
static inline int utf8stringbuf_private_write_char( char *destination, unsigned int max_size, const uint32_t source );

/* utf8 sequences longer or equal 2 bytes start with a byte with 2 highest bits set: 0xc0 */
/* utf8 sequences longer or equal 3 bytes start with a byte with 3 highest bits set: 0xe0 */
/* utf8 sequences longer or equal 4 bytes start with a byte with 4 highest bits set: 0xf0 */
/* utf8 sequences longer or equal 5 bytes start with a byte with 5 highest bits set: 0xf8 */
/* utf8 sequences longer or equal 6 bytes start with a byte with 6 highest bits set: 0xfc */
static const unsigned char utf8stringbuf_private_pattern_to_detect_half_utf8_sequences[7] = { 0, 0, 0xc0, 0xe0, 0xf0, 0xf8, 0xfc };

/* Note: There is some magic in the design of utf8 which makes the implementation of this function quite short */
unsigned int utf8_string_buf_private_make_null_termination( utf8stringbuf_t this_ ) {
    unsigned int truncatedLength;
    int clearAtEnd = 1;

    for ( int searchBackwards = 2; searchBackwards <= 6; searchBackwards ++ ) {
        if ( searchBackwards > this_.size ) {
            break;
        }
        char pattern = utf8stringbuf_private_pattern_to_detect_half_utf8_sequences[searchBackwards];
        if ( ( this_.buf[this_.size-searchBackwards] & pattern ) == pattern ) {
            clearAtEnd = searchBackwards;
            break;
        }
    }

    truncatedLength = this_.size - clearAtEnd;
    /* this_.buf[truncatedLength] = '\0'; */ /* Note: some functions like splitIn2 require complete zeroed trailings */
    memset( &(this_.buf[truncatedLength]), '\0', clearAtEnd );
    return truncatedLength;
}

utf8error_t utf8stringbuf_copy_region_from_buf( utf8stringbuf_t this_, const utf8stringbuf_t that, int start, int length ) {
    utf8error_t result;
    if (( start < 0 ) || ( length < 0 )) {
        this_.buf[0] = '\0';
        result = UTF8ERROR_OUT_OF_RANGE;
    }
    else {
        int thatLen = utf8stringbuf_get_length( that );
        if ( start > thatLen ) {
            this_.buf[0] = '\0';
            result = UTF8ERROR_OUT_OF_RANGE;
        }
        else if ( start+length > thatLen ) {
            strncpy( this_.buf, &(that.buf[start]), this_.size );
            utf8_string_buf_private_make_null_termination( this_ );
            result = UTF8ERROR_OUT_OF_RANGE;
        }
        else {
            if ( length >= this_.size ) {
                memcpy( this_.buf, &(that.buf[start]), this_.size-1);
                utf8_string_buf_private_make_null_termination( this_ );
                result = UTF8ERROR_TRUNCATED;
            }
            else {
                memcpy( this_.buf, &(that.buf[start]), length );
                this_.buf[length] = '\0';
                result = UTF8ERROR_SUCCESS;
            }
        }
    }
    return result;
}

utf8error_t utf8stringbuf_copy_region_from_str( utf8stringbuf_t this_, const char *that, int start, int length ) {
    utf8error_t result;
    if (( start < 0 ) || ( length < 0 )) {
        this_.buf[0] = '\0';
        result = UTF8ERROR_OUT_OF_RANGE;
    }
    else if ( that == NULL ) {
        this_.buf[0] = '\0';
        result = UTF8ERROR_NULL_PARAM;
    }
    else {
        int thatLen = strlen( that );
        if ( start > thatLen ) {
            this_.buf[0] = '\0';
            result = UTF8ERROR_OUT_OF_RANGE;
        }
        else if ( start+length > thatLen ) {
            strncpy( this_.buf, &(that[start]), this_.size );
            utf8_string_buf_private_make_null_termination( this_ );
            result = UTF8ERROR_OUT_OF_RANGE;
        }
        else {
            if ( length >= this_.size ) {
                memcpy( this_.buf, &(that[start]), this_.size-1);
                utf8_string_buf_private_make_null_termination( this_ );
                result = UTF8ERROR_TRUNCATED;
            }
            else {
                memcpy( this_.buf, &(that[start]), length );
                this_.buf[length] = '\0';
                result = UTF8ERROR_SUCCESS;
            }
        }
    }
    return result;
}

utf8error_t utf8stringbuf_append_char( utf8stringbuf_t this_, const uint32_t appendix ) {
    int result;
    unsigned int start = utf8stringbuf_get_length( this_ );
    int appendLen = utf8stringbuf_private_write_char( &(this_.buf[start]), this_.size - start - 1, appendix );
    if ( appendLen >= 0 ) {
        this_.buf[start+appendLen] = '\0';
        result = UTF8ERROR_SUCCESS;
    }
    else {
        result = (utf8error_t) appendLen;
    }
    return result;
}

utf8error_t utf8stringbuf_append_wstr( utf8stringbuf_t this_, const wchar_t *appendix ) {
    int result = UTF8ERROR_NULL_PARAM;
    if ( appendix != NULL ) {
        unsigned int start = utf8stringbuf_get_length( this_ );
        result = UTF8ERROR_SUCCESS;
        for( ; appendix[0]!=L'\0'; appendix = &(appendix[1]) ) {
            int appendLen = utf8stringbuf_private_write_char( &(this_.buf[start]), this_.size - start - 1, appendix[0] );
            if ( appendLen < 0 ) {
                result = (utf8error_t) appendLen;
                if ( result == UTF8ERROR_TRUNCATED ) {
                    break;
                }
            }
            start = start + appendLen;
        }
        this_.buf[start] = '\0';
    }
    return result;
}

static inline int utf8stringbuf_private_write_char( char *destination, unsigned int max_size, const uint32_t source ) {
    int result = UTF8ERROR_TRUNCATED;
    if ( source <= 0x7ff ) {
        if ( source <= 0x7f ) {
            /* 1 byte character */
            /* check if there is enough space for the character */
            if ( max_size >= 1 ) {
                destination[0] = source;
                result = 1;
            }
        }
        else {
            /* 2 byte character */
            if ( max_size >= 2 ) {
                destination[0] = 0xc0 | ( source >> 6 );
                destination[1] = 0x80 | ( source & 0x3f );
                result = 2;
            }
        }
    }
    else {
        if ( source <= 0x1fffff ) {
            if ( source <= 0xffff ) {
                /* 3 byte character */
                if ( max_size >= 3 ) {
                    destination[0] = 0xe0 | ( source >> 12 );
                    destination[1] = 0x80 | (( source >> 6 ) & 0x3f );
                    destination[2] = 0x80 | ( source & 0x3f );
                    result = 3;
                }
            }
            else {
                /* 4 byte character */
                if ( max_size >= 4 ) {
                    destination[0] = 0xf0 | ( source >> 18 );
                    destination[1] = 0x80 | (( source >> 12 ) & 0x3f );
                    destination[2] = 0x80 | (( source >> 6 ) & 0x3f );
                    destination[3] = 0x80 | ( source & 0x3f );
                    result = 4;
                }
            }
        }
        else {
            if ( source <= 0x3ffffff ) {
                /* 5 byte character */
                if ( max_size >= 5 ) {
                    destination[0] = 0xf8 | ( source >> 24 );
                    destination[1] = 0x80 | (( source >> 18 ) & 0x3f );
                    destination[2] = 0x80 | (( source >> 12 ) & 0x3f );
                    destination[3] = 0x80 | (( source >> 6 ) & 0x3f );
                    destination[4] = 0x80 | ( source & 0x3f );
                    result = 5;
                }
            }
            else {
                if ( source <= 0x7fffffff ) {
                    /* 6 byte character */
                    if ( max_size >= 6 ) {
                        destination[0] = 0xfc | ( source >> 30 );
                        destination[1] = 0x80 | (( source >> 24 ) & 0x3f );
                        destination[2] = 0x80 | (( source >> 18 ) & 0x3f );
                        destination[3] = 0x80 | (( source >> 12 ) & 0x3f );
                        destination[4] = 0x80 | (( source >> 6 ) & 0x3f );
                        destination[5] = 0x80 | ( source & 0x3f );
                        result = 6;
                    }
                }
                else {
                    /* note: utf8 can not encode more than 31 bits per character. */
                    result = UTF8ERROR_NOT_A_CODEPOINT;
                }
            }
        }
    }
    return result;
}

utf8error_t utf8_string_buf_private_replace_region_by_str( utf8stringbuf_t this_, unsigned int this_Length, int start, int length, const char *replacement ) {
    utf8error_t result = UTF8ERROR_OUT_OF_RANGE;
    if (( start >= 0 ) && ( start <= this_Length ) && ( length >= 0 )) {
        result = UTF8ERROR_SUCCESS;
        unsigned int replaceLen = ( replacement == NULL ) ? (0) : ( strlen(replacement) );
        if (( start + length ) > this_Length ) {
            /* length value was too big. update this; success is 0 */
            length = this_Length - start;
            result = UTF8ERROR_OUT_OF_RANGE;
        }
        int tailLen = this_Length - start - length;
        if ( length > replaceLen ) {
            memmove( &(this_.buf[start+replaceLen]), &(this_.buf[start+length]), tailLen+1 );
        }
        else if ( length < replaceLen ) {
            if ( ( start + replaceLen + tailLen ) < this_.size ) {
                memmove( &(this_.buf[start+replaceLen]), &(this_.buf[start+length]), tailLen+1 );
            }
            else if ( ( start + replaceLen ) < this_.size ) {
                tailLen = this_.size - start - replaceLen - 1;
                memmove( &(this_.buf[start+replaceLen]), &(this_.buf[start+length]), tailLen );
                result = UTF8ERROR_TRUNCATED;
            }
            else {
                replaceLen = this_.size - start - 1;
                result = UTF8ERROR_TRUNCATED;
            }
        }
        else {
            /* original and replacement strings have equal lengths */
        }
        /* replace string */
        if ( replaceLen > 0 ) {
            memcpy ( &(this_.buf[start]), replacement, replaceLen );
        }
        /* terminate string */
        if ( result != UTF8ERROR_SUCCESS ) {
            utf8_string_buf_private_make_null_termination( this_ );
        }
    }
    return result;
}

utf8error_t utf8stringbuf_replace_all( const utf8stringbuf_t this_, const char * const * patterns_and_replacements ) {
    utf8error_t result = UTF8ERROR_NULL_PARAM;

    /* count input patterns */
    int maxPatternIdx = 0;
    if ( patterns_and_replacements != NULL ) {
        result = UTF8ERROR_SUCCESS;
        for ( maxPatternIdx = 0; patterns_and_replacements[maxPatternIdx] != NULL; maxPatternIdx += 2 ) {
        };
    }

    /* search patterns */
    unsigned int thisLen = utf8stringbuf_get_length( this_ );
    for ( int index = 0; index < thisLen; index ++ ) {
        int matchingPatternIdx = -1;
        unsigned int remainingLength = thisLen-index;
        for ( int patternIdx = 0; ( patternIdx < maxPatternIdx )&&( matchingPatternIdx == -1 ); patternIdx += 2 ) {
            int finished = 0;
            for ( int compareIdx = 0; ( compareIdx <= remainingLength )&&( finished == 0 ); compareIdx ++ ) {
                char cmpChar = patterns_and_replacements[patternIdx][compareIdx];
                if ( cmpChar == '\0' ) {
                    if ( compareIdx != 0 ) {
                           /* all characters were equal (and there was at least one) */
                           matchingPatternIdx = patternIdx;
                    }
                    finished = 1;
                }
                if ( this_.buf[index+compareIdx] != cmpChar ) {
                    /* difference found */
                    finished = 1;
                }
            }
        }
        /* replace pattern */
        if ( matchingPatternIdx != -1 ) {
            int patternLen = strlen( patterns_and_replacements[matchingPatternIdx] );
            const char * replacement = patterns_and_replacements[matchingPatternIdx+1];
            int replaceLen = 0;
            if ( replacement != NULL ) {
                replaceLen = strlen(replacement);
            }
            utf8error_t replaceErr;
            replaceErr = utf8_string_buf_private_replace_region_by_str( this_, thisLen, index, patternLen, replacement );
            if ( replaceErr != UTF8ERROR_SUCCESS ) {
                result = UTF8ERROR_TRUNCATED;
                thisLen = utf8stringbuf_get_length( this_ );
            }
            else {
                thisLen = thisLen - patternLen + replaceLen;
            }
            index = index + replaceLen - 1;
        }
    }

    return result;
}

static const int SIZE_OF_TERM0 = 1; /*!< Size of the terminating zero */

utf8string2tuple_t utf8stringbuf_split_in_2( utf8stringbuf_t this_, int start2 ) {
    utf8string2tuple_t result;
    unsigned int length = utf8stringbuf_get_length( this_ );

    /* zero all trailing data, otherwise joins will not work */
    memset( &(this_.buf[length+SIZE_OF_TERM0]), '\0', this_.size - length - SIZE_OF_TERM0 );

    if (( 0 <= start2 )&&( start2 <= length )) {
        int newStart2 = start2+SIZE_OF_TERM0;
        if ( length+SIZE_OF_TERM0 < this_.size ) {
            /* everything fits */
            memmove( &(this_.buf[newStart2]), &(this_.buf[start2]), length-start2+SIZE_OF_TERM0 );
            this_.buf[newStart2-SIZE_OF_TERM0] = '\0';
            result.first = this_.buf;
            result.second = &(this_.buf[start2+1]);
            result.error = UTF8ERROR_SUCCESS;
        }
        else {
            /* we need to truncate */
            int secondLen = this_.size - SIZE_OF_TERM0 - newStart2;
            if ( secondLen <= 0 ) {
                secondLen = 0;
                newStart2 = this_.size - SIZE_OF_TERM0;
                this_.buf[newStart2] = '\0';
            }
            else {
                memmove( &(this_.buf[newStart2]), &(this_.buf[start2]), secondLen );
                utf8_string_buf_private_make_null_termination( utf8stringbuf_init( secondLen+SIZE_OF_TERM0, &(this_.buf[newStart2])) );
            }
            this_.buf[start2] = '\0';  /* at the end of the first string, always write a terminating zero */
            /*
            printf("\nFIRST=%i SECOND=%i \\0=%i TRAIL=%i [size=%i] -> FIRST=%i \\0=%i SECOND=%i \\0=%i [size=%i]",
                    start2, length-start2, SIZE_OF_TERM0, this_.size-length-SIZE_OF_TERM0, this_.size,
                    start2, newStart2-start2, secondLen, SIZE_OF_TERM0, this_.size  );
            */
            result.first = this_.buf;
            result.second = &(this_.buf[newStart2]);
            result.error = UTF8ERROR_TRUNCATED;

        }
    }
    else {
        /* out of range */
        result.first = this_.buf;
        result.second = &(this_.buf[length]);
        result.error = UTF8ERROR_OUT_OF_RANGE;
    }

    return result;
}

utf8string3tuple_t utf8stringbuf_split_in_3( utf8stringbuf_t this_, int start2, int start3 ) {
    utf8string3tuple_t result;
    unsigned int length = utf8stringbuf_get_length( this_ );

    /* zero all trailing data, otherwise joins will not work */
    memset( &(this_.buf[length+SIZE_OF_TERM0]), '\0', this_.size - length - SIZE_OF_TERM0 );

    if (( 0 <= start2 )&&( start2 <= start3 )&&( start3 <= length )) {
        int newStart2 = start2+SIZE_OF_TERM0;
        int newStart3 = start3+SIZE_OF_TERM0+SIZE_OF_TERM0;
        if ( length+SIZE_OF_TERM0+SIZE_OF_TERM0 < this_.size ) {
            /* everything fits */
            memmove( &(this_.buf[newStart3]), &(this_.buf[start3]), length-start3+SIZE_OF_TERM0 );
            this_.buf[newStart3-SIZE_OF_TERM0] = '\0';
            memmove( &(this_.buf[newStart2]), &(this_.buf[start2]), start3-start2 );
            this_.buf[newStart2-SIZE_OF_TERM0] = '\0';
            result.first = this_.buf;
            result.second = &(this_.buf[newStart2]);
            result.third = &(this_.buf[newStart3]);
            result.error = UTF8ERROR_SUCCESS;
        }
        else {
            /* we need to truncate */
            int thirdLen = this_.size - SIZE_OF_TERM0 - newStart3;
            if ( thirdLen <= 0 ) {
                thirdLen = 0;
                newStart3 = this_.size - SIZE_OF_TERM0;
                this_.buf[newStart3] = '\0';
            }
            else {
                memmove( &(this_.buf[newStart3]), &(this_.buf[start3]), thirdLen );
                utf8_string_buf_private_make_null_termination( utf8stringbuf_init( thirdLen+SIZE_OF_TERM0, &(this_.buf[newStart3])) );
            }
            int secondLen = newStart3 - SIZE_OF_TERM0 - newStart2;
            if ( secondLen < 0 ) {
                secondLen = 0;
                newStart2 = this_.size - SIZE_OF_TERM0;
            }
            else if ( secondLen == 0 ) {
                this_.buf[newStart2] = '\0';
            }
            else {
                memmove( &(this_.buf[newStart2]), &(this_.buf[start2]), secondLen );
                utf8_string_buf_private_make_null_termination( utf8stringbuf_init( secondLen+SIZE_OF_TERM0, &(this_.buf[newStart2])) );
            }
            this_.buf[start2] = '\0';  /* at the end of the first string, always write a terminating zero */
            /*
            printf("\nFIRST=%i SECOND=%i THIRD=%i \\0=%i TRAIL=%i [size=%i] -> FIRST=%i \\0=%i SECOND=%i \\0=%i THIRD=%i \\0=%i [size=%i]",
                    start2, start3-start2, length-start3, SIZE_OF_TERM0, this_.size-length-SIZE_OF_TERM0, this_.size,
                    start2, newStart2-start2, secondLen, (int)(newStart3!=newStart2), thirdLen, SIZE_OF_TERM0, this_.size  );
            */
            result.first = this_.buf;
            result.second = &(this_.buf[newStart2]);
            result.third = &(this_.buf[newStart3]);
            result.error = UTF8ERROR_TRUNCATED;
        }
    }
    else {
        /* out of range */
        result.first = this_.buf;
        result.second = &(this_.buf[length]);
        result.third = &(this_.buf[length]);
        result.error = UTF8ERROR_OUT_OF_RANGE;
    }

    return result;
}

utf8string4tuple_t utf8stringbuf_split_in_4( utf8stringbuf_t this_, int start2, int start3, int start4 ) {
    utf8string4tuple_t result;
    unsigned int length = utf8stringbuf_get_length( this_ );

    /* zero all trailing data, otherwise joins will not work */
    memset( &(this_.buf[length+SIZE_OF_TERM0]), '\0', this_.size - length - SIZE_OF_TERM0 );

    if (( 0 <= start2 )&&( start2 <= start3 )&&( start3 <= start4 )&&( start4 <= length )) {
        int newStart2 = start2+SIZE_OF_TERM0;
        int newStart3 = start3+SIZE_OF_TERM0+SIZE_OF_TERM0;
        int newStart4 = start4+SIZE_OF_TERM0+SIZE_OF_TERM0+SIZE_OF_TERM0;
        if ( length+SIZE_OF_TERM0+SIZE_OF_TERM0+SIZE_OF_TERM0 < this_.size ) {
            /* everything fits */
            memmove( &(this_.buf[newStart4]), &(this_.buf[start4]), length-start4+SIZE_OF_TERM0 );
            this_.buf[newStart4-SIZE_OF_TERM0] = '\0';
            memmove( &(this_.buf[newStart3]), &(this_.buf[start3]), start4-start3 );
            this_.buf[newStart3-SIZE_OF_TERM0] = '\0';
            memmove( &(this_.buf[newStart2]), &(this_.buf[start2]), start3-start2 );
            this_.buf[newStart2-SIZE_OF_TERM0] = '\0';
            result.first = this_.buf;
            result.second = &(this_.buf[newStart2]);
            result.third = &(this_.buf[newStart3]);
            result.fourth = &(this_.buf[newStart4]);
            result.error = UTF8ERROR_SUCCESS;
        }
        else {
            /* we need to truncate */
            int fourthLen = this_.size - SIZE_OF_TERM0 - newStart4;
            if ( fourthLen <= 0 ) {
                fourthLen = 0;
                newStart4 = this_.size - SIZE_OF_TERM0;
                this_.buf[newStart4] = '\0';
            }
            else {
                memmove( &(this_.buf[newStart4]), &(this_.buf[start4]), fourthLen );
                utf8_string_buf_private_make_null_termination( utf8stringbuf_init( fourthLen+SIZE_OF_TERM0, &(this_.buf[newStart4])) );
            }
            int thirdLen = newStart4 - SIZE_OF_TERM0 - newStart3;
            if ( thirdLen < 0 ) {
                thirdLen = 0;
                newStart3 = this_.size - SIZE_OF_TERM0;
            }
            else if ( thirdLen == 0 ) {
                this_.buf[newStart3] = '\0';
            }
            else {
                memmove( &(this_.buf[newStart3]), &(this_.buf[start3]), thirdLen );
                utf8_string_buf_private_make_null_termination( utf8stringbuf_init( thirdLen+SIZE_OF_TERM0, &(this_.buf[newStart3])) );
            }
            int secondLen = newStart3 - SIZE_OF_TERM0 - newStart2;
            if ( secondLen < 0 ) {
                secondLen = 0;
                newStart2 = this_.size - SIZE_OF_TERM0;
            }
            else if ( secondLen == 0 ) {
                this_.buf[newStart2] = '\0';
            }
            else {
                memmove( &(this_.buf[newStart2]), &(this_.buf[start2]), secondLen );
                utf8_string_buf_private_make_null_termination( utf8stringbuf_init( secondLen+SIZE_OF_TERM0, &(this_.buf[newStart2])) );
            }
            this_.buf[start2] = '\0';  /* at the end of the first string, always write a terminating zero */
            /*
            printf("\nFIRST=%i SECOND=%i THIRD=%i FOURTH=%i \\0=%i TRAIL=%i [size=%i] -> FIRST=%i \\0=%i SECOND=%i \\0=%i THIRD=%i \\0=%i FOURTH=%i \\0=%i [size=%i]",
                    start2, start3-start2, start4-start3, length-start4, SIZE_OF_TERM0, this_.size-length-SIZE_OF_TERM0, this_.size,
                    start2, newStart2-start2, secondLen, (int)(newStart3!=newStart2), thirdLen, (int)(newStart4!=newStart3), fourthLen, SIZE_OF_TERM0, this_.size  );
            */
            result.first = this_.buf;
            result.second = &(this_.buf[newStart2]);
            result.third = &(this_.buf[newStart3]);
            result.fourth = &(this_.buf[newStart4]);
            result.error = UTF8ERROR_TRUNCATED;
        }
    }
    else {
        /* out of range */
        result.first = this_.buf;
        result.second = &(this_.buf[length]);
        result.third = &(this_.buf[length]);
        result.fourth = &(this_.buf[length]);
        result.error = UTF8ERROR_OUT_OF_RANGE;
    }

    return result;
}

utf8string5tuple_t utf8stringbuf_split_in_5( utf8stringbuf_t this_, int start2, int start3, int start4, int start5 ) {
    utf8string5tuple_t result;
    unsigned int length = utf8stringbuf_get_length( this_ );

    /* zero all trailing data, otherwise joins will not work */
    memset( &(this_.buf[length+SIZE_OF_TERM0]), '\0', this_.size - length - SIZE_OF_TERM0 );

    if (( 0 <= start2 )&&( start2 <= start3 )&&( start3 <= start4 )&&( start4 <= start5 )&&( start5 <= length )) {
        int newStart2 = start2+SIZE_OF_TERM0;
        int newStart3 = start3+SIZE_OF_TERM0+SIZE_OF_TERM0;
        int newStart4 = start4+SIZE_OF_TERM0+SIZE_OF_TERM0+SIZE_OF_TERM0;
        int newStart5 = start4+SIZE_OF_TERM0+SIZE_OF_TERM0+SIZE_OF_TERM0+SIZE_OF_TERM0;
        if ( length+SIZE_OF_TERM0+SIZE_OF_TERM0+SIZE_OF_TERM0+SIZE_OF_TERM0 < this_.size ) {
            /* everything fits */
            memmove( &(this_.buf[newStart5]), &(this_.buf[start5]), length-start5+SIZE_OF_TERM0 );
            this_.buf[newStart5-SIZE_OF_TERM0] = '\0';
            memmove( &(this_.buf[newStart4]), &(this_.buf[start4]), start5-start4 );
            this_.buf[newStart4-SIZE_OF_TERM0] = '\0';
            memmove( &(this_.buf[newStart3]), &(this_.buf[start3]), start4-start3 );
            this_.buf[newStart3-SIZE_OF_TERM0] = '\0';
            memmove( &(this_.buf[newStart2]), &(this_.buf[start2]), start3-start2 );
            this_.buf[newStart2-SIZE_OF_TERM0] = '\0';
            result.first = this_.buf;
            result.second = &(this_.buf[newStart2]);
            result.third = &(this_.buf[newStart3]);
            result.fourth = &(this_.buf[newStart4]);
            result.fifth = &(this_.buf[newStart5]);
            result.error = UTF8ERROR_SUCCESS;
        }
        else {
            /* we need to truncate */
            int fifthLen = this_.size - SIZE_OF_TERM0 - newStart5;
            if ( fifthLen <= 0 ) {
                fifthLen = 0;
                newStart5 = this_.size - SIZE_OF_TERM0;
                this_.buf[newStart5] = '\0';
            }
            else {
                memmove( &(this_.buf[newStart5]), &(this_.buf[start5]), fifthLen );
                utf8_string_buf_private_make_null_termination( utf8stringbuf_init( fifthLen+SIZE_OF_TERM0, &(this_.buf[newStart5])) );
            }
            int fourthLen = newStart5 - SIZE_OF_TERM0 - newStart4;
            if ( fourthLen < 0 ) {
                fourthLen = 0;
                newStart4 = this_.size - SIZE_OF_TERM0;
            }
            else if ( fourthLen == 0 ) {
                this_.buf[newStart4] = '\0';
            }
            else {
                memmove( &(this_.buf[newStart4]), &(this_.buf[start4]), fourthLen );
                utf8_string_buf_private_make_null_termination( utf8stringbuf_init( fourthLen+SIZE_OF_TERM0, &(this_.buf[newStart4])) );
            }
            int thirdLen = newStart4 - SIZE_OF_TERM0 - newStart3;
            if ( thirdLen < 0 ) {
                thirdLen = 0;
                newStart3 = this_.size - SIZE_OF_TERM0;
            }
            else if ( thirdLen == 0 ) {
                this_.buf[newStart3] = '\0';
            }
            else {
                memmove( &(this_.buf[newStart3]), &(this_.buf[start3]), thirdLen );
                utf8_string_buf_private_make_null_termination( utf8stringbuf_init( thirdLen+SIZE_OF_TERM0, &(this_.buf[newStart3])) );
            }
            int secondLen = newStart3 - SIZE_OF_TERM0 - newStart2;
            if ( secondLen < 0 ) {
                secondLen = 0;
                newStart2 = this_.size - SIZE_OF_TERM0;
            }
            else if ( secondLen == 0 ) {
                this_.buf[newStart2] = '\0';
            }
            else {
                memmove( &(this_.buf[newStart2]), &(this_.buf[start2]), secondLen );
                utf8_string_buf_private_make_null_termination( utf8stringbuf_init( secondLen+SIZE_OF_TERM0, &(this_.buf[newStart2])) );
            }
            this_.buf[start2] = '\0';  /* at the end of the first string, always write a terminating zero */
            /*
            printf("\nFIRST=%i SECOND=%i THIRD=%i FOURTH=%i FIFTH=%i \\0=%i TRAIL=%i [size=%i] -> FIRST=%i \\0=%i SECOND=%i \\0=%i THIRD=%i \\0=%i FOURTH=%i \\0=%i FIFTH=%i \\0=%i [size=%i]",
                    start2, start3-start2, start4-start3, start5-start4, length-start5, SIZE_OF_TERM0, this_.size-length-SIZE_OF_TERM0, this_.size,
                    start2, newStart2-start2, secondLen, (int)(newStart3!=newStart2), thirdLen, (int)(newStart4!=newStart3), fourthLen, (int)(newStart5!=newStart4), fifthLen, SIZE_OF_TERM0, this_.size  );
            */
            result.first = this_.buf;
            result.second = &(this_.buf[newStart2]);
            result.third = &(this_.buf[newStart3]);
            result.fourth = &(this_.buf[newStart4]);
            result.fifth = &(this_.buf[newStart5]);
            result.error = UTF8ERROR_TRUNCATED;
        }
    }
    else {
        /* out of range */
        result.first = this_.buf;
        result.second = &(this_.buf[length]);
        result.third = &(this_.buf[length]);
        result.fourth = &(this_.buf[length]);
        result.fifth = &(this_.buf[length]);
        result.error = UTF8ERROR_OUT_OF_RANGE;
    }

    return result;
}

void utf8stringbuf_join( utf8stringbuf_t this_ ) {
    /* search first terminating zero, start from there. */
    unsigned int currentReadPos = strlen( this_.buf );
    unsigned int currentWritePos = currentReadPos;
    unsigned int confirmedLength = currentReadPos;

    /* copy all characters except the zeroes */
    for ( ; currentReadPos < this_.size; currentReadPos ++ ) {
        char buf = this_.buf[currentReadPos];
        if ( buf == '\0' ) {
            confirmedLength = currentWritePos;
        }
        else {
            this_.buf[currentWritePos] = buf;
            currentWritePos ++;
        }
    }

    /* set all trailing characters to 0 */
    memset( &(this_.buf[confirmedLength]), '\0', this_.size - confirmedLength );
}