/* File: pencil_diagram_painter.c; Copyright and License: see below */

#include "pencil_diagram_painter.h"
#include "pencil_size.h"
#include "trace.h"
#include <stdio.h>
#include <stdlib.h>

void pencil_diagram_painter_init( pencil_diagram_painter_t *this_ )
{
    TRACE_BEGIN();

    pencil_classifier_painter_init( &((*this_).classifier_painter) );
    pencil_private_marker_init( &((*this_).marker) );

    TRACE_END();
}

void pencil_diagram_painter_destroy( pencil_diagram_painter_t *this_ )
{
    TRACE_BEGIN();

    pencil_classifier_painter_destroy( &((*this_).classifier_painter) );
    pencil_private_marker_destroy( &((*this_).marker) );

    TRACE_END();
}

void pencil_diagram_painter_draw ( pencil_diagram_painter_t *this_,
                                   pencil_input_data_t *input_data,
                                   data_id_t mark_focused,
                                   data_id_t mark_highlighted,
                                   data_small_set_t *mark_selected,
                                   cairo_t *cr,
                                   geometry_rectangle_t destination )
{
    TRACE_BEGIN();

    pencil_size_t pencil_size_object;
    pencil_size_t *pencil_size = &pencil_size_object;

    double left = geometry_rectangle_get_left ( &destination );
    double top = geometry_rectangle_get_top ( &destination );
    double right = geometry_rectangle_get_right ( &destination );
    double bottom = geometry_rectangle_get_bottom ( &destination );
    double width = geometry_rectangle_get_width ( &destination );
    double height = geometry_rectangle_get_height ( &destination );
    pencil_size_init( pencil_size, width, height );

    TRACE_INFO_INT( "w", (int)(width) );
    TRACE_INFO_INT( "h", (int)(height) );

    double gap = pencil_size_get_standard_object_border( pencil_size );
    double f_size = pencil_size_get_standard_font_size( pencil_size );
    double f_ascent = pencil_size_get_standard_font_ascent( pencil_size );
    double f_line_gap = pencil_size_get_font_line_gap( pencil_size );

    /* draw diagram border and name */
    {
        data_diagram_t *diag;
        diag = pencil_input_data_get_diagram_ptr( input_data );
        TRACE_INFO_INT("drawing diagram id",data_diagram_get_id(diag));

        double std_line_width = pencil_size_get_standard_line_width( pencil_size );
        cairo_set_line_width( cr, std_line_width );
        if ( data_diagram_is_valid(diag) )
        {
            /* draw border line */
            if ( data_id_equals_id( &mark_highlighted, DATA_TABLE_DIAGRAM, data_diagram_get_id(diag) ))
            {
                GdkRGBA highlight_color;
                highlight_color = pencil_size_get_highlight_color( pencil_size );
                cairo_set_source_rgba( cr, highlight_color.red, highlight_color.green, highlight_color.blue, highlight_color.alpha );
            }
            else
            {
                GdkRGBA standard_color;
                standard_color = pencil_size_get_standard_color( pencil_size );
                cairo_set_source_rgba( cr, standard_color.red, standard_color.green, standard_color.blue, standard_color.alpha );
            }
            cairo_rectangle ( cr, left+gap, top+gap, width-2.0*gap, height-2.0*gap );
            cairo_stroke (cr);

            /* draw title corner */
            cairo_move_to ( cr, left+gap, top+gap+f_size+f_line_gap );
            cairo_line_to ( cr, left+(width/3.0), top+gap+f_size+f_line_gap );
            cairo_line_to ( cr, left+(width/3.0)+4.0, top+gap+f_size+f_line_gap-4.0 );
            cairo_line_to ( cr, left+(width/3.0)+4.0, top+gap );
            cairo_stroke (cr);

            cairo_set_font_size ( cr, f_size );
            cairo_move_to ( cr, left+2.0*gap, top+gap+f_ascent+f_line_gap );
            cairo_show_text ( cr, data_diagram_get_name_ptr( diag ) );
        }
        else
        {
            /* draw cross line */
            GdkRGBA standard_color;
            standard_color = pencil_size_get_standard_color( pencil_size );
            cairo_set_source_rgba( cr, standard_color.red, standard_color.green, standard_color.blue, standard_color.alpha );
            cairo_move_to ( cr, left, top );
            cairo_line_to ( cr, right, bottom );
            cairo_move_to ( cr, left, bottom );
            cairo_line_to ( cr, right, top );
            cairo_stroke (cr);
        }
    }

    double tenth_width = width / 10.0;

    /* draw all contained classifiers */
    if (( width > 10.0 ) && ( height > 25.0 ))
    {
        geometry_rectangle_t classifier_bounds;
        geometry_rectangle_init ( &classifier_bounds, left + 2.0 * gap + tenth_width, top + 2.0 * gap + f_size + f_line_gap, width - 4.0 * gap - 2.0 * tenth_width, height - 4.0 * gap - f_size - f_line_gap );
        pencil_classifier_painter_draw ( &((*this_).classifier_painter),
                                         input_data,
                                         mark_focused,
                                         mark_highlighted,
                                         mark_selected,
                                         pencil_size,
                                         cr,
                                         classifier_bounds
                                       );
        geometry_rectangle_destroy( &classifier_bounds );
    }

    /* mark focused and highlighted */
    {
        data_diagram_t *diag2;
        diag2 = pencil_input_data_get_diagram_ptr( input_data );
        if ( data_id_equals_id( &mark_focused, DATA_TABLE_DIAGRAM, data_diagram_get_id(diag2) ))
        {
            pencil_private_marker_mark_focused_rectangle( &((*this_).marker), destination, cr );
        }

        if ( data_small_set_contains_row_id( mark_selected, DATA_TABLE_DIAGRAM, data_diagram_get_id(diag2) ))
        {
            geometry_rectangle_t selected_rect;
            geometry_rectangle_init( &selected_rect, left+gap, top+gap, width-2.0*gap, height-2.0*gap );
            pencil_private_marker_mark_selected_rectangle( &((*this_).marker), selected_rect, cr );
            geometry_rectangle_destroy( &selected_rect );
        }
    }

    pencil_size_destroy( pencil_size );
    TRACE_END();
}

