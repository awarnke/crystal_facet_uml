/* File: draw_contour.c; Copyright and License: see below */

#include "draw/draw_contour.h"
#include "trace.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

/*! where to place the control points of a bezier curve to get a good approximation for a 90 degree curve */
const static double BEZIER_CTRL_POINT_FOR_90_DEGREE_CIRCLE = 0.552284749831;

void draw_contour_get_shape_border_dimensions( const draw_contour_t *this_,
                                                data_classifier_type_t classifier_type,
                                                const pencil_size_t *pencil_size,
                                                double *out_top_border,
                                                double *out_left_border,
                                                double *out_bottom_border,
                                                double *out_right_border )
{
    TRACE_BEGIN();
    assert( NULL != pencil_size );
    assert( NULL != out_top_border );
    assert( NULL != out_left_border );
    assert( NULL != out_bottom_border );
    assert( NULL != out_right_border );

    double gap = pencil_size_get_standard_object_border( pencil_size );
    double double_gap = 2.0 * gap;  /* a line has the gap disance on both sides to the next object */

    switch ( classifier_type )
    {
        case DATA_CLASSIFIER_TYPE_UML_USE_CASE:
        {
            /* within a use case, space is limited: */
            double v_offset = pencil_size_get_standard_font_size( pencil_size );
            double h_offset = 1.5 * pencil_size_get_standard_font_size( pencil_size );

            *out_top_border = double_gap + v_offset;
            *out_left_border = double_gap + h_offset;
            *out_bottom_border = double_gap + v_offset;
            *out_right_border = double_gap + h_offset;
        }
        break;

        case DATA_CLASSIFIER_TYPE_UML_NODE:
        {
            /* the 3d border of a node shrinks the space */
            double offset_3d = double_gap;

            *out_top_border = double_gap + offset_3d;
            *out_left_border = double_gap;
            *out_bottom_border = double_gap;
            *out_right_border = double_gap + offset_3d;
        }
        break;

        case DATA_CLASSIFIER_TYPE_UML_ACTOR:
        case DATA_CLASSIFIER_TYPE_DYN_INITIAL_NODE:
        case DATA_CLASSIFIER_TYPE_DYN_FINAL_NODE:
        case DATA_CLASSIFIER_TYPE_DYN_FORK_NODE:
        case DATA_CLASSIFIER_TYPE_DYN_JOIN_NODE:
        case DATA_CLASSIFIER_TYPE_DYN_SHALLOW_HISTORY:
        case DATA_CLASSIFIER_TYPE_DYN_DEEP_HISTORY:
        case DATA_CLASSIFIER_TYPE_DYN_ACCEPT_TIME_EVENT:
        {
            /* the symbol icon height is part of the shape border  */
            double symbol_icon_height = pencil_size_get_classifier_symbol_height( pencil_size );

            *out_top_border = double_gap + symbol_icon_height;
            *out_left_border = gap;
            *out_bottom_border = gap;
            *out_right_border = gap;
        }
        break;

        case DATA_CLASSIFIER_TYPE_UML_DIAGRAM_REFERENCE:  /* and */
        case DATA_CLASSIFIER_TYPE_UML_PACKAGE:
        {
            double top_ornament_height = pencil_size_get_standard_font_size( pencil_size );

            *out_top_border = double_gap + top_ornament_height;
            *out_left_border = double_gap;
            *out_bottom_border = double_gap;
            *out_right_border = double_gap;
        }
        break;

        case DATA_CLASSIFIER_TYPE_BLOCK:
        case DATA_CLASSIFIER_TYPE_FEATURE:
        case DATA_CLASSIFIER_TYPE_REQUIREMENT:
        case DATA_CLASSIFIER_TYPE_UML_SYSTEM_BOUNDARY:
        case DATA_CLASSIFIER_TYPE_UML_ACTIVITY:
        case DATA_CLASSIFIER_TYPE_UML_STATE:
        case DATA_CLASSIFIER_TYPE_UML_COMPONENT:
        case DATA_CLASSIFIER_TYPE_UML_PART:
        case DATA_CLASSIFIER_TYPE_UML_INTERFACE:
        case DATA_CLASSIFIER_TYPE_UML_CLASS:
        case DATA_CLASSIFIER_TYPE_UML_OBJECT:
        case DATA_CLASSIFIER_TYPE_UML_ARTIFACT:
        case DATA_CLASSIFIER_TYPE_UML_COMMENT:
        case DATA_CLASSIFIER_TYPE_CONSTRAINT_PROPERTY:
        case DATA_CLASSIFIER_TYPE_DYN_INTERRUPTABLE_REGION:
        case DATA_CLASSIFIER_TYPE_DYN_DECISION_NODE:
        case DATA_CLASSIFIER_TYPE_DYN_ACCEPT_EVENT :
        case DATA_CLASSIFIER_TYPE_DYN_SEND_SIGNAL:
        {
            /* standard size */
            *out_top_border = double_gap;
            *out_left_border = double_gap;
            *out_bottom_border = double_gap;
            *out_right_border = double_gap;
        }
        break;

        default:
        {
            TSLOG_ERROR("unknown data_classifier_type_t in draw_contour_get_shape_border_dimensions()");
            *out_top_border = 0.0;
            *out_left_border = 0.0;
            *out_bottom_border = 0.0;
            *out_right_border = 0.0;
        }
        break;
    }
    TRACE_END();
}

