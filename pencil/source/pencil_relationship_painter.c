/* File: pencil_relationship_painter.c; Copyright and License: see below */

#include "pencil_relationship_painter.h"
#include "trace.h"
#include <pango/pangocairo.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

void pencil_relationship_painter_init( pencil_relationship_painter_t *this_ )
{
    TRACE_BEGIN();

    pencil_marker_init( &((*this_).marker) );

    TRACE_END();
}

void pencil_relationship_painter_destroy( pencil_relationship_painter_t *this_ )
{
    TRACE_BEGIN();

    pencil_marker_destroy( &((*this_).marker) );

    TRACE_END();
}

void pencil_relationship_painter_draw ( pencil_relationship_painter_t *this_,
                                        data_relationship_t *the_relationship,
                                        bool mark_focused,
                                        bool mark_highlighted,
                                        bool mark_selected,
                                        pencil_size_t *pencil_size,
                                        geometry_connector_t *connector_shape,
                                        PangoLayout *layout,
                                        cairo_t *cr )
{
    TRACE_BEGIN();
    assert( NULL != pencil_size );
    assert( NULL != the_relationship );
    assert( NULL != connector_shape );
    assert( NULL != layout );
    assert( NULL != cr );

    if ( data_relationship_is_valid( the_relationship ) )
    {
        TRACE_INFO_INT("drawing relationship id", data_relationship_get_id( the_relationship ) );

        double std_line_width = pencil_size_get_standard_line_width( pencil_size );
        cairo_set_line_width( cr, std_line_width );

        /* draw connector */
        GdkRGBA foreground_color;
        if ( mark_highlighted )
        {
            foreground_color = pencil_size_get_highlight_color( pencil_size );
        }
        else
        {
            foreground_color = pencil_size_get_standard_color( pencil_size );
        }
        cairo_set_source_rgba( cr, foreground_color.red, foreground_color.green, foreground_color.blue, foreground_color.alpha );
        double p1x = geometry_connector_get_source_end_x ( connector_shape );
        double p1y = geometry_connector_get_source_end_y ( connector_shape );
        double p2x = geometry_connector_get_main_line_source_x ( connector_shape );
        double p2y = geometry_connector_get_main_line_source_y ( connector_shape );
        double p3x = geometry_connector_get_main_line_destination_x ( connector_shape );
        double p3y = geometry_connector_get_main_line_destination_y ( connector_shape );
        double p4x = geometry_connector_destination_end_x ( connector_shape );
        double p4y = geometry_connector_get_destination_end_y ( connector_shape );
        cairo_move_to ( cr, p1x, p1y );
        cairo_line_to ( cr, p2x, p2y );
        cairo_line_to ( cr, p3x, p3y );
        cairo_line_to ( cr, p4x, p4y );
        cairo_stroke (cr);
    }
    else
    {
        TSLOG_ERROR("invalid relationship in array!");
    }

    TRACE_END();
}


/*
Copyright 2017-2017 Andreas Warnke

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
