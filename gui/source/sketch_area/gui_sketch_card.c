/* File: gui_sketch_card.c; Copyright and License: see below */

#include "sketch_area/gui_sketch_card.h"
#include "pencil_diagram_maker.h"
#include "util/geometry/geometry_rectangle.h"
#include "trace.h"
#include "tslog.h"
#include <gdk/gdk.h>

void gui_sketch_card_init( gui_sketch_card_t *this_ )
{
    TRACE_BEGIN();

    (*this_).visible = false;
    (*this_).dirty_elements_layout = false;
    shape_int_rectangle_init( &((*this_).bounds), 0, 0, 0, 0 );
    data_visible_set_init( &((*this_).painter_input_data) );
    pencil_diagram_maker_init( &((*this_).painter), &((*this_).painter_input_data) );

    TRACE_END();
}

void gui_sketch_card_destroy( gui_sketch_card_t *this_ )
{
    TRACE_BEGIN();

    pencil_diagram_maker_destroy( &((*this_).painter) );
    data_visible_set_destroy( &((*this_).painter_input_data) );
    shape_int_rectangle_destroy(&((*this_).bounds));

    TRACE_END();
}

void gui_sketch_card_draw ( gui_sketch_card_t *this_, gui_marked_set_t *marker, cairo_t *cr )
{
    TRACE_BEGIN();
    assert( NULL != cr );

    if ( (*this_).visible )
    {
        int32_t left;
        int32_t top;
        uint32_t width;
        uint32_t height;

        left = shape_int_rectangle_get_left( &((*this_).bounds) );
        top = shape_int_rectangle_get_top( &((*this_).bounds) );
        width = shape_int_rectangle_get_width( &((*this_).bounds) );
        height = shape_int_rectangle_get_height( &((*this_).bounds) );

        /* layout elements if necessary */
        if ( (*this_).dirty_elements_layout )
        {
            pencil_diagram_maker_layout_elements ( &((*this_).painter), cr );
            (*this_).dirty_elements_layout = false;
        }

        /* draw paper */
        cairo_set_source_rgba( cr, 1.0, 1.0, 1.0, 1.0 );
        cairo_rectangle ( cr, left, top, width, height );
        cairo_fill (cr);

        /* draw the current diagram */
        data_id_t mark_focused = gui_marked_set_get_focused( marker );
        data_id_t mark_highlighted = gui_marked_set_get_highlighted( marker );
        data_small_set_t *mark_selected_set = gui_marked_set_get_selected_set_ptr( marker );
        pencil_diagram_maker_draw ( &((*this_).painter),
                                    mark_focused,
                                    mark_highlighted,
                                    mark_selected_set,
                                    cr
                                  );
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
