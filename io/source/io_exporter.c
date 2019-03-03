/* File: io_exporter.c; Copyright and License: see below */

#include "io_exporter.h"
#include "trace.h"
#include "tslog.h"
#include <gtk/gtk.h>
#include <cairo-svg.h>
#include <cairo-pdf.h>
#include <cairo-ps.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

void io_exporter_init ( io_exporter_t *this_,
                        data_database_reader_t *db_reader )
{
    TRACE_BEGIN();
    assert( NULL != db_reader );

    (*this_).db_reader = db_reader;
    (*this_).temp_filename = utf8stringbuf_init( sizeof((*this_).temp_filename_buf), (*this_).temp_filename_buf );

    pencil_input_data_init( &((*this_).painter_input_data) );
    pencil_description_writer_init( &((*this_).description_writer), &((*this_).painter_input_data) );
    io_diagram_image_exporter_init( &((*this_).diagram_image_exporter ), &((*this_).painter_input_data) );
    io_document_exporter_init( &((*this_).document_exporter ), &((*this_).painter_input_data) );

    TRACE_END();
}

void io_exporter_destroy( io_exporter_t *this_ )
{
    TRACE_BEGIN();

    io_document_exporter_destroy( &((*this_).document_exporter ) );
    io_diagram_image_exporter_destroy( &((*this_).diagram_image_exporter ) );
    pencil_description_writer_destroy( &((*this_).description_writer) );
    pencil_input_data_destroy( &((*this_).painter_input_data) );

    (*this_).db_reader = NULL;

    TRACE_END();
}

int io_exporter_export_files( io_exporter_t *this_,
                              io_file_format_t export_type,
                              const char* target_folder )
{
    TRACE_BEGIN();
    int export_err = 0;

    if ( NULL != target_folder )
    {

        if ( ( export_type & IO_FILE_FORMAT_SVG ) != 0 )
        {
            export_err |= io_exporter_private_export_image_files( this_, DATA_ID_VOID_ID, 16, IO_FILE_FORMAT_SVG, target_folder );
        }

        if ( ( export_type & ( IO_FILE_FORMAT_PDF | IO_FILE_FORMAT_DOCBOOK ) ) != 0 )
        {
            export_err |= io_exporter_private_export_image_files( this_, DATA_ID_VOID_ID, 16, IO_FILE_FORMAT_PDF, target_folder );
        }

        if ( ( export_type & IO_FILE_FORMAT_PS ) != 0 )
        {
            export_err |= io_exporter_private_export_image_files( this_, DATA_ID_VOID_ID, 16, IO_FILE_FORMAT_PS, target_folder );
        }

        if ( ( export_type & ( IO_FILE_FORMAT_PNG | IO_FILE_FORMAT_DOCBOOK | IO_FILE_FORMAT_XHTML ) ) != 0 )
        {
            export_err |= io_exporter_private_export_image_files( this_, DATA_ID_VOID_ID, 16, IO_FILE_FORMAT_PNG, target_folder );
        }

        if ( ( export_type & IO_FILE_FORMAT_TXT ) != 0 )
        {
            export_err |= io_exporter_private_export_image_files( this_, DATA_ID_VOID_ID, 16, IO_FILE_FORMAT_TXT, target_folder );
        }

        if ( ( export_type & IO_FILE_FORMAT_DOCBOOK ) != 0 )
        {
            export_err |= io_exporter_private_export_document_file( this_, IO_FILE_FORMAT_DOCBOOK, target_folder );
        }

        if ( ( export_type & IO_FILE_FORMAT_XHTML ) != 0 )
        {
            export_err |= io_exporter_private_export_document_file( this_, IO_FILE_FORMAT_XHTML, target_folder );
        }
    }
    else /* target_folder == NULL */
    {
        TSLOG_WARNING("selected target folder was NULL.");
        export_err = -1;
    }

    TRACE_END_ERR(export_err);
    return export_err;
}

