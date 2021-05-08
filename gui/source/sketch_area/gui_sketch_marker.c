/* File: gui_sketch_marker.c; Copyright and License: see below */

#include "sketch_area/gui_sketch_marker.h"
#include "trace.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

static const double BLACK_R = 0.0;
static const double BLACK_G = 0.0;
static const double BLACK_B = 0.0;
static const double BLACK_A = 1.0;
static const double WHITE_R = 1.0;
static const double WHITE_G = 1.0;
static const double WHITE_B = 1.0;
static const double WHITE_A = 1.0;

static const double LINE_W = 1.0;

void gui_sketch_marker_prepare_draw ( const gui_sketch_marker_t *this_,
                                      data_table_t table,
                                      data_row_id_t row_id,
                                      gui_marked_set_t *marked_set,
                                      shape_int_rectangle_t rect,
                                      cairo_t *cr )
{
    const data_id_t focused_id = gui_marked_set_get_focused ( marked_set );
    const data_row_id_t focused_diagram_id = gui_marked_set_get_focused_diagram ( marked_set );
    const data_id_t highlighted_id = gui_marked_set_get_highlighted ( marked_set );
    const data_small_set_t *const selected_set = gui_marked_set_get_selected_set_ptr ( marked_set );

    if ( DATA_TABLE_DIAGRAM == table )
    {
        const int32_t left = shape_int_rectangle_get_left( &rect );
        const int32_t top = shape_int_rectangle_get_top( &rect );
        const uint32_t width = shape_int_rectangle_get_width( &rect );
        const uint32_t height = shape_int_rectangle_get_height( &rect );
        
        /* mark focused diagram */
        if ( row_id == focused_diagram_id )
        {
            cairo_set_source_rgba( cr, BLACK_R, BLACK_G, BLACK_B, BLACK_A );
            cairo_rectangle ( cr, left, top, width, height );
            cairo_stroke (cr);
        }

        /* set color to mark highlighted */
        if ( row_id == data_id_get_row_id( &highlighted_id ) )
        {
            /* draw a white border around the target rectangle */
            {
                cairo_set_source_rgba( cr, WHITE_R, WHITE_G, WHITE_B, WHITE_A );
                cairo_rectangle ( cr, left-2, top-1, LINE_W, height+2 );
                cairo_rectangle ( cr, left-2, top-2, width+4, LINE_W );
                cairo_rectangle ( cr, left+width+1, top-1, LINE_W, height+2 );
                cairo_rectangle ( cr, left-2, top+height+1, width+4, LINE_W );
                cairo_fill (cr);
            }
            /* draw a light border around the target rectangle */
            /*
            {
                cairo_set_source_rgba( cr, 0.0, 0.8, 0.6, 1.0 );
                cairo_rectangle ( cr, left-3, top-2, LINE_W, height+4 );
                cairo_rectangle ( cr, left-2, top-3, width+6, LINE_W );
                cairo_rectangle ( cr, left+width+2, top-2, LINE_W, height+4 );
                cairo_rectangle ( cr, left-3, top+height+2, width+6, LINE_W );
                cairo_fill (cr);
            }
            */
        }
        
        /* mark focused */
        if (( row_id == data_id_get_row_id( &focused_id ) )&& (*this_).with_pencil_markers )
        {
            geometry_rectangle_t pencil_rect;
            geometry_rectangle_init( &pencil_rect, left, top, width, height );
            pencil_marker_mark_focused_rectangle( &((*this_).pencil_marker), pencil_rect, cr );
        }

        /* mark selected */
        if (( data_small_set_contains_row_id( selected_set, table, row_id ) )&& (*this_).with_pencil_markers )
        {
            geometry_rectangle_t pencil_rect;
            geometry_rectangle_init( &pencil_rect, left, top, width, height );
            pencil_marker_mark_selected_rectangle( &((*this_).pencil_marker), pencil_rect, cr );
        }

        /* set color to mark highlighted */
        if ( row_id == data_id_get_row_id( &highlighted_id ) )
        {
            /* use same color as in pencil_size.inl */
            cairo_set_source_rgba( cr, 0.0, 0.6, 0.4, 1.0 );
        }
        else
        {
            cairo_set_source_rgba( cr, BLACK_R, BLACK_G, BLACK_B, BLACK_A );
        }
    }
    else
    {
        TSLOG_WARNING ( "marking objects other than DATA_TABLE_DIAGRAM is not yet implemented" );
    }
}


/*
Copyright 2018-2021 Andreas Warnke

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
