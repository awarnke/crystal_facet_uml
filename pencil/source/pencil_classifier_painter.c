/* File: pencil_classifier_painter.c; Copyright and License: see below */

#include "pencil_classifier_painter.h"
#include "trace.h"
#include "util/string/utf8stringbuf.h"
#include "util/string/utf8string.h"
#include <pango/pangocairo.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

/*! where to place the control points of a bezier curve to get a good approximation for a 90 degree curve */
const static double BEZIER_CTRL_POINT_FOR_90_DEGREE_CIRCLE = 0.552284749831;

void pencil_classifier_painter_init( pencil_classifier_painter_t *this_ )
{
    TRACE_BEGIN();

    pencil_marker_init( &((*this_).marker) );

    TRACE_END();
}

void pencil_classifier_painter_destroy( pencil_classifier_painter_t *this_ )
{
    TRACE_BEGIN();

    pencil_marker_destroy( &((*this_).marker) );

    TRACE_END();
}

void pencil_classifier_painter_draw ( const pencil_classifier_painter_t *this_,
                                      data_visible_classifier_t *visible_classifier,
                                      data_id_t mark_focused,
                                      data_id_t mark_highlighted,
                                      const data_small_set_t *mark_selected,
                                      const pencil_size_t *pencil_size,
                                      const geometry_rectangle_t *classifier_bounds,
                                      PangoLayout *font_layout,
                                      cairo_t *cr )
{
    TRACE_BEGIN();
    assert( NULL != pencil_size );
    assert( NULL != visible_classifier );
    assert( NULL != classifier_bounds );
    assert( NULL != font_layout );
    assert( NULL != cr );

    double left, top;
    double width, height;

    left = geometry_rectangle_get_left ( classifier_bounds );
    top = geometry_rectangle_get_top ( classifier_bounds );
    width = geometry_rectangle_get_width ( classifier_bounds );
    height = geometry_rectangle_get_height ( classifier_bounds );

    double gap = pencil_size_get_standard_object_border( pencil_size );
    double f_line_gap = pencil_size_get_font_line_gap( pencil_size );

    if (( visible_classifier != NULL ) && ( data_visible_classifier_is_valid( visible_classifier ) ))
    {
        data_classifier_t *classifier;
        data_diagramelement_t *diagramelement;
        classifier = data_visible_classifier_get_classifier_ptr( visible_classifier );
        diagramelement = data_visible_classifier_get_diagramelement_ptr( visible_classifier );
        data_diagramelement_flag_t display_flags;
        display_flags = data_diagramelement_get_display_flags( diagramelement );

        TRACE_INFO_INT("drawing classifier id", data_classifier_get_id( classifier ) );

        double std_line_width = pencil_size_get_standard_line_width( pencil_size );
        cairo_set_line_width( cr, std_line_width );

        /* set color */
        GdkRGBA foreground_color;
        {
            if ( data_id_equals_id( &mark_highlighted, DATA_TABLE_DIAGRAMELEMENT, data_diagramelement_get_id( diagramelement ) ) )
            {
                foreground_color = pencil_size_get_highlight_color( pencil_size );
            }
            else if ( 0 != ( display_flags & DATA_DIAGRAMELEMENT_FLAG_GREY_OUT ))
            {
                foreground_color = pencil_size_get_gray_out_color( pencil_size );
            }
            else
            {
                foreground_color = pencil_size_get_standard_color( pencil_size );
            }
            cairo_set_source_rgba( cr, foreground_color.red, foreground_color.green, foreground_color.blue, foreground_color.alpha );
        }

        /* draw rectangle */
        switch ( data_classifier_get_main_type ( classifier ) )
        {
            case DATA_CLASSIFIER_TYPE_BLOCK:  /* SysML */
            case DATA_CLASSIFIER_TYPE_FEATURE:
            case DATA_CLASSIFIER_TYPE_REQUIREMENT:  /* SysML */
            case DATA_CLASSIFIER_TYPE_UML_CLASS:
            case DATA_CLASSIFIER_TYPE_UML_OBJECT:
            case DATA_CLASSIFIER_TYPE_UML_PART:
            {
                cairo_rectangle ( cr, left+gap, top+gap, width-2.0*gap, height-2.0*gap );
                cairo_stroke (cr);
            }
            break;

            case DATA_CLASSIFIER_TYPE_UML_COMPONENT:
            {
                cairo_rectangle ( cr, left+gap, top+gap, width-2.0*gap, height-2.0*gap );
                cairo_stroke (cr);

                /* draw icon */
                double icon_height = pencil_size_get_larger_font_size( pencil_size );
                double icon_width;
                pencil_classifier_painter_private_draw_component_icon ( this_,
                                                                        left + width - 2*gap,  /* x */
                                                                        top + 2*gap,  /* y */
                                                                        GEOMETRY_H_ALIGN_RIGHT,
                                                                        GEOMETRY_V_ALIGN_TOP,
                                                                        icon_height,
                                                                        cr,
                                                                        &icon_width );
            }
            break;

            case DATA_CLASSIFIER_TYPE_UML_ARTIFACT:
            {
                cairo_rectangle ( cr, left+gap, top+gap, width-2.0*gap, height-2.0*gap );
                cairo_stroke (cr);

                /* draw icon */
                double icon_height = pencil_size_get_larger_font_size( pencil_size );
                double icon_width;
                pencil_classifier_painter_private_draw_artifact_icon ( this_,
                                                                       left + width - 2*gap,  /* x */
                                                                       top + 2*gap,  /* y */
                                                                       GEOMETRY_H_ALIGN_RIGHT,
                                                                       GEOMETRY_V_ALIGN_TOP,
                                                                       icon_height,
                                                                       cr,
                                                                       &icon_width );
            }
            break;

            case DATA_CLASSIFIER_TYPE_UML_ACTIVITY:
            case DATA_CLASSIFIER_TYPE_UML_STATE:
            {
                double corner_radius = 6.0*gap;
                double bottom = top + height;
                double right = left + width;
                double ctrl_offset = corner_radius * (1.0-BEZIER_CTRL_POINT_FOR_90_DEGREE_CIRCLE);

                cairo_move_to ( cr, right - corner_radius, bottom );
                cairo_line_to ( cr, left + corner_radius, bottom );
                cairo_curve_to ( cr, left + ctrl_offset, bottom, left, bottom - ctrl_offset, left /* end point x */, bottom - corner_radius /* end point y */ );
                cairo_line_to ( cr, left, top + corner_radius );
                cairo_curve_to ( cr, left, top + ctrl_offset, left + ctrl_offset, top, left + corner_radius /* end point x */, top /* end point y */ );
                cairo_line_to ( cr, right - corner_radius, top );
                cairo_curve_to ( cr, right - ctrl_offset, top, right, top + ctrl_offset, right /* end point x */, top + corner_radius /* end point y */ );
                cairo_line_to ( cr, right, bottom - corner_radius );
                cairo_curve_to ( cr, right, bottom - ctrl_offset, right - ctrl_offset, bottom, right - corner_radius /* end point x */, bottom /* end point y */ );
                cairo_stroke (cr);
            }
            break;

            case DATA_CLASSIFIER_TYPE_UML_USE_CASE:
            {
                double bottom = top + height;
                double right = left + width;
                double half_width = 0.5 * width;
                double half_height = 0.5 * height;
                double center_x = left + half_width;
                double center_y = top + half_height;
                double ctrl_xoffset = half_width * (1.0-BEZIER_CTRL_POINT_FOR_90_DEGREE_CIRCLE);
                double ctrl_yoffset = half_height * (1.0-BEZIER_CTRL_POINT_FOR_90_DEGREE_CIRCLE);

                cairo_move_to ( cr, center_x, bottom );
                cairo_curve_to ( cr, left + ctrl_xoffset, bottom, left, bottom - ctrl_yoffset, left /* end point x */, center_y /* end point y */ );
                cairo_curve_to ( cr, left, top + ctrl_yoffset, left + ctrl_xoffset, top, center_x /* end point x */, top /* end point y */ );
                cairo_curve_to ( cr, right - ctrl_xoffset, top, right, top + ctrl_yoffset, right /* end point x */, center_y /* end point y */ );
                cairo_curve_to ( cr, right, bottom - ctrl_yoffset, right - ctrl_xoffset, bottom, center_x /* end point x */, bottom /* end point y */ );
                cairo_stroke (cr);
            }
            break;

            case DATA_CLASSIFIER_TYPE_UML_ACTOR:
            case DATA_CLASSIFIER_TYPE_UML_SYSTEM_BOUNDARY:
            case DATA_CLASSIFIER_TYPE_UML_DIAGRAM_REFERENCE:
            case DATA_CLASSIFIER_TYPE_UML_NODE:
            case DATA_CLASSIFIER_TYPE_UML_INTERFACE:
            case DATA_CLASSIFIER_TYPE_UML_PACKAGE:
            case DATA_CLASSIFIER_TYPE_UML_COMMENT:
            {
                cairo_rectangle ( cr, left+gap, top+gap, width-2.0*gap, height-2.0*gap );
                cairo_stroke (cr);
            }
            break;

            default:
            {
                TSLOG_ERROR("unknown data_classifier_type_t in pencil_classifier_painter_draw()");
            }
            break;
        }

        /* draw stereotype text */
        int text1_height = 0;
        {
            if ( 0 != utf8string_get_length( data_classifier_get_stereotype_ptr( classifier ) ) )
            {
                /* prepare text */
                char stereotype_text[DATA_CLASSIFIER_MAX_STEREOTYPE_SIZE+4];
                utf8stringbuf_t stereotype_buf = UTF8STRINGBUF(stereotype_text);
                utf8stringbuf_copy_str( stereotype_buf, "<<" );
                utf8stringbuf_append_str( stereotype_buf, data_classifier_get_stereotype_ptr( classifier ) );
                utf8stringbuf_append_str( stereotype_buf, ">>" );

                int text1_width;
                pango_layout_set_font_description (font_layout, pencil_size_get_standard_font_description(pencil_size) );
                pango_layout_set_text (font_layout, utf8stringbuf_get_string( stereotype_buf ), -1);
                pango_layout_get_pixel_size (font_layout, &text1_width, &text1_height);
                cairo_move_to ( cr, left + 0.5*( width - text1_width ), top+gap );
                pango_cairo_show_layout (cr, font_layout);
            }
        }

        /* draw name text */
        {
            /* prepare text */
            char name_text[DATA_CLASSIFIER_MAX_NAME_SIZE+1];
            utf8stringbuf_t name_buf = UTF8STRINGBUF(name_text);
            if ( 0 != ( display_flags & DATA_DIAGRAMELEMENT_FLAG_INSTANCE ))
            {
                utf8stringbuf_copy_str( name_buf, ":" );
            }
            else
            {
                utf8stringbuf_clear( name_buf );
            }
            utf8stringbuf_append_str( name_buf, data_classifier_get_name_ptr( classifier ) );

            int text2_width;
            int text2_height;
            pango_layout_set_font_description (font_layout, pencil_size_get_larger_font_description(pencil_size) );
            pango_layout_set_text (font_layout, utf8stringbuf_get_string( name_buf ), -1);
            pango_layout_get_pixel_size (font_layout, &text2_width, &text2_height);

            /* highlight */
            if ( 0 != ( display_flags & DATA_DIAGRAMELEMENT_FLAG_EMPHASIS ))
            {
                GdkRGBA emph_color = pencil_size_get_emphasized_color( pencil_size );
                cairo_set_source_rgba( cr, emph_color.red, emph_color.green, emph_color.blue, emph_color.alpha );
                cairo_rectangle ( cr, left + 0.5*( width - text2_width ), top+gap+text1_height+f_line_gap, text2_width, text2_height );
                cairo_fill (cr);
            }

            /* draw text */
            cairo_set_source_rgba( cr, foreground_color.red, foreground_color.green, foreground_color.blue, foreground_color.alpha );
            cairo_move_to ( cr, left + 0.5*( width - text2_width ), top+gap+text1_height+f_line_gap );
            pango_cairo_show_layout (cr, font_layout);

            /* underline instances */
            if ( 0 != ( display_flags & DATA_DIAGRAMELEMENT_FLAG_INSTANCE ))
            {
                cairo_move_to ( cr, left + 0.5*( width - text2_width ), top+gap+text1_height+f_line_gap+text2_height );
                cairo_line_to ( cr, left + 0.5*( width + text2_width ), top+gap+text1_height+f_line_gap+text2_height );
                cairo_stroke (cr);
            }
        }

        if ( data_small_set_contains_row_id( mark_selected, DATA_TABLE_DIAGRAMELEMENT, data_diagramelement_get_id(diagramelement) ) )
        {
            pencil_marker_mark_selected_rectangle( &((*this_).marker), *classifier_bounds, cr );
        }

        if ( data_id_equals_id( &mark_focused, DATA_TABLE_DIAGRAMELEMENT, data_diagramelement_get_id(diagramelement) ) )
        {
            pencil_marker_mark_focused_rectangle( &((*this_).marker), *classifier_bounds, cr );
        }
    }
    else
    {
        TSLOG_ERROR("invalid visible classifier in array!");
    }

    TRACE_END();
}

