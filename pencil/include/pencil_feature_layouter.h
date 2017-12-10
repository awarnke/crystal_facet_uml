/* File: pencil_feature_layouter.h; Copyright and License: see below */

#ifndef PENCIL_FEATURE_LAYOUTER_H
#define PENCIL_FEATURE_LAYOUTER_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Calculates positions of classifiers, features and relationships in a diagram
 */

#include "pencil_input_data.h"
#include "pencil_size.h"
#include "pencil_input_data_layout.h"
#include "pencil_feature_painter.h"
#include "util/geometry/geometry_rectangle.h"
#include "util/geometry/geometry_non_linear_scale.h"
#include "data_diagram.h"
#include "data_small_set.h"
#include "data_id.h"
#include "universal_int32_pair.h"
#include "universal_bool_list.h"
#include "universal_array_index_sorter.h"
#include <cairo.h>
#include <stdint.h>

/*!
 *  \brief attributes of the feature layouter
 */
struct pencil_feature_layouter_struct {
    pencil_input_data_t *input_data;  /*!< pointer to an external data cache */
    pencil_input_data_layout_t *layout_data;  /* pointer to an external layout data */

    pencil_size_t *pencil_size;  /*!< pointer to an external pencil_size_t object, defining pen sizes, gap sizes, font sizes and colors */

    pencil_feature_painter_t feature_painter;  /*!< own instance of a painter object to ask for display dimensions */
};

typedef struct pencil_feature_layouter_struct pencil_feature_layouter_t;

/*!
 *  \brief initializes the feature layouter
 *
 *  \param this_ pointer to own object attributes
 *  \param input_data pointer to the (cached) data to be layouted
 *  \param layout_data pointer to the layout information to be used and modified
 *  \param pencil_size pointer to the pencil_size_t object
 */
void pencil_feature_layouter_init( pencil_feature_layouter_t *this_,
                                   pencil_input_data_t *input_data,
                                   pencil_input_data_layout_t *layout_data,
                                   pencil_size_t *pencil_size
                                 );

/*!
 *  \brief destroys the feature layouter
 *
 *  \param this_ pointer to own object attributes
 */
void pencil_feature_layouter_destroy( pencil_feature_layouter_t *this_ );

/*!
 *  \brief calculates feature bounds of all features for one classifier
 *
 *  \param this_ pointer to own object attributes
 *  \param classifier_id id of the classifier for which to calculate the feature bounds
 *  \param font_layout pango layout object to determine the font metrics in the current cairo drawing context
 *  \param out_features_bounds memory location where the result shall be stored. Must not be NULL.
 */
void pencil_feature_layouter_calculate_features_bounds ( pencil_feature_layouter_t *this_,
                                                         int64_t classifier_id,
                                                         PangoLayout *font_layout,
                                                         geometry_rectangle_t *out_features_bounds
                                                       );

#endif  /* PENCIL_FEATURE_LAYOUTER_H */


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