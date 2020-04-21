/* File: md_filter.c; Copyright and License: see below */

#include "md/md_filter.h"
#include "trace.h"
#include "tslog.h"
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

/* Note, this is no full markdown syntax support - but it helps keeping markdown in shape */
// static const char * const MD_FILTER_PRIVATE_ENCODE_FMT_DB_STRINGS[] = {
//     "<", "&lt;",
//     ">", "&gt;",
//     "\"", "&quot;",
//     "&", "&amp;",
//     "\n\n", "</para>\n<para>",
//     "\n+", "</para>\n<para>+",  /* markdown list */
//     "\n-", "</para>\n<para>-",  /* markdown list or heading */
//     "\n*", "</para>\n<para>*",  /* markdown list */
//     "\n0", "</para>\n<para>0",  /* markdown list */
//     "\n1", "</para>\n<para>1",  /* markdown list */
//     "\n2", "</para>\n<para>2",  /* markdown list */
//     "\n3", "</para>\n<para>3",  /* markdown list */
//     "\n4", "</para>\n<para>4",  /* markdown list */
//     "\n5", "</para>\n<para>5",  /* markdown list */
//     "\n6", "</para>\n<para>6",  /* markdown list */
//     "\n7", "</para>\n<para>7",  /* markdown list */
//     "\n8", "</para>\n<para>8",  /* markdown list */
//     "\n9", "</para>\n<para>9",  /* markdown list */
//     "\n>", "</para>\n<para>&gt;",  /* markdown citation */
//     "\n=", "</para>\n<para>=",  /* markdown heading */
//     "\n#", "</para>\n<para>#",  /* markdown heading */
//     "\n ", "</para>\n<para>&#xA0;",  /* markdown list-entry continuation */
//     "\n|", "</para>\n<para>|",  /* markdown table */
//     NULL,  /* end translation table */
// };

/* Note, this is no full markdown syntax support - but it helps keeping markdown in shape */
// static const char * const MD_FILTER_PRIVATE_ENCODE_FMT_XHTML_STRINGS[] = {
//     "<", "&lt;",
//     ">", "&gt;",
//     "\"", "&quot;",
//     "&", "&amp;",
//     "\n\n", "<br />\n",
//     "\n+", "<br />+",  /* markdown list */
//     "\n-", "<br />-",  /* markdown list or heading */
//     "\n*", "<br />*",  /* markdown list */
//     "\n0", "<br />0",  /* markdown list */
//     "\n1", "<br />1",  /* markdown list */
//     "\n2", "<br />2",  /* markdown list */
//     "\n3", "<br />3",  /* markdown list */
//     "\n4", "<br />4",  /* markdown list */
//     "\n5", "<br />5",  /* markdown list */
//     "\n6", "<br />6",  /* markdown list */
//     "\n7", "<br />7",  /* markdown list */
//     "\n8", "<br />8",  /* markdown list */
//     "\n9", "<br />9",  /* markdown list */
//     "\n>", "<br />&gt;",  /* markdown citation */
//     "\n=", "<br />=",  /* markdown heading */
//     "\n#", "<br />#",  /* markdown heading */
//     "\n ", "<br />&nbsp;",  /* markdown list-entry continuation */
//     "\n|", "<br />|",  /* markdown table */
//     NULL,  /* end translation table */
// };

/* Note, this is no full markdown syntax support - but it helps keeping markdown in shape */
static const char MD_FILTER_LINEBREAK = '\n';
static const char MD_FILTER_LINK_AS_ID[] = "#id";
static const char MD_FILTER_LINK_AS_NAME[] = "#name";

void md_filter_init ( md_filter_t *this_,
                      data_database_reader_t *db_reader,
                      const char * tag_linebreak,
                      const char * tag_xref_start,
                      const char * tag_xref_middle,
                      const char * tag_xref_end,
                      xml_writer_t *sink )
{
    TRACE_BEGIN();
    assert( NULL != db_reader );
    assert( NULL != tag_linebreak );
    assert( NULL != tag_xref_start );
    assert( NULL != tag_xref_middle );
    assert( NULL != tag_xref_end );
    assert( NULL != sink );

    (*this_).db_reader = db_reader;
    (*this_).tag_linebreak = tag_linebreak;
    (*this_).tag_xref_start = tag_xref_start;
    (*this_).tag_xref_middle = tag_xref_middle;
    (*this_).tag_xref_end = tag_xref_end;
    (*this_).sink = sink;

    TRACE_END();
}