void draw_contour_draw_rounded_rect ( const draw_contour_t *this_,
                                      const geometry_rectangle_t *outer_bounds,
                                      bool dashed_line,
                                      const pencil_size_t *pencil_size,
                                      cairo_t *cr )
{
    TRACE_BEGIN();
    assert ( NULL != outer_bounds );
    assert ( NULL != pencil_size );
    assert ( NULL != cr );
    const double gap = pencil_size_get_standard_object_border( pencil_size );
    const double border_left = geometry_rectangle_get_left ( outer_bounds ) + gap;
    const double border_top = geometry_rectangle_get_top ( outer_bounds ) + gap;
    const double border_width = geometry_rectangle_get_width ( outer_bounds ) - gap - gap;
    const double border_height = geometry_rectangle_get_height ( outer_bounds ) - gap - gap;
    const double border_bottom = border_top + border_height;
    const double border_right = border_left + border_width;
    const double corner_radius = 6.0 * gap;
    const double ctrl_offset = corner_radius * (1.0-BEZIER_CTRL_POINT_FOR_90_DEGREE_CIRCLE);

    /* set dashes */
    if ( dashed_line )
    {
        double dashes[2];
        dashes[0] = 2.0 * pencil_size_get_line_dash_length( pencil_size );
        dashes[1] = 1.0 * pencil_size_get_line_dash_length( pencil_size );
        cairo_set_dash ( cr, dashes, 2, 0.0 );
    }

    cairo_move_to ( cr, border_right - corner_radius, border_bottom );
    cairo_line_to ( cr, border_left + corner_radius, border_bottom );
    cairo_curve_to ( cr, border_left + ctrl_offset, border_bottom, border_left, border_bottom - ctrl_offset, border_left /* end point x */, border_bottom - corner_radius /* end point y */ );
    cairo_line_to ( cr, border_left, border_top + corner_radius );
    cairo_curve_to ( cr, border_left, border_top + ctrl_offset, border_left + ctrl_offset, border_top, border_left + corner_radius /* end point x */, border_top /* end point y */ );
    cairo_line_to ( cr, border_right - corner_radius, border_top );
    cairo_curve_to ( cr, border_right - ctrl_offset, border_top, border_right, border_top + ctrl_offset, border_right /* end point x */, border_top + corner_radius /* end point y */ );
    cairo_line_to ( cr, border_right, border_bottom - corner_radius );
    cairo_curve_to ( cr, border_right, border_bottom - ctrl_offset, border_right - ctrl_offset, border_bottom, border_right - corner_radius /* end point x */, border_bottom /* end point y */ );
    cairo_stroke (cr);

    /* reset dashes */
    if ( dashed_line )
    {
        cairo_set_dash ( cr, NULL, 0, 0.0 );
    }

    TRACE_END();
}

