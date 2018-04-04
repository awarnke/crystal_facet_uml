/* File: pencil_feature_painter.h; Copyright and License: see below */

#ifndef PENCIL_FEATURE_PAINTER_H
#define PENCIL_FEATURE_PAINTER_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Renders a feature into a cairo drawing context
 */

#include "pencil_input_data.h"
#include "pencil_marker.h"
#include "pencil_size.h"
#include "layout/layout_feature.h"
#include "util/geometry/geometry_rectangle.h"
#include "data_diagram.h"
#include "util/id/data_small_set.h"
#include "data_id.h"
#include <cairo.h>
#include <stdint.h>

/*!
 *  \brief attributes of the feature painter
 */
struct pencil_feature_painter_struct {
    pencil_marker_t marker;  /*!< own instance of a marker */
};

typedef struct pencil_feature_painter_struct pencil_feature_painter_t;

/*!
 *  \brief initializes the painter
 *
 *  \param this_ pointer to own object attributes
 */
void pencil_feature_painter_init( pencil_feature_painter_t *this_ );

/*!
 *  \brief destroys the painter
 *
 *  \param this_ pointer to own object attributes
 */
void pencil_feature_painter_destroy( pencil_feature_painter_t *this_ );

/*!
 *  \brief draws the chosen classifier contents into the diagram_bounds area of the cairo drawing context
 *
 *  \param this_ pointer to own object attributes
 *  \param layouted_feature pointer to the layout-information and data to be drawn
 *  \param mark_focused true if the object is to be marked as "focused"
 *  \param mark_highlighted true if the object is to be marked as "highlighted"
 *  \param mark_selected true if the object is to be marked as "selected"
 *  \param grey_out true if the object is to be marked as greyed out. Is ignored in case mark_highlighted is true.
 *  \param pencil_size set of sizes and colors for drawing lines and text
 *  \param layout structure to layout fonts
 *  \param cr a cairo drawing context
 */
void pencil_feature_painter_draw ( pencil_feature_painter_t *this_,
                                   layout_feature_t *layouted_feature,
                                   bool mark_focused,
                                   bool mark_highlighted,
                                   bool mark_selected,
                                   bool grey_out,
                                   pencil_size_t *pencil_size,
                                   PangoLayout *layout,
                                   cairo_t *cr
                                 );

/*!
 *  \brief determines the minumum feature bounds
 *
 *  \param this_ pointer to own object attributes
 *  \param the_feature pointer to the data to be layouted
 *  \param pencil_size set of sizes and colors for drawing lines and text
 *  \param font_layout pango layout object to determine the font metrics in the current cairo drawing context
 *  \param out_feature_bounds memory location where the result shall be stored. Must not be NULL.
 */
void pencil_feature_painter_get_minimum_bounds ( pencil_feature_painter_t *this_,
                                                 const data_feature_t *the_feature,
                                                 const pencil_size_t *pencil_size,
                                                 PangoLayout *font_layout,
                                                 geometry_rectangle_t *out_feature_bounds
                                               );

#endif  /* PENCIL_FEATURE_PAINTER_H */


/*
Copyright 2017-2018 Andreas Warnke

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