void md_filter_destroy( md_filter_t *this_ )
{
    TRACE_BEGIN();

    (*this_).sink = NULL;
    (*this_).db_reader = NULL;

    TRACE_END();
}

int md_filter_transform ( md_filter_t *this_, const char *text )
{
    TRACE_BEGIN();
    assert ( NULL != text );
    assert ( NULL != (*this_).db_reader );
    assert ( NULL != (*this_).sink );

    unsigned int text_start_byte = 0;
    const unsigned int text_byte_length = utf8string_get_length( text );
    int write_err = 0;

    for ( unsigned int text_current_byte = 0; text_current_byte < text_byte_length; text_current_byte ++ )
    {
        char current = text[text_current_byte];  /* note: only in case current<=0x7f this is a valid code point */
        if ( current == MD_FILTER_LINEBREAK )
        {
            write_err |= xml_writer_write_xml_enc_buf( (*this_).sink, &(text[text_start_byte]), text_current_byte-text_start_byte );
            write_err |= xml_writer_write_plain ( (*this_).sink, (*this_).tag_linebreak );
            text_start_byte = text_current_byte+1;
        }
        else if ( current == DATA_TABLE_ALPHANUM_DIAGRAM /* = 'D' */)
        {
            /* try to parse an id */
            data_id_t probe_id;
            size_t length;
            data_id_init_by_string_region ( &probe_id, text, text_current_byte, &length );
            if ( data_id_is_valid( &probe_id ) )
            {
                const bool show_id = utf8string_equals_region_str( text, text_current_byte+length, MD_FILTER_LINK_AS_ID );
                const bool show_name
                    = show_id
                    ? false
                    : utf8string_equals_region_str( text, text_current_byte+length, MD_FILTER_LINK_AS_NAME );

                if ( show_id || show_name )
                {
                    data_error_t d_err;
                    d_err = data_database_reader_get_diagram_by_id ( (*this_).db_reader, data_id_get_row_id( &probe_id ), &((*this_).temp_diagram) );
                    if ( d_err == DATA_ERROR_NONE )
                    {
                        /* write previously parsed characters */
                        write_err |= xml_writer_write_xml_enc_buf( (*this_).sink, &(text[text_start_byte]), text_current_byte-text_start_byte );
                        char probe_id_str_buf[DATA_ID_MAX_UTF8STRING_SIZE] = "";
                        utf8stringbuf_t probe_id_str = UTF8STRINGBUF( probe_id_str_buf );
                        write_err |= data_id_to_utf8stringbuf ( &probe_id, probe_id_str );
                        write_err |= xml_writer_write_plain ( (*this_).sink, (*this_).tag_xref_start );
                        write_err |= xml_writer_write_xml_enc( (*this_).sink, utf8stringbuf_get_string( probe_id_str ) );
                        write_err |= xml_writer_write_plain ( (*this_).sink, (*this_).tag_xref_middle );
                        text_start_byte = text_current_byte;
                        if ( show_id )
                        {
                            write_err |= xml_writer_write_xml_enc( (*this_).sink, utf8stringbuf_get_string( probe_id_str ) );
                            text_current_byte += length + sizeof(MD_FILTER_LINK_AS_ID)-1 - 1;
                        }
                        else /* show_name */
                        {
                            write_err |= xml_writer_write_xml_enc( (*this_).sink, data_diagram_get_name_ptr( &((*this_).temp_diagram) ) );
                            text_current_byte += length + sizeof(MD_FILTER_LINK_AS_NAME)-1 - 1;
                        }
                        write_err |= xml_writer_write_plain ( (*this_).sink, (*this_).tag_xref_end );
                        text_start_byte = text_current_byte+1;

                        /* destroy the diagram */
                        data_diagram_destroy( &((*this_).temp_diagram) );
                    }
                    else
                    {
                        TRACE_INFO_INT("id found but diagram does not exist: D", data_id_get_row_id( &probe_id ) );
                    }
                }
                else
                {
                    TRACE_INFO_INT("id found but #id or #name fragment missing: D", data_id_get_row_id( &probe_id ) );
                }
            }
        }

        if ( text_current_byte+1 == text_byte_length )
        {
            write_err |= xml_writer_write_xml_enc( (*this_).sink, &(text[text_start_byte]) );
        }
    }

    TRACE_END_ERR( write_err );
    return write_err;
}


/*
Copyright 2019-2020 Andreas Warnke

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