void draw_contour_draw_ellipse ( const draw_contour_t *this_,
                                 const geometry_rectangle_t *outer_bounds,
                                 const pencil_size_t *pencil_size,
                                 cairo_t *cr )
{
    TRACE_BEGIN();
    assert ( NULL != outer_bounds );
    assert ( NULL != pencil_size );
    assert ( NULL != cr );
    const double gap = pencil_size_get_standard_object_border( pencil_size );
    const double border_left = geometry_rectangle_get_left ( outer_bounds ) + gap;
    const double border_top = geometry_rectangle_get_top ( outer_bounds ) + gap;
    const double border_width = geometry_rectangle_get_width ( outer_bounds ) - gap - gap;
    const double border_height = geometry_rectangle_get_height ( outer_bounds ) - gap - gap;
    const double border_bottom = border_top + border_height;
    const double border_right = border_left + border_width;
    const double half_width = 0.5 * border_width;
    const double half_height = 0.5 * border_height;
    const double center_x = border_left + half_width;
    const double center_y = border_top + half_height;
    const double ctrl_xoffset = half_width * (1.0-BEZIER_CTRL_POINT_FOR_90_DEGREE_CIRCLE);
    const double ctrl_yoffset = half_height * (1.0-BEZIER_CTRL_POINT_FOR_90_DEGREE_CIRCLE);

    cairo_move_to ( cr, center_x, border_bottom );
    cairo_curve_to ( cr, border_left + ctrl_xoffset, border_bottom, border_left, border_bottom - ctrl_yoffset, border_left /* end point x */, center_y /* end point y */ );
    cairo_curve_to ( cr, border_left, border_top + ctrl_yoffset, border_left + ctrl_xoffset, border_top, center_x /* end point x */, border_top /* end point y */ );
    cairo_curve_to ( cr, border_right - ctrl_xoffset, border_top, border_right, border_top + ctrl_yoffset, border_right /* end point x */, center_y /* end point y */ );
    cairo_curve_to ( cr, border_right, border_bottom - ctrl_yoffset, border_right - ctrl_xoffset, border_bottom, center_x /* end point x */, border_bottom /* end point y */ );
    cairo_stroke (cr);

    TRACE_END();
}

void draw_contour_draw_rhombus ( const draw_contour_t *this_,
                                 const geometry_rectangle_t *outer_bounds,
                                 const pencil_size_t *pencil_size,
                                 cairo_t *cr )
{
    TRACE_BEGIN();
    assert ( NULL != outer_bounds );
    assert ( NULL != pencil_size );
    assert ( NULL != cr );
    const double gap = pencil_size_get_standard_object_border( pencil_size );
    const double border_left = geometry_rectangle_get_left ( outer_bounds ) + gap;
    const double border_top = geometry_rectangle_get_top ( outer_bounds ) + gap;
    const double border_width = geometry_rectangle_get_width ( outer_bounds ) - gap - gap;
    const double border_height = geometry_rectangle_get_height ( outer_bounds ) - gap - gap;
    const double border_bottom = border_top + border_height;
    const double border_right = border_left + border_width;
    const double x_center = geometry_rectangle_get_x_center ( outer_bounds );
    const double y_center = geometry_rectangle_get_y_center ( outer_bounds );

    cairo_move_to ( cr, x_center, border_bottom );
    cairo_line_to ( cr, border_left, y_center );
    cairo_line_to ( cr, x_center, border_top );
    cairo_line_to ( cr, border_right, y_center );
    cairo_line_to ( cr, x_center, border_bottom );
    cairo_stroke (cr);

    TRACE_END();
}

void draw_contour_draw_3d_box ( const draw_contour_t *this_,
                                const geometry_rectangle_t *outer_bounds,
                                const pencil_size_t *pencil_size,
                                cairo_t *cr )
{
    TRACE_BEGIN();
    assert ( NULL != outer_bounds );
    assert ( NULL != pencil_size );
    assert ( NULL != cr );
    const double gap = pencil_size_get_standard_object_border( pencil_size );
    const double border_left = geometry_rectangle_get_left ( outer_bounds ) + gap;
    const double border_top = geometry_rectangle_get_top ( outer_bounds ) + gap;
    const double border_width = geometry_rectangle_get_width ( outer_bounds ) - gap - gap;
    const double border_height = geometry_rectangle_get_height ( outer_bounds ) - gap - gap;
    const double border_bottom = border_top + border_height;
    const double border_right = border_left + border_width;
    const double offset_3d = 2.0*gap;

    cairo_rectangle ( cr, border_left, border_top+offset_3d, border_width-offset_3d, border_height-offset_3d );
    cairo_stroke (cr);

    cairo_move_to ( cr, border_left, border_top+offset_3d );
    cairo_line_to ( cr, border_left+offset_3d, border_top );
    cairo_line_to ( cr, border_right, border_top );
    cairo_line_to ( cr, border_right, border_bottom-offset_3d );
    cairo_line_to ( cr, border_right-offset_3d, border_bottom );
    cairo_stroke (cr);
    cairo_move_to ( cr, border_right, border_top );
    cairo_line_to ( cr, border_right-offset_3d, border_top+offset_3d );
    cairo_stroke (cr);

    TRACE_END();
}

void draw_contour_draw_accept_event ( const draw_contour_t *this_,
                                      const geometry_rectangle_t *outer_bounds,
                                      const pencil_size_t *pencil_size,
                                      cairo_t *cr )
{
    TRACE_BEGIN();
    assert ( NULL != outer_bounds );
    assert ( NULL != pencil_size );
    assert ( NULL != cr );
    const double gap = pencil_size_get_standard_object_border( pencil_size );
    const double border_left = geometry_rectangle_get_left ( outer_bounds ) + gap;
    const double border_top = geometry_rectangle_get_top ( outer_bounds ) + gap;
    const double border_width = geometry_rectangle_get_width ( outer_bounds ) - gap - gap;
    const double border_height = geometry_rectangle_get_height ( outer_bounds ) - gap - gap;
    const double border_bottom = border_top + border_height;
    const double border_right = border_left + border_width;
    const double y_center = geometry_rectangle_get_y_center ( outer_bounds );
    const double x_indent = border_height / 2.0;

    cairo_move_to ( cr, border_right, border_bottom );
    cairo_line_to ( cr, border_right, border_top );
    cairo_line_to ( cr, border_left, border_top );
    cairo_line_to ( cr, border_left + x_indent, y_center );
    cairo_line_to ( cr, border_left, border_bottom );
    cairo_line_to ( cr, border_right, border_bottom );
    cairo_stroke (cr);

    TRACE_END();
}

void draw_contour_draw_send_signal ( const draw_contour_t *this_,
                                     const geometry_rectangle_t *outer_bounds,
                                     const pencil_size_t *pencil_size,
                                     cairo_t *cr )
{
    TRACE_BEGIN();
    assert ( NULL != outer_bounds );
    assert ( NULL != pencil_size );
    assert ( NULL != cr );
    const double gap = pencil_size_get_standard_object_border( pencil_size );
    const double border_left = geometry_rectangle_get_left ( outer_bounds ) + gap;
    const double border_top = geometry_rectangle_get_top ( outer_bounds ) + gap;
    const double border_width = geometry_rectangle_get_width ( outer_bounds ) - gap - gap;
    const double border_height = geometry_rectangle_get_height ( outer_bounds ) - gap - gap;
    const double border_bottom = border_top + border_height;
    const double border_right = border_left + border_width;
    const double y_center = geometry_rectangle_get_y_center ( outer_bounds );
    const double x_indent = border_height / 2.0;

    cairo_move_to ( cr, border_right - x_indent, border_bottom );
    cairo_line_to ( cr, border_right, y_center );
    cairo_line_to ( cr, border_right - x_indent, border_top );
    cairo_line_to ( cr, border_left, border_top );
    cairo_line_to ( cr, border_left, border_bottom );
    cairo_line_to ( cr, border_right - x_indent, border_bottom );
    cairo_stroke (cr);

    TRACE_END();
}