int io_exporter_private_export_image_files( io_exporter_t *this_,
                                            int64_t diagram_id,
                                            uint32_t max_recursion,
                                            io_file_format_t export_type,
                                            const char* target_folder )
{
    TRACE_BEGIN();
    assert ( NULL != target_folder );
    int result = 0;

    /* draw current diagram */
    if ( DATA_ID_VOID_ID != diagram_id )
    {
        /* load data to be drawn */
        pencil_input_data_load( &((*this_).painter_input_data), diagram_id, (*this_).db_reader );
        data_diagram_t *diag_ptr;
        diag_ptr = pencil_input_data_get_diagram_ptr ( &((*this_).painter_input_data) );
        const char *diag_name;
        diag_name = data_diagram_get_name_ptr( diag_ptr );

        /* determine filename */
        utf8stringbuf_copy_str( (*this_).temp_filename, target_folder );
        utf8stringbuf_append_str( (*this_).temp_filename, "/" );
        {
            data_id_t the_id;
            data_id_init( &the_id, DATA_TABLE_DIAGRAM, diagram_id );
            data_id_to_utf8stringbuf( &the_id, (*this_).temp_filename );
        }
        utf8stringbuf_append_str( (*this_).temp_filename, "_" );
        io_exporter_private_append_valid_chars_to_filename( this_, diag_name, (*this_).temp_filename );
        if ( IO_FILE_FORMAT_SVG == export_type )
        {
            utf8stringbuf_append_str( (*this_).temp_filename, ".svg" );
        }
        else if ( IO_FILE_FORMAT_PNG == export_type )
        {
            utf8stringbuf_append_str( (*this_).temp_filename, ".png" );
        }
        else if ( IO_FILE_FORMAT_PDF == export_type )
        {
            utf8stringbuf_append_str( (*this_).temp_filename, ".pdf" );
        }
        else if ( IO_FILE_FORMAT_PS == export_type )
        {
            utf8stringbuf_append_str( (*this_).temp_filename, ".ps" );
        }
        else /* IO_FILE_FORMAT_TXT */
        {
            utf8stringbuf_append_str( (*this_).temp_filename, ".txt" );
        }
        TSLOG_EVENT_STR( "exporting diagram to file:", utf8stringbuf_get_string( (*this_).temp_filename ) );

        /* create surface */
        if (( IO_FILE_FORMAT_SVG == export_type )
            || ( IO_FILE_FORMAT_PDF == export_type )
            || ( IO_FILE_FORMAT_PS == export_type )
            || ( IO_FILE_FORMAT_PNG == export_type ) )
        {
            result = io_diagram_image_exporter_render_surface_to_file( &((*this_).diagram_image_exporter ),
                                                                       export_type,
                                                                       utf8stringbuf_get_string( (*this_).temp_filename )
                                                                     );
        }
        else /* IO_FILE_FORMAT_TXT */
        {
            FILE *text_output;

            /* open file */
            text_output = fopen( utf8stringbuf_get_string( (*this_).temp_filename ), "w" );
            if ( NULL == text_output )
            {
                TSLOG_ERROR("error creating txt.");
                result = -1;
            }
            else
            {
                /* write file */
                int write_err;
                write_err = pencil_description_writer_draw ( &((*this_).description_writer), text_output );
                if ( 0 != write_err )
                {
                    TSLOG_ERROR("error writing txt.");
                    result = -1;
                }

                /* close file */
                int close_err;
                close_err = fclose( text_output );
                if ( 0 != close_err )
                {
                    TSLOG_ERROR("error finishing txt.");
                    result = -1;
                }
            }
        }
    }

    /* recursion to children */
    if (( result == 0 )&&( max_recursion > 0 ))
    {
        data_error_t db_err;
        data_small_set_t the_set;
        data_small_set_init( &the_set );
        db_err = data_database_reader_get_diagram_ids_by_parent_id ( (*this_).db_reader, diagram_id, &the_set );
        if ( db_err != DATA_ERROR_NONE )
        {
            TSLOG_ERROR("error reading database.");
            result = -1;
        }
        else
        {
            for ( uint32_t pos = 0; pos < data_small_set_get_count( &the_set ); pos ++ )
            {
                data_id_t probe;
                probe = data_small_set_get_id( &the_set, pos );
                int64_t probe_row_id;
                probe_row_id = data_id_get_row_id( &probe );

                result |= io_exporter_private_export_image_files( this_, probe_row_id, max_recursion-1, export_type, target_folder );

                data_id_destroy( &probe );
            }
        }
        data_small_set_destroy( &the_set );
    }

    TRACE_END_ERR( result );
    return result;
}

int io_exporter_private_export_document_file( io_exporter_t *this_,
                                              io_file_format_t export_type,
                                              const char* target_folder )
{
    TRACE_BEGIN();
    assert ( NULL != target_folder );
    int export_err = 0;
    FILE *output;

    /* open file */
    utf8stringbuf_copy_str( (*this_).temp_filename, target_folder );
    switch ( export_type )
    {
        case IO_FILE_FORMAT_DOCBOOK:
        {
            utf8stringbuf_append_str( (*this_).temp_filename, "/document.db.xml" );
        }
        break;

        case IO_FILE_FORMAT_XHTML:
        {
            utf8stringbuf_append_str( (*this_).temp_filename, "/document.xhtml" );
        }
        break;

        default:
        {
            utf8stringbuf_append_str( (*this_).temp_filename, "/unknown_format" );
            TSLOG_ERROR("error: unknown_format.");
        }
        break;
    }
    TSLOG_EVENT_STR( "exporting diagrams to document file:", utf8stringbuf_get_string( (*this_).temp_filename ) );
    output = fopen( utf8stringbuf_get_string( (*this_).temp_filename ), "w" );
    if ( NULL == output )
    {
        TSLOG_ERROR("error creating document.");
        export_err |= -1;
    }
    else
    {
        int write_err;
        
        /* write file */
        write_err = io_document_exporter_write_header( &((*this_).document_exporter), export_type, "Document Title", output );
        write_err |= io_exporter_private_export_document_part( this_, DATA_ID_VOID_ID, 16, export_type, output );
        write_err |= io_document_exporter_write_footer( &((*this_).document_exporter), export_type, output );

        if ( 0 != write_err )
        {
            TSLOG_ERROR("error writing document.");
            export_err |= -1;
        }
        
        /* close file */
        int close_err;
        close_err = fclose( output );
        if ( 0 != close_err )
        {
            TSLOG_ERROR("error finishing document.");
            export_err |= -1;
        }
    }

    TRACE_END_ERR( export_err );
    return export_err;
    
}