data_id_t pencil_diagram_painter_get_object_id_at_pos ( pencil_diagram_painter_t *this_,
                                                        pencil_input_data_t *input_data,
                                                        double x,
                                                        double y,
                                                        geometry_rectangle_t diagram_bounds,
                                                        bool dereference )
{
    TRACE_BEGIN();

    data_id_t result;
    data_diagram_t *diag;
    diag = pencil_input_data_get_diagram_ptr( input_data );
    pencil_size_t pencil_size_object;
    pencil_size_t *pencil_size = &pencil_size_object;
    pencil_size_init( pencil_size, geometry_rectangle_get_width( &diagram_bounds ), geometry_rectangle_get_height( &diagram_bounds ) );

    double gap = pencil_size_get_standard_object_border( pencil_size );
    double f_size = pencil_size_get_standard_font_size( pencil_size );
    double f_line_gap = pencil_size_get_font_line_gap( pencil_size );
    double width = geometry_rectangle_get_width ( &diagram_bounds );
    double tenth_width = width / 10.0;

    if ( geometry_rectangle_contains( &diagram_bounds, x, y ) && data_diagram_is_valid(diag) )
    {
        double left = geometry_rectangle_get_left ( &diagram_bounds );
        double top = geometry_rectangle_get_top ( &diagram_bounds );
        double width = geometry_rectangle_get_width ( &diagram_bounds );
        double height = geometry_rectangle_get_height ( &diagram_bounds );

        geometry_rectangle_t classifier_bounds;
        geometry_rectangle_init( &classifier_bounds,
                                 left + 2.0 * gap + tenth_width,
                                 top + 2.0 * gap + f_size + f_line_gap,
                                 width - 4.0 * gap - 2.0 * tenth_width,
                                 height - 4.0 * gap - f_size - f_line_gap
                               );

        result = pencil_classifier_painter_get_object_id_at_pos( &((*this_).classifier_painter), input_data, x, y, classifier_bounds, dereference );

        geometry_rectangle_destroy( &classifier_bounds );

        if ( ! data_id_is_valid( &result ) )
        {
            data_id_destroy( &result );
            data_diagram_t *pencil_input_data_get_diagram_ptr ( pencil_input_data_t *this_ );
            data_id_init( &result, DATA_TABLE_DIAGRAM, data_diagram_get_id(diag) );
        }
    }
    else
    {
        TRACE_INFO( "no object at given location or no diagram chosen" );
        data_id_init_void( &result );
    }

    TRACE_END();
    return result;
}


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
