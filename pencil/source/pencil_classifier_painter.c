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

        /* prepare text to draw */
        int text1_top = top+2*gap;

        /* draw rectangle */
        int border_left = left + gap;
        int border_top = top + gap;
        int border_width = width - 2.0 * gap;
        int border_height = height - 2.0 * gap;
        switch ( data_classifier_get_main_type ( classifier ) )
        {
            case DATA_CLASSIFIER_TYPE_BLOCK:  /* SysML */
            case DATA_CLASSIFIER_TYPE_FEATURE:
            case DATA_CLASSIFIER_TYPE_REQUIREMENT:  /* SysML */
            case DATA_CLASSIFIER_TYPE_UML_CLASS:
            case DATA_CLASSIFIER_TYPE_UML_OBJECT:
            case DATA_CLASSIFIER_TYPE_UML_PART:
            {
                cairo_rectangle ( cr, border_left, border_top, border_width, border_height );
                cairo_stroke (cr);

                /* adjust the text position */
                text1_top = border_top + gap;
            }
            break;

            case DATA_CLASSIFIER_TYPE_UML_COMPONENT:
            {
                cairo_rectangle ( cr, border_left, border_top, border_width, border_height );
                cairo_stroke (cr);

                /* draw icon */
                double icon_height = pencil_size_get_larger_font_size( pencil_size );
                double icon_width;
                pencil_classifier_painter_private_draw_component_icon ( this_,
                                                                        border_left + border_width - gap,  /* x */
                                                                        border_top + gap,  /* y */
                                                                        GEOMETRY_H_ALIGN_RIGHT,
                                                                        GEOMETRY_V_ALIGN_TOP,
                                                                        icon_height,
                                                                        cr,
                                                                        &icon_width );

                /* adjust the text position */
                text1_top = border_top + gap;
            }
            break;

            case DATA_CLASSIFIER_TYPE_UML_ARTIFACT:
            {
                cairo_rectangle ( cr, border_left, border_top, border_width, border_height );
                cairo_stroke (cr);

                /* draw icon */
                double icon_height = pencil_size_get_larger_font_size( pencil_size );
                double icon_width;
                pencil_classifier_painter_private_draw_artifact_icon ( this_,
                                                                       border_left + border_width - gap,  /* x */
                                                                       border_top + gap,  /* y */
                                                                       GEOMETRY_H_ALIGN_RIGHT,
                                                                       GEOMETRY_V_ALIGN_TOP,
                                                                       icon_height,
                                                                       cr,
                                                                       &icon_width );

                /* adjust the text position */
                text1_top = border_top + gap;
            }
            break;

            case DATA_CLASSIFIER_TYPE_UML_ACTIVITY:
            case DATA_CLASSIFIER_TYPE_UML_STATE:
            {
                double corner_radius = 6.0*gap;
                double bottom = border_top + border_height;
                double right = border_left + border_width;
                double ctrl_offset = corner_radius * (1.0-BEZIER_CTRL_POINT_FOR_90_DEGREE_CIRCLE);

                cairo_move_to ( cr, right - corner_radius, bottom );
                cairo_line_to ( cr, border_left + corner_radius, bottom );
                cairo_curve_to ( cr, border_left + ctrl_offset, bottom, border_left, bottom - ctrl_offset, border_left /* end point x */, bottom - corner_radius /* end point y */ );
                cairo_line_to ( cr, border_left, border_top + corner_radius );
                cairo_curve_to ( cr, border_left, border_top + ctrl_offset, border_left + ctrl_offset, border_top, border_left + corner_radius /* end point x */, border_top /* end point y */ );
                cairo_line_to ( cr, right - corner_radius, border_top );
                cairo_curve_to ( cr, right - ctrl_offset, border_top, right, border_top + ctrl_offset, right /* end point x */, border_top + corner_radius /* end point y */ );
                cairo_line_to ( cr, right, bottom - corner_radius );
                cairo_curve_to ( cr, right, bottom - ctrl_offset, right - ctrl_offset, bottom, right - corner_radius /* end point x */, bottom /* end point y */ );
                cairo_stroke (cr);

                /* adjust the text position */
                text1_top = border_top + gap;
            }
            break;

            case DATA_CLASSIFIER_TYPE_UML_USE_CASE:
            {
                double bottom = border_top + border_height;
                double right = border_left + border_width;
                double half_width = 0.5 * border_width;
                double half_height = 0.5 * border_height;
                double center_x = border_left + half_width;
                double center_y = border_top + half_height;
                double ctrl_xoffset = half_width * (1.0-BEZIER_CTRL_POINT_FOR_90_DEGREE_CIRCLE);
                double ctrl_yoffset = half_height * (1.0-BEZIER_CTRL_POINT_FOR_90_DEGREE_CIRCLE);

                cairo_move_to ( cr, center_x, bottom );
                cairo_curve_to ( cr, border_left + ctrl_xoffset, bottom, border_left, bottom - ctrl_yoffset, border_left /* end point x */, center_y /* end point y */ );
                cairo_curve_to ( cr, border_left, border_top + ctrl_yoffset, border_left + ctrl_xoffset, border_top, center_x /* end point x */, border_top /* end point y */ );
                cairo_curve_to ( cr, right - ctrl_xoffset, border_top, right, border_top + ctrl_yoffset, right /* end point x */, center_y /* end point y */ );
                cairo_curve_to ( cr, right, bottom - ctrl_yoffset, right - ctrl_xoffset, bottom, center_x /* end point x */, bottom /* end point y */ );
                cairo_stroke (cr);

                /* adjust the text position */
                double some_offset = pencil_size_get_standard_font_size( pencil_size );
                text1_top = border_top + gap + some_offset;
            }
            break;

            case DATA_CLASSIFIER_TYPE_UML_NODE:
            {
                double offset_3d = 2.0*gap;
                double bottom = border_top + border_height;
                double right = border_left + border_width;

                cairo_rectangle ( cr, border_left, border_top+offset_3d, border_width-offset_3d, border_height-offset_3d );
                cairo_stroke (cr);

                cairo_move_to ( cr, border_left, border_top+offset_3d );
                cairo_line_to ( cr, border_left+offset_3d, border_top );
                cairo_line_to ( cr, right, border_top );
                cairo_line_to ( cr, right, bottom-offset_3d );
                cairo_line_to ( cr, right-offset_3d, bottom );
                cairo_stroke (cr);
                cairo_move_to ( cr, right, border_top );
                cairo_line_to ( cr, right-offset_3d, border_top+offset_3d );
                cairo_stroke (cr);

                /* adjust the text position */
                text1_top = border_top + offset_3d + gap;
            }
            break;

            case DATA_CLASSIFIER_TYPE_UML_ACTOR:
            {
                double actor_height = 2.5 * pencil_size_get_larger_font_size( pencil_size );
                double half_width = 0.5 * border_width;
                double actor_width;
                pencil_classifier_painter_private_draw_actor_icon ( this_,
                                                                    border_left + half_width,
                                                                    border_top,
                                                                    GEOMETRY_H_ALIGN_CENTER,
                                                                    GEOMETRY_V_ALIGN_TOP,
                                                                    actor_height,
                                                                    cr,
                                                                    &actor_width );

                /* adjust the text position */
                text1_top = border_top + actor_height + gap;
            }
            break;

            case DATA_CLASSIFIER_TYPE_UML_SYSTEM_BOUNDARY:
            case DATA_CLASSIFIER_TYPE_UML_INTERFACE:
            {
                cairo_rectangle ( cr, border_left, border_top, border_width, border_height );
                cairo_stroke (cr);

                /* adjust the text position */
                text1_top = border_top + gap;
            }
            break;

            case DATA_CLASSIFIER_TYPE_UML_DIAGRAM_REFERENCE:
            {
                double top_ornament_height = pencil_size_get_standard_font_size( pencil_size );

                cairo_rectangle ( cr, border_left, border_top, border_width/5.0, top_ornament_height );

                cairo_rectangle ( cr, border_left, border_top, border_width, border_height );
                cairo_stroke (cr);

                /* adjust the text position */
                text1_top = border_top + gap + top_ornament_height;
            }
            break;

            case DATA_CLASSIFIER_TYPE_UML_PACKAGE:
            {
                double top_ornament_height = pencil_size_get_standard_font_size( pencil_size );

                cairo_rectangle ( cr, border_left, border_top, border_width/3.0, top_ornament_height );

                cairo_rectangle ( cr, border_left, border_top+top_ornament_height, border_width, border_height-top_ornament_height );
                cairo_stroke (cr);

                /* adjust the text position */
                text1_top = border_top + gap + top_ornament_height;
            }
            break;

            case DATA_CLASSIFIER_TYPE_UML_COMMENT:
            {
                double corner_edge = pencil_size_get_standard_font_size( pencil_size );
                double border_bottom = border_top + border_height;
                double border_right = border_left + border_width;

                cairo_move_to ( cr, border_right, border_top + corner_edge );
                cairo_line_to ( cr, border_right - corner_edge, border_top + corner_edge );
                cairo_line_to ( cr, border_right - corner_edge, border_top );
                cairo_line_to ( cr, border_left, border_top );
                cairo_line_to ( cr, border_left, border_bottom );
                cairo_line_to ( cr, border_right, border_bottom );
                cairo_line_to ( cr, border_right, border_top + corner_edge );
                cairo_line_to ( cr, border_right - corner_edge, border_top );
                cairo_stroke (cr);

                /* adjust the text position */
                text1_top = border_top + gap;
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
                cairo_move_to ( cr, left + 0.5*( width - text1_width ), text1_top );
                pango_cairo_show_layout (cr, font_layout);
            }
        }

        /* draw name text */
        int text2_height = 0;
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
            pango_layout_set_font_description (font_layout, pencil_size_get_larger_font_description(pencil_size) );
            pango_layout_set_text (font_layout, utf8stringbuf_get_string( name_buf ), -1);
            pango_layout_get_pixel_size (font_layout, &text2_width, &text2_height);

            /* highlight */
            if ( 0 != ( display_flags & DATA_DIAGRAMELEMENT_FLAG_EMPHASIS ))
            {
                GdkRGBA emph_color = pencil_size_get_emphasized_color( pencil_size );
                cairo_set_source_rgba( cr, emph_color.red, emph_color.green, emph_color.blue, emph_color.alpha );
                cairo_rectangle ( cr, left + 0.5*( width - text2_width ), text1_top+text1_height+f_line_gap, text2_width, text2_height );
                cairo_fill (cr);
            }

            /* draw text */
            cairo_set_source_rgba( cr, foreground_color.red, foreground_color.green, foreground_color.blue, foreground_color.alpha );
            cairo_move_to ( cr, left + 0.5*( width - text2_width ), text1_top+text1_height+f_line_gap );
            pango_cairo_show_layout (cr, font_layout);

            /* underline instances */
            if ( 0 != ( display_flags & DATA_DIAGRAMELEMENT_FLAG_INSTANCE ))
            {
                cairo_move_to ( cr, left + 0.5*( width - text2_width ), text1_top+text1_height+f_line_gap+text2_height );
                cairo_line_to ( cr, left + 0.5*( width + text2_width ), text1_top+text1_height+f_line_gap+text2_height );
                cairo_stroke (cr);
            }
        }

        /* draw description text */
        if (( DATA_CLASSIFIER_TYPE_UML_COMMENT == data_classifier_get_main_type ( classifier ) )
            || ( DATA_CLASSIFIER_TYPE_REQUIREMENT == data_classifier_get_main_type ( classifier ) ))
        {
            int text3_width;
            int text3_height;
            pango_layout_set_font_description (font_layout, pencil_size_get_standard_font_description(pencil_size) );
            pango_layout_set_text (font_layout, data_classifier_get_description_ptr( classifier ), -1);
            pango_layout_get_pixel_size (font_layout, &text3_width, &text3_height);

            /* draw text */
            cairo_set_source_rgba( cr, foreground_color.red, foreground_color.green, foreground_color.blue, foreground_color.alpha );
            cairo_move_to ( cr, left + 0.5*( width - text3_width ), text1_top+text1_height+f_line_gap+text2_height+f_line_gap );
            pango_cairo_show_layout (cr, font_layout);
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
                                                    geometry_rectangle_t *out_classifier_bounds,
                                                    geometry_rectangle_t *out_classifier_space )
{
    TRACE_BEGIN();
    assert( NULL != pencil_size );
    assert( NULL != visible_classifier );
    assert( NULL != out_classifier_bounds );
    assert( NULL != out_classifier_space );
    assert( NULL != font_layout );

    /* border sizes of the classifier-shape */
    double top_border = 0.0;
    double left_border = 0.0;
    double bottom_border = 0.0;
    double right_border = 0.0;

    /* stereotype and name dimensions */
    double text_height;
    double text_width;

    if ( data_visible_classifier_is_valid( visible_classifier ) )
    {
        data_classifier_t *classifier;
        data_diagramelement_t *diagramelement;
        classifier = data_visible_classifier_get_classifier_ptr( visible_classifier );

        TRACE_INFO_INT("calculating minimum bounds of classifier id", data_classifier_get_id( classifier ) );

        /* determine border sizes of the classifier-shape */
        pencil_classifier_painter_private_get_shape_border_dimensions( this_,
                                                                       data_classifier_get_main_type ( classifier ),
                                                                       pencil_size,
                                                                       &top_border,
                                                                       &left_border,
                                                                       &bottom_border,
                                                                       &right_border );

        /* determine stereotype and name dimensions */
        pencil_classifier_painter_private_get_stereotype_and_name_dimensions( this_,
                                                                              visible_classifier,
                                                                              pencil_size,
                                                                              font_layout,
                                                                              &text_height,
                                                                              &text_width );
    }
    else
    {
        TSLOG_ERROR("invalid visible classifier in array!");
    }

    /* minimum bounding box */
    double left = 0.0;
    double top = 0.0;
    double width = left_border + text_width + right_border;
    double height = top_border + text_height + bottom_border;

    geometry_rectangle_reinit( out_classifier_bounds, left, top, width, height );
    geometry_rectangle_reinit( out_classifier_space, left + left_border, top + top_border + text_height, text_width, 0.0 );
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

    double space_left = 0.0;
    double space_top = 0.0;
    double space_width = 0.0;
    double space_height = 0.0;

    if ( data_visible_classifier_is_valid( visible_classifier ) )
    {
        /* get the classifier */
        data_classifier_t *classifier;
        classifier = data_visible_classifier_get_classifier_ptr( visible_classifier );

        /* determine border sizes of the classifier-shape */
        double top_border;
        double left_border;
        double bottom_border;
        double right_border;
        pencil_classifier_painter_private_get_shape_border_dimensions( this_,
                                                                    data_classifier_get_main_type ( classifier ),
                                                                    pencil_size,
                                                                    &top_border,
                                                                    &left_border,
                                                                    &bottom_border,
                                                                    &right_border );

        /* determine stereotype and name dimensions */
        double text_height;
        double text_width;
        pencil_classifier_painter_private_get_stereotype_and_name_dimensions( this_,
                                                                              visible_classifier,
                                                                              pencil_size,
                                                                              font_layout,
                                                                              &text_height,
                                                                              &text_width );

        /* calculate the result */
        space_left = geometry_rectangle_get_left( classifier_bounds ) + left_border;
        space_width = geometry_rectangle_get_width( classifier_bounds ) - left_border - right_border;
        space_top = geometry_rectangle_get_top( classifier_bounds ) + top_border + text_height;
        space_height = geometry_rectangle_get_height( classifier_bounds ) - top_border - bottom_border - text_height;
    }
    else
    {
        TSLOG_ERROR("invalid visible classifier in array!");
    }

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

void pencil_classifier_painter_private_draw_actor_icon ( const pencil_classifier_painter_t *this_,
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

    /* calculate actor bounds */
    double act_left;
    double act_right;
    double act_top;
    double act_bottom;
    double act_height;
    double act_width;

    act_height = height;
    act_width = height/3.0;
    act_left = geometry_h_align_get_left( &h_align, act_width, x, 0.0 );
    act_top = geometry_v_align_get_top( &v_align, act_height, y, 0.0 );
    act_right = act_left + act_width;
    act_bottom = act_top + act_height;

    /* draw the icon */
    double radius = act_width/2.0;
    double ctrl_offset = radius * (1.0-BEZIER_CTRL_POINT_FOR_90_DEGREE_CIRCLE);
    double head_bottom = act_top + act_height/3.0;
    double leg_top = act_top + act_height*0.6;
    double arm_top = act_top + act_height*0.45;

    cairo_move_to ( cr, act_left + radius, head_bottom );
    cairo_curve_to ( cr, act_left + ctrl_offset, head_bottom, act_left, head_bottom - ctrl_offset, act_left /* end point x */, act_top + radius /* end point y */ );
    cairo_curve_to ( cr, act_left, act_top + ctrl_offset, act_left + ctrl_offset, act_top, act_left + radius /* end point x */, act_top /* end point y */ );
    cairo_curve_to ( cr, act_right - ctrl_offset, act_top, act_right, act_top + ctrl_offset, act_right /* end point x */, act_top + radius /* end point y */ );
    cairo_curve_to ( cr, act_right, head_bottom - ctrl_offset, act_right - ctrl_offset, head_bottom, act_left + radius /* end point x */, head_bottom /* end point y */ );
    cairo_line_to ( cr, act_left + radius, leg_top );
    cairo_line_to ( cr, act_left, act_bottom );
    cairo_move_to ( cr, act_right, act_bottom );
    cairo_line_to ( cr, act_left + radius, leg_top );
    cairo_move_to ( cr, act_right, arm_top );
    cairo_line_to ( cr, act_left, arm_top );
    cairo_stroke (cr);

    /* return the result */
    *out_width = act_width;

    TRACE_END();
}


/*
Copyright 2016-2018 Andreas Warnke
    http://www.apache.org/licenses/LICENSE-2.0

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at


Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/