int io_exporter_private_export_document_part( io_exporter_t *this_,
                                              int64_t diagram_id,
                                              uint32_t max_recursion,
                                              io_file_format_t export_type,
                                              FILE *output )
{
    TRACE_BEGIN();
    assert ( NULL != output );
    int export_err = 0;

    /* write part for current diagram */
    if ( DATA_ID_VOID_ID != diagram_id )
    {
        /* load data to be drawn */
        pencil_input_data_load( &((*this_).painter_input_data), diagram_id, (*this_).db_reader );
        data_diagram_t *diag_ptr;
        diag_ptr = pencil_input_data_get_diagram_ptr ( &((*this_).painter_input_data) );
        const char *diag_name;
        diag_name = data_diagram_get_name_ptr( diag_ptr );

        /* determine filename */
        utf8stringbuf_clear( (*this_).temp_filename );
        {
            data_id_t the_id;
            data_id_init( &the_id, DATA_TABLE_DIAGRAM, diagram_id );
            data_id_to_utf8stringbuf( &the_id, (*this_).temp_filename );
        }
        utf8stringbuf_append_str( (*this_).temp_filename, "_" );
        io_exporter_private_append_valid_chars_to_filename( this_, diag_name, (*this_).temp_filename );
        
        /* write doc part */
        export_err |= io_document_exporter_start_diagram( &((*this_).document_exporter),
                                                          export_type,
                                                          output
                                                        );
        export_err |= io_document_exporter_write_diagram( &((*this_).document_exporter),
                                                          export_type,
                                                          utf8stringbuf_get_string( (*this_).temp_filename ),
                                                          output
                                                        );
    }

    /* recursion to children */
    if (( export_err == 0 )&&( max_recursion > 0 ))
    {
        data_error_t db_err;
        data_small_set_t the_set;
        data_small_set_init( &the_set );
        db_err = data_database_reader_get_diagram_ids_by_parent_id ( (*this_).db_reader, diagram_id, &the_set );
        if ( db_err != DATA_ERROR_NONE )
        {
            TSLOG_ERROR("error reading database.");
            export_err |= -1;
        }
        else
        {
            for ( uint32_t pos = 0; pos < data_small_set_get_count( &the_set ); pos ++ )
            {
                data_id_t probe;
                probe = data_small_set_get_id( &the_set, pos );
                int64_t probe_row_id;
                probe_row_id = data_id_get_row_id( &probe );

                export_err |= io_exporter_private_export_document_part( this_, probe_row_id, max_recursion-1, export_type, output );

                data_id_destroy( &probe );
            }
        }
        data_small_set_destroy( &the_set );
    }

    /* end diagram section */
    if ( DATA_ID_VOID_ID != diagram_id )
    {
        /* write doc part */
        export_err |= io_document_exporter_end_diagram( &((*this_).document_exporter),
                                                        export_type,
                                                        output
                                                      );
    }

    TRACE_END_ERR( export_err );
    return export_err;
}

void io_exporter_private_append_valid_chars_to_filename( io_exporter_t *this_,
                                                         const char* name,
                                                         utf8stringbuf_t filename )
{
    TRACE_BEGIN();
    assert( NULL != name );
    TRACE_INFO_STR( "name:", name );

    bool finished = false;
    static const int MAX_APPEND_CHARS = 64;
    for ( int pos = 0; ( pos < MAX_APPEND_CHARS ) && ( ! finished ); pos ++ )
    {
        char probe = name[pos];
        if ( probe == '\0' )
        {
            finished = true;
        }
        else if (( 'A' <= probe ) && ( probe <= 'Z' ))
        {
            utf8stringbuf_append_char( filename, probe );
        }
        else if (( 'a' <= probe ) && ( probe <= 'z' ))
        {
            utf8stringbuf_append_char( filename, probe );
        }
        else if (( '0' <= probe ) && ( probe <= '9' ))
        {
            utf8stringbuf_append_char( filename, probe );
        }
        else if ( '-' == probe )
        {
            utf8stringbuf_append_char( filename, probe );
        }
        else if ( '_' == probe )
        {
            utf8stringbuf_append_char( filename, probe );
        }
        else if ( ' ' == probe )
        {
            utf8stringbuf_append_char( filename, '_' );
        }
    }

    TRACE_END();
}


/*
Copyright 2016-2019 Andreas Warnke

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