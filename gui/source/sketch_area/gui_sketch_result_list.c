/* File: gui_sketch_result_list.c; Copyright and License: see below */

#include "sketch_area/gui_sketch_result_list.h"
#include "util/geometry/geometry_rectangle.h"
#include "util/string/utf8stringbuf.h"
#include "trace.h"
#include "tslog.h"
#include <gdk/gdk.h>

void gui_sketch_result_list_init( gui_sketch_result_list_t *this_ )
{
    TRACE_BEGIN();

    (*this_).visible = false;
    (*this_).result_count = 0;
    shape_int_rectangle_init( &((*this_).bounds), 0, 0, 0, 0 );

    TRACE_END();
}

void gui_sketch_result_list_destroy( gui_sketch_result_list_t *this_ )
{
    TRACE_BEGIN();

    shape_int_rectangle_destroy( &((*this_).bounds) );

    TRACE_END();
}

static const double BLACK_R = 0.0;
static const double BLACK_G = 0.0;
static const double BLACK_B = 0.0;
static const double BLACK_A = 1.0;

void gui_sketch_result_list_draw ( gui_sketch_result_list_t *this_, gui_marked_set_t *marker, cairo_t *cr )
{
    TRACE_BEGIN();
    assert( NULL != marker );
    assert( NULL != cr );

    if ( (*this_).visible )
    {
        const int32_t left = shape_int_rectangle_get_left( &((*this_).bounds) );
        const int32_t top = shape_int_rectangle_get_top( &((*this_).bounds) );
        const uint32_t width = shape_int_rectangle_get_width( &((*this_).bounds) );
        const uint32_t height = shape_int_rectangle_get_height( &((*this_).bounds) );

        char count_buf[16] = "\0";
        utf8stringbuf_t count_str = UTF8STRINGBUF( count_buf );
        utf8error_t u8err = utf8stringbuf_append_int( count_str, (*this_).result_count );
        if ( u8err != UTF8ERROR_SUCCESS )
        {
            assert(false);
            TSLOG_WARNING( "utf8stringbuf_append_int failed in gui_sketch_result_list_draw." );
        }

        cairo_set_source_rgba( cr, 0.8, 0.8, 0.8, 1.0 );
        cairo_rectangle ( cr, left, top, width, height );
        cairo_fill (cr);

        cairo_set_source_rgba( cr, BLACK_R, BLACK_G, BLACK_B, BLACK_A );
        cairo_move_to ( cr, 10, 10+14 );
        cairo_show_text ( cr, "results:" );
        cairo_move_to ( cr, 10, 10+2*14 );
        cairo_show_text ( cr, utf8stringbuf_get_string( count_str ) );
    }

    TRACE_END();
}


/*
Copyright 2018-2020 Andreas Warnke

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

