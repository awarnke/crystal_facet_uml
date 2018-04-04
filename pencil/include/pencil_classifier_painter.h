/* File: pencil_classifier_painter.h; Copyright and License: see below */

#ifndef PENCIL_CLASSIFIER_PAINTER_H
#define PENCIL_CLASSIFIER_PAINTER_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Renders a classifier and its contents into a cairo drawing context
 */

#include "pencil_input_data.h"
#include "pencil_marker.h"
#include "pencil_size.h"
#include "pencil_feature_painter.h"
#include "layout/layout_visible_classifier.h"
#include "util/geometry/geometry_rectangle.h"
#include "util/geometry/geometry_h_align.h"
#include "util/geometry/geometry_v_align.h"
#include "data_diagram.h"
#include "util/id/data_small_set.h"
#include "data_id.h"
#include <cairo.h>
#include <stdint.h>

/*!
 *  \brief attributes of the classifier painter
 */
struct pencil_classifier_painter_struct {
    pencil_marker_t marker;  /*!< own instance of a marker */
};

typedef struct pencil_classifier_painter_struct pencil_classifier_painter_t;

/*!
 *  \brief initializes the painter
 *
 *  \param this_ pointer to own object attributes
 */
void pencil_classifier_painter_init( pencil_classifier_painter_t *this_ );

/*!
 *  \brief destroys the painter
 *
 *  \param this_ pointer to own object attributes
 */
void pencil_classifier_painter_destroy( pencil_classifier_painter_t *this_ );

/*!
 *  \brief draws the chosen classifier contents into the diagram_bounds area of the cairo drawing context
 *
 *  \param this_ pointer to own object attributes
 *  \param layouted_classifier pointer to the layout-information and data to be drawn
 *  \param mark_focused true if the object is to be marked as "focused"
 *  \param mark_highlighted true if the object is to be marked as "highlighted"
 *  \param mark_selected true if the object is to be marked as "selected"
 *  \param pencil_size set of sizes and colors for drawing lines and text
 *  \param font_layout structure to layout fonts
 *  \param cr a cairo drawing context
 */
void pencil_classifier_painter_draw ( const pencil_classifier_painter_t *this_,
                                      layout_visible_classifier_t *layouted_classifier,
                                      data_id_t mark_focused,
                                      data_id_t mark_highlighted,
                                      const data_small_set_t *mark_selected,
                                      const pencil_size_t *pencil_size,
                                      PangoLayout *font_layout,
                                      cairo_t *cr
                                    );

/*!
 *  \brief determines the minumum classifier bounds
 *
 *  without contained features, without contained classifiers.
 *
 *  \param this_ pointer to own object attributes
 *  \param visible_classifier pointer to the data to be drawn
 *  \param pencil_size set of sizes and colors for drawing lines and text
 *  \param font_layout pango layout object to determine the font metrics in the current cairo drawing context
 *  \param out_classifier_bounds memory location where the result shall be stored. Must not be NULL.
 *  \param out_classifier_space memory location where the drawing space corresponding to the result bounds shall be stored. Must not be NULL.
 */
void pencil_classifier_painter_get_minimum_bounds ( const pencil_classifier_painter_t *this_,
                                                    const data_visible_classifier_t *visible_classifier,
                                                    const pencil_size_t *pencil_size,
                                                    PangoLayout *font_layout,
                                                    geometry_rectangle_t *out_classifier_bounds,
                                                    geometry_rectangle_t *out_classifier_space
                                                  );

/* !
 *  \brief determines the inner drawing space for contained classifiers and features
 *
 *  \param this_ pointer to own object attributes
 *  \param visible_classifier pointer to the data to be drawn
 *  \param pencil_size set of sizes and colors for drawing lines and text
 *  \param classifier_bounds bounds of the classifier
 *  \param font_layout pango layout object to determine the font metrics in the current cairo drawing context
 *  \param out_classifier_space memory location where the result shall be stored. Must not be NULL.
 */
void pencil_classifier_painter_get_drawing_space ( const pencil_classifier_painter_t *this_,
                                                   const data_visible_classifier_t *visible_classifier,
                                                   const pencil_size_t *pencil_size,
                                                   const geometry_rectangle_t *classifier_bounds,
                                                   PangoLayout *font_layout,
                                                   geometry_rectangle_t *out_classifier_space
                                                 );