void draw_contour_draw_package ( const draw_contour_t *this_,
                                 const geometry_rectangle_t *outer_bounds,
                                 const pencil_size_t *pencil_size,
                                 cairo_t *cr )
{
    TRACE_BEGIN();
    assert ( NULL != outer_bounds );
    assert ( NULL != pencil_size );
    assert ( NULL != cr );
    const double gap = pencil_size_get_standard_object_border( pencil_size );
    const double border_left = geometry_rectangle_get_left ( outer_bounds ) + gap;
    const double border_top = geometry_rectangle_get_top ( outer_bounds ) + gap;
    const double border_width = geometry_rectangle_get_width ( outer_bounds ) - gap - gap;
    const double border_height = geometry_rectangle_get_height ( outer_bounds ) - gap - gap;
    const double top_ornament_height = pencil_size_get_standard_font_size( pencil_size );

    cairo_rectangle ( cr, border_left, border_top, border_width/3.0, top_ornament_height );
    cairo_rectangle ( cr, border_left, border_top+top_ornament_height, border_width, border_height-top_ornament_height );
    cairo_stroke (cr);

    TRACE_END();
}

void draw_contour_draw_diagram_ref ( const draw_contour_t *this_,
                                 const geometry_rectangle_t *outer_bounds,
                                 const pencil_size_t *pencil_size,
                                 cairo_t *cr )
{
    TRACE_BEGIN();
    assert ( NULL != outer_bounds );
    assert ( NULL != pencil_size );
    assert ( NULL != cr );
    const double gap = pencil_size_get_standard_object_border( pencil_size );
    const double border_left = geometry_rectangle_get_left ( outer_bounds ) + gap;
    const double border_top = geometry_rectangle_get_top ( outer_bounds ) + gap;
    const double border_width = geometry_rectangle_get_width ( outer_bounds ) - gap - gap;
    const double border_height = geometry_rectangle_get_height ( outer_bounds ) - gap - gap;
    const double title_corner_height = pencil_size_get_standard_font_size( pencil_size );
    const double title_corner_edge45 = 0.4 * title_corner_height;
    const double title_corner_width = border_width/5.0;

    cairo_rectangle ( cr, border_left, border_top, border_width, border_height );
    cairo_move_to ( cr, border_left, border_top+title_corner_height );
    cairo_line_to ( cr, border_left+title_corner_width - title_corner_edge45, border_top+title_corner_height );
    cairo_line_to ( cr, border_left+title_corner_width, border_top+title_corner_height-title_corner_edge45 );
    cairo_line_to ( cr, border_left+title_corner_width, border_top );
    cairo_stroke (cr);

    TRACE_END();
}

void draw_contour_draw_comment ( const draw_contour_t *this_,
                                 const geometry_rectangle_t *outer_bounds,
                                 const pencil_size_t *pencil_size,
                                 cairo_t *cr )
{
    TRACE_BEGIN();
    assert ( NULL != outer_bounds );
    assert ( NULL != pencil_size );
    assert ( NULL != cr );
    const double gap = pencil_size_get_standard_object_border( pencil_size );
    const double border_left = geometry_rectangle_get_left ( outer_bounds ) + gap;
    const double border_top = geometry_rectangle_get_top ( outer_bounds ) + gap;
    const double border_width = geometry_rectangle_get_width ( outer_bounds ) - gap - gap;
    const double border_height = geometry_rectangle_get_height ( outer_bounds ) - gap - gap;
    const double corner_edge = pencil_size_get_standard_font_size( pencil_size );
    const double border_bottom = border_top + border_height;
    const double border_right = border_left + border_width;

    cairo_move_to ( cr, border_right, border_top + corner_edge );
    cairo_line_to ( cr, border_right - corner_edge, border_top + corner_edge );
    cairo_line_to ( cr, border_right - corner_edge, border_top );
    cairo_line_to ( cr, border_left, border_top );
    cairo_line_to ( cr, border_left, border_bottom );
    cairo_line_to ( cr, border_right, border_bottom );
    cairo_line_to ( cr, border_right, border_top + corner_edge );
    cairo_line_to ( cr, border_right - corner_edge, border_top );
    cairo_stroke (cr);

    TRACE_END();
}


/*
Copyright 2016-2019 Andreas Warnke
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