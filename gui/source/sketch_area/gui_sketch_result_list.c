/* File: gui_sketch_result_list.c; Copyright and License: see below */

#include "sketch_area/gui_sketch_result_list.h"
#include "util/geometry/geometry_rectangle.h"
#include "util/string/utf8stringbuf.h"
#include "trace.h"
#include "tslog.h"
#include <gdk/gdk.h>

const int GUI_SKETCH_RESULT_LIST_LINE_HEIGHT = 28;  /*!< height of an element-name/entry in pixels */
static const int OBJ_GAP = 4;

void gui_sketch_result_list_init( gui_sketch_result_list_t *this_, gui_resources_t *resources )
{
    TRACE_BEGIN();
    assert( resources != NULL );

    DATA_SEARCH_RESULT_LIST_INIT( &((*this_).result_list), (*this_).result_list_buf );

    (*this_).visible = false;
    shape_int_rectangle_init( &((*this_).bounds), 0, 0, 0, 0 );

    (*this_).element_count = 0;

    gui_sketch_style_init( &((*this_).sketch_style) );
    gui_sketch_marker_init( &((*this_).sketch_marker), true );
    (*this_).resources = resources;
    gui_resource_selector_init( &((*this_).selector), resources );

    TRACE_END();
}

void gui_sketch_result_list_destroy( gui_sketch_result_list_t *this_ )
{
    TRACE_BEGIN();

    gui_resource_selector_destroy( &((*this_).selector) );
    (*this_).resources = NULL;
    gui_sketch_marker_destroy( &((*this_).sketch_marker) );
    gui_sketch_style_destroy( &((*this_).sketch_style) );

    data_search_result_list_destroy( &((*this_).result_list) );

    shape_int_rectangle_destroy( &((*this_).bounds) );

    TRACE_END();
}

void gui_sketch_result_list_do_layout( gui_sketch_result_list_t *this_, cairo_t *cr )
{
    TRACE_BEGIN();

    const unsigned int count = data_search_result_list_get_length( &((*this_).result_list) );
    assert( count <= GUI_SKETCH_RESULT_LIST_MAX_ELEMENTS );
    (*this_).element_count = count;
    for ( unsigned int idx = 0; idx < count; idx ++ )
    {
        const data_search_result_t *result = data_search_result_list_get_const( &((*this_).result_list), idx );
        pos_search_result_init( &((*this_).element_pos[idx]), result );
    }

    TRACE_END();
}

static const double GREY_R = 0.8;
static const double GREY_G = 0.8;
static const double GREY_B = 0.8;
static const double GREY_A = 1.0;

void gui_sketch_result_list_draw ( gui_sketch_result_list_t *this_, gui_marked_set_t *marker, cairo_t *cr )
{
    TRACE_BEGIN();
    assert( NULL != marker );
    assert( NULL != cr );

    if ( (*this_).visible )
    {
        PangoLayout *layout = pango_cairo_create_layout (cr);
        const PangoFontDescription *const std_font
            = gui_sketch_style_get_standard_font_description( &((*this_).sketch_style ) );
        pango_layout_set_font_description ( layout, std_font );

        const int32_t left = shape_int_rectangle_get_left( &((*this_).bounds) );
        const int32_t top = shape_int_rectangle_get_top( &((*this_).bounds) );
        const uint32_t width = shape_int_rectangle_get_width( &((*this_).bounds) );
        const uint32_t height = shape_int_rectangle_get_height( &((*this_).bounds) );

        /* draw background */
        cairo_set_source_rgba( cr, GREY_R, GREY_G, GREY_B, GREY_A );
        cairo_rectangle ( cr, left, top, width, height );
        cairo_fill (cr);

        /* draw icons and text */
        const unsigned int count = data_search_result_list_get_length( &((*this_).result_list) );
        if ( count == 0 )
        {
            const GdkPixbuf *undef_icon = gui_resources_get_type_undef( (*this_).resources );
            const GdkRGBA std_color = gui_sketch_style_get_standard_color( &((*this_).sketch_style) );
            cairo_set_source_rgba( cr, std_color.red, std_color.green, std_color.blue, std_color.alpha );
            gui_sketch_result_list_private_draw_icon_and_label( this_, undef_icon, "n/a", 8, 8, layout, cr );
        }
        else
        {
            const data_id_t highlighted = gui_marked_set_get_highlighted( marker );

            for ( unsigned int idx = 0; idx < count; idx ++ )
            {
                const data_search_result_t *result = data_search_result_list_get_const( &((*this_).result_list), idx );
                const int type = data_search_result_get_match_type( result );
                const data_table_t table = data_id_get_table( data_search_result_get_match_id_const( result ) );

                shape_int_rectangle_t destination_rect;
                shape_int_rectangle_init( &destination_rect,
                                          left+OBJ_GAP,
                                          top+OBJ_GAP+GUI_SKETCH_RESULT_LIST_LINE_HEIGHT*idx,
                                          width-OBJ_GAP,
                                          GUI_SKETCH_RESULT_LIST_LINE_HEIGHT-2*OBJ_GAP
                                        );

                gui_sketch_marker_prepare_draw( &((*this_).sketch_marker),
                                                data_search_result_get_match_id( result ),
                                                marker,
                                                destination_rect,
                                                cr
                                              );
               if ( data_id_equals( &highlighted, data_search_result_get_diagram_id_const( result ) ) )
                {
                    const GdkRGBA high_color = gui_sketch_style_get_highlight_color( &((*this_).sketch_style) );
                    cairo_set_source_rgba( cr, high_color.red, high_color.green, high_color.blue, high_color.alpha );
                }

                const GdkPixbuf *icon = gui_resource_selector_get_icon ( &((*this_).selector), table, type );
                gui_sketch_result_list_private_draw_icon_and_label( this_,
                                                                    icon,
                                                                    data_search_result_get_match_name_const( result ),
                                                                    left+OBJ_GAP,
                                                                    top+OBJ_GAP+GUI_SKETCH_RESULT_LIST_LINE_HEIGHT*idx,
                                                                    layout,
                                                                    cr
                                                                  );

                shape_int_rectangle_destroy( &destination_rect );
            }
        }

        g_object_unref(layout);
    }

    TRACE_END();
}

void gui_sketch_result_list_private_draw_icon_and_label( gui_sketch_result_list_t *this_,
                                                         const GdkPixbuf *icon_1,
                                                         const char *label_1,
                                                         int x,
                                                         int y,
                                                         PangoLayout *layout,
                                                         cairo_t *cr )
{
    TRACE_BEGIN();
    assert( NULL != cr );
    assert( NULL != layout );
    assert( NULL != icon_1 );
    assert( NULL != label_1 );

    /* determine coodinates */
    double icon_width = gdk_pixbuf_get_width ( icon_1 );
    double icon_height = gdk_pixbuf_get_height ( icon_1 );

    /* draw text first, use the pre-set color and font */
    cairo_move_to ( cr, x+icon_width+OBJ_GAP, y );
    pango_layout_set_text ( layout, label_1, -1 );
    pango_cairo_show_layout ( cr, layout );

    /* draw the icon */
    gdk_cairo_set_source_pixbuf( cr, icon_1, x, y );
    cairo_rectangle ( cr, x, y, x+icon_width, y+icon_height );
    cairo_fill (cr);

    TRACE_END();
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