void pencil_classifier_painter_get_minimum_bounds ( const pencil_classifier_painter_t *this_,
                                                    data_visible_classifier_t *visible_classifier,
                                                    const pencil_size_t *pencil_size,
                                                    PangoLayout *font_layout,
                                                    geometry_rectangle_t *out_classifier_bounds )
{
    TRACE_BEGIN();
    assert( NULL != pencil_size );
    assert( NULL != visible_classifier );
    assert( NULL != out_classifier_bounds );
    assert( NULL != font_layout );

    double left = 0.0;
    double top = 0.0;
    double width = 0.0;
    double height = 0.0;

    double gap = pencil_size_get_standard_object_border( pencil_size );

    if (( visible_classifier != NULL ) && ( data_visible_classifier_is_valid( visible_classifier ) ))
    {
        data_classifier_t *classifier;
        data_diagramelement_t *diagramelement;
        classifier = data_visible_classifier_get_classifier_ptr( visible_classifier );
        diagramelement = data_visible_classifier_get_diagramelement_ptr( visible_classifier );
        data_diagramelement_flag_t display_flags;
        display_flags = data_diagramelement_get_display_flags( diagramelement );

        TRACE_INFO_INT("calculating minimum bounds of classifier id", data_classifier_get_id( classifier ) );

        /* bounding rectangle */
        width += 6.0 * gap;
        height += 6.0 * gap;

        /* stereotype text */
        int text1_height = 0;
        int text1_width = 0;
        {
            if ( 0 != utf8string_get_length( data_classifier_get_stereotype_ptr( classifier ) ) )
            {
                /* prepare text */
                char stereotype_text[DATA_CLASSIFIER_MAX_STEREOTYPE_SIZE+4];
                utf8stringbuf_t stereotype_buf = UTF8STRINGBUF(stereotype_text);
                utf8stringbuf_copy_str( stereotype_buf, "<<" );
                utf8stringbuf_append_str( stereotype_buf, data_classifier_get_stereotype_ptr( classifier ) );
                utf8stringbuf_append_str( stereotype_buf, ">>" );

                /* determine text width and height */
                pango_layout_set_font_description (font_layout, pencil_size_get_standard_font_description(pencil_size) );
                pango_layout_set_text (font_layout, utf8stringbuf_get_string( stereotype_buf ), -1);
                pango_layout_get_pixel_size (font_layout, &text1_width, &text1_height);
            }
        }

        /* draw name text */
        int text2_width;
        int text2_height;
        {
            /* prepare text */
            char name_text[DATA_CLASSIFIER_MAX_NAME_SIZE+1];
            utf8stringbuf_t name_buf = UTF8STRINGBUF(name_text);
            if ( 0 != ( display_flags & DATA_DIAGRAMELEMENT_FLAG_INSTANCE ))
            {
                utf8stringbuf_copy_str( name_buf, ":" );
            }
            else
            {
                utf8stringbuf_clear( name_buf );
            }
            utf8stringbuf_append_str( name_buf, data_classifier_get_name_ptr( classifier ) );

            /* determine text width and height */
            pango_layout_set_font_description (font_layout, pencil_size_get_larger_font_description(pencil_size) );
            pango_layout_set_text (font_layout, utf8stringbuf_get_string( name_buf ), -1);
            pango_layout_get_pixel_size (font_layout, &text2_width, &text2_height);
        }

        width += ( text1_width > text2_width ) ? text1_width : text2_width;
        height += text1_height + text2_height;
    }
    else
    {
        TSLOG_ERROR("invalid visible classifier in array!");
    }

    geometry_rectangle_reinit( out_classifier_bounds, left, top, width, height );
    TRACE_END();
}