/*!
 *  \brief draws the component icon at the defined location
 *
 *  \param this_ pointer to own object attributes
 *  \param x x location where to draw the icon
 *  \param y y location where to draw the icon
 *  \param h_align alignment of the icon versus the given x coordinate
 *  \param v_align alignment of the icon versus the given y coordinate
 *  \param height size of the icon
 *  \param cr a cairo drawing context
 *  \param out_width width of the drawn icon to be returned. NULL is not allowed.
 */
void pencil_classifier_painter_private_draw_component_icon ( const pencil_classifier_painter_t *this_,
                                                             double x,
                                                             double y,
                                                             geometry_h_align_t h_align,
                                                             geometry_v_align_t v_align,
                                                             double height,
                                                             cairo_t *cr,
                                                             double *out_width
                                                           );

/*!
 *  \brief draws the artifact icon at the defined location
 *
 *  \param this_ pointer to own object attributes
 *  \param x x location where to draw the icon
 *  \param y y location where to draw the icon
 *  \param h_align alignment of the icon versus the given x coordinate
 *  \param v_align alignment of the icon versus the given y coordinate
 *  \param height size of the icon
 *  \param cr a cairo drawing context
 *  \param out_width width of the drawn icon to be returned. NULL is not allowed.
 */
void pencil_classifier_painter_private_draw_artifact_icon ( const pencil_classifier_painter_t *this_,
                                                            double x,
                                                            double y,
                                                            geometry_h_align_t h_align,
                                                            geometry_v_align_t v_align,
                                                            double height,
                                                            cairo_t *cr,
                                                            double *out_width
                                                          );

/*!
 *  \brief draws the actor icon at the defined location
 *
 *  \param this_ pointer to own object attributes
 *  \param x x location where to draw the icon
 *  \param y y location where to draw the icon
 *  \param h_align alignment of the icon versus the given x coordinate
 *  \param v_align alignment of the icon versus the given y coordinate
 *  \param height size of the icon
 *  \param cr a cairo drawing context
 *  \param out_width width of the drawn icon to be returned. NULL is not allowed.
 */
void pencil_classifier_painter_private_draw_actor_icon ( const pencil_classifier_painter_t *this_,
                                                         double x,
                                                         double y,
                                                         geometry_h_align_t h_align,
                                                         geometry_v_align_t v_align,
                                                         double height,
                                                         cairo_t *cr,
                                                         double *out_width
                                                       );

/*!
 *  \brief determines the dimensions of the border of the classifier-shape.
 *
 *  E.g. a simple line border is 2.0 * pencil_size_get_standard_object_border()
 *
 *  \param this_ pointer to own object attributes
 *  \param classifier_type type of the classifier to draw
 *  \param pencil_size set of sizes and colors for drawing lines and text
 *  \param out_top_border height of the top border is returned. NULL is not allowed.
 *  \param out_left_border width of the left border is returned. NULL is not allowed.
 *  \param out_bottom_border height of the bottom border is returned. NULL is not allowed.
 *  \param out_right_border width of the right border is returned. NULL is not allowed.
 */
static inline void pencil_classifier_painter_private_get_shape_border_dimensions( const pencil_classifier_painter_t *this_,
                                                                                  data_classifier_type_t classifier_type,
                                                                                  const pencil_size_t *pencil_size,
                                                                                  double *out_top_border,
                                                                                  double *out_left_border,
                                                                                  double *out_bottom_border,
                                                                                  double *out_right_border
                                                                                );

/*!
 *  \brief determines the dimensions of the stereotype and name of the classifier.
 *
 *  \param this_ pointer to own object attributes
 *  \param visible_classifier the visible_classifier consisting of diagramelement and classifier to draw
 *  \param pencil_size set of sizes and colors for drawing lines and text
 *  \param font_layout pango layout object to determine the font metrics in the current cairo drawing context
 *  \param out_text_height height of the text is returned. NULL is not allowed.
 *  \param out_text_width width of the text is returned. NULL is not allowed.
 */
static inline void pencil_classifier_painter_private_get_stereotype_and_name_dimensions( const pencil_classifier_painter_t *this_,
                                                                                         const data_visible_classifier_t *visible_classifier,
                                                                                         const pencil_size_t *pencil_size,
                                                                                         PangoLayout *font_layout,
                                                                                         double *out_text_height,
                                                                                         double *out_text_width
                                                                                       );

#include "pencil_classifier_painter.inl"

#endif  /* PENCIL_CLASSIFIER_PAINTER_H */


/*
Copyright 2016-2018 Andreas Warnke

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