void pencil_classifier_painter_get_drawing_space ( const pencil_classifier_painter_t *this_,
                                                   data_visible_classifier_t *visible_classifier,
                                                   const pencil_size_t *pencil_size,
                                                   const geometry_rectangle_t *classifier_bounds,
                                                   PangoLayout *font_layout,
                                                   geometry_rectangle_t *out_classifier_space )
{
    TRACE_BEGIN();
    assert( NULL != pencil_size );
    assert( NULL != visible_classifier );
    assert( NULL != classifier_bounds );
    assert( NULL != out_classifier_space );
    assert( NULL != font_layout );

    data_classifier_t *classifier;
    classifier = data_visible_classifier_get_classifier_ptr( visible_classifier );

    bool has_stereotype = data_classifier_has_stereotype( classifier );
    double space_left = geometry_rectangle_get_left( classifier_bounds )
    + 2.0 * pencil_size_get_standard_object_border( pencil_size );
    double space_width = geometry_rectangle_get_width( classifier_bounds )
    - 4.0 * pencil_size_get_standard_object_border( pencil_size );
    double space_height = geometry_rectangle_get_height( classifier_bounds )
    - 4.0 * pencil_size_get_standard_object_border( pencil_size )
    - pencil_size_get_larger_font_size( pencil_size );
    /* for underscores under object instance names: */
    space_height = space_height
    - 2.0 * pencil_size_get_standard_object_border( pencil_size );
    if ( has_stereotype )
    {
        space_height = space_height
        - pencil_size_get_font_line_gap( pencil_size )
        - pencil_size_get_standard_font_size( pencil_size );
        /* this is an approximation to fix the differnce between font size and the actual line height */
        space_height = space_height
        - 2.0 * pencil_size_get_font_line_gap( pencil_size );
    }
    double space_top = geometry_rectangle_get_bottom( classifier_bounds )
    - space_height
    - 2.0 * pencil_size_get_standard_object_border( pencil_size );
    geometry_rectangle_reinit( out_classifier_space, space_left, space_top, space_width, space_height );

    TRACE_END();
}

void pencil_classifier_painter_private_draw_component_icon ( const pencil_classifier_painter_t *this_,
                                                             double x,
                                                             double y,
                                                             geometry_h_align_t h_align,
                                                             geometry_v_align_t v_align,
                                                             double height,
                                                             cairo_t *cr,
                                                             double *out_width )
{
    TRACE_BEGIN();
    assert( NULL != cr );
    assert( NULL != out_width );

    /* calculate component bounds */
    double comp_left;
    double comp_right;
    double comp_top;
    double comp_bottom;
    double comp_height;
    double comp_width;
    double port_half_width;

    comp_height = height;
    comp_width = comp_height * 1.2;
    port_half_width = height * 0.2;

    switch ( h_align )
    {
        case GEOMETRY_H_ALIGN_LEFT:
        {
            comp_left = x + port_half_width;
        }
        break;

        case GEOMETRY_H_ALIGN_CENTER:
        {
            comp_left = x + 0.5 * ( port_half_width - comp_width );
        }
        break;

        case GEOMETRY_H_ALIGN_RIGHT:
        {
            comp_left = x - comp_width;
        }
        break;

        default:
        {
            TSLOG_ERROR("unknown geometry_h_align_t in pencil_classifier_painter_private_draw_component_icon()");
        }
        break;
    }
    comp_right = comp_left + comp_width;

    switch ( v_align )
    {
        case GEOMETRY_V_ALIGN_TOP:
        {
            comp_top = y;
        }
        break;

        case GEOMETRY_V_ALIGN_CENTER:
        {
            comp_top = y - 0.5 * comp_height;
        }
        break;

        case GEOMETRY_V_ALIGN_BOTTOM:
        {
            comp_top = y - comp_height;
        }
        break;

        default:
        {
            TSLOG_ERROR("unknown geometry_v_align_t in pencil_classifier_painter_private_draw_component_icon()");
        }
        break;
    }
    comp_bottom = comp_top + comp_height;

    /* calculate bounds of the two ports */
    double port1_top;
    double port2_top;
    double port_left;
    double port_width;
    double port_height;

    port_left = comp_left - port_half_width;
    port_width = 2.0*port_half_width;
    port_height = 0.2*height;
    port1_top = comp_top + 0.2*height;
    port2_top = comp_top + 0.6*height;

    /* draw the icon */
    cairo_move_to ( cr, comp_left, port1_top );
    cairo_line_to ( cr, comp_left, comp_top );
    cairo_line_to ( cr, comp_right, comp_top );
    cairo_line_to ( cr, comp_right, comp_bottom );
    cairo_line_to ( cr, comp_left, comp_bottom );
    cairo_line_to ( cr, comp_left, port2_top + port_height );
    cairo_stroke (cr);
    cairo_move_to ( cr, comp_left, port2_top );
    cairo_line_to ( cr, comp_left, port1_top + port_height );
    cairo_stroke (cr);
    cairo_rectangle ( cr, port_left, port1_top, port_width, port_height );
    cairo_stroke (cr);
    cairo_rectangle ( cr, port_left, port2_top, port_width, port_height );
    cairo_stroke (cr);

    /* return the result */
    *out_width = comp_width + port_half_width;

    TRACE_END();
}

void pencil_classifier_painter_private_draw_artifact_icon ( const pencil_classifier_painter_t *this_,
                                                            double x,
                                                            double y,
                                                            geometry_h_align_t h_align,
                                                            geometry_v_align_t v_align,
                                                            double height,
                                                            cairo_t *cr,
                                                            double *out_width )
{
    TRACE_BEGIN();
    assert( NULL != cr );
    assert( NULL != out_width );

    /* calculate artifact bounds */
    double art_left;
    double art_right;
    double art_top;
    double art_bottom;
    double art_height;
    double art_width;
    double art_corner_edge;

    art_height = height;
    art_width = art_height * 0.7;
    art_corner_edge = height * 0.3;

    switch ( h_align )
    {
        case GEOMETRY_H_ALIGN_LEFT:
        {
            art_left = x;
        }
        break;

        case GEOMETRY_H_ALIGN_CENTER:
        {
            art_left = x - 0.5 * art_width;
        }
        break;

        case GEOMETRY_H_ALIGN_RIGHT:
        {
            art_left = x - art_width;
        }
        break;

        default:
        {
            TSLOG_ERROR("unknown geometry_h_align_t in pencil_classifier_painter_private_draw_component_icon()");
        }
        break;
    }
    art_right = art_left + art_width;

    switch ( v_align )
    {
        case GEOMETRY_V_ALIGN_TOP:
        {
            art_top = y;
        }
        break;

        case GEOMETRY_V_ALIGN_CENTER:
        {
            art_top = y - 0.5 * art_height;
        }
        break;

        case GEOMETRY_V_ALIGN_BOTTOM:
        {
            art_top = y - art_height;
        }
        break;

        default:
        {
            TSLOG_ERROR("unknown geometry_v_align_t in pencil_classifier_painter_private_draw_component_icon()");
        }
        break;
    }
    art_bottom = art_top + art_height;

    /* draw the icon */
    cairo_move_to ( cr, art_right, art_top + art_corner_edge );
    cairo_line_to ( cr, art_right - art_corner_edge, art_top + art_corner_edge );
    cairo_line_to ( cr, art_right - art_corner_edge, art_top );
    cairo_line_to ( cr, art_left, art_top );
    cairo_line_to ( cr, art_left, art_bottom );
    cairo_line_to ( cr, art_right, art_bottom );
    cairo_line_to ( cr, art_right, art_top + art_corner_edge );
    cairo_line_to ( cr, art_right - art_corner_edge, art_top );
    cairo_stroke (cr);

    /* return the result */
    *out_width = art_width;

    TRACE_END();
}


/*
Copyright 2016-2017 Andreas Warnke

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
