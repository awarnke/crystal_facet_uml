/* File: pencil_input_data_layout.h; Copyright and License: see below */

#ifndef PENCIL_INPUT_DATA_LAYOUT_H
#define PENCIL_INPUT_DATA_LAYOUT_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Stores the layout data for input data.
 */

#include "util/geometry/geometry_connector.h"
#include "util/geometry/geometry_rectangle.h"
#include "layout/layout_feature.h"
#include "layout/layout_relationship.h"
#include "layout/layout_visible_classifier.h"
#include "pencil_input_data.h"
#include "option/pencil_visibility.h"
#include <cairo.h>
#include <stdint.h>
#include <stdbool.h>

/*!
 *  \brief constants for maximum values of pencil_input_data_layout_t
 */
enum pencil_input_data_layout_max_enum {
    PENCIL_INPUT_DATA_LAYOUT_MAX_CLASSIFIERS = PENCIL_INPUT_DATA_MAX_CLASSIFIERS,  /*!< maximum number of classifiers to be shown in one single diagram */
    PENCIL_INPUT_DATA_LAYOUT_MAX_FEATURES = PENCIL_INPUT_DATA_MAX_FEATURES,  /*!< maximum number of features to be shown in one single diagram */
    PENCIL_INPUT_DATA_LAYOUT_MAX_RELATIONSHIPS = PENCIL_INPUT_DATA_MAX_RELATIONSHIPS,  /*!< maximum number of relationships to be shown in one single diagram */
};

/*!
 *  \brief attributes of the pencil_input_data_layout_t
 */
struct pencil_input_data_layout_struct {
    /* classifier layout*/
    layout_visible_classifier_t visible_classifier_layout[PENCIL_INPUT_DATA_LAYOUT_MAX_CLASSIFIERS];  /*!< layout data of visible classifiers */
    uint32_t visible_classifier_count;  /*!< number of all layouted visible classifier records */

    /* feature layout */
    layout_feature_t feature_layout[PENCIL_INPUT_DATA_LAYOUT_MAX_FEATURES];  /*!< layout data of features */
    uint32_t feature_count;  /*!< number of all layouted feature records */

    /* relationship layout */
    layout_relationship_t relationship_layout[PENCIL_INPUT_DATA_LAYOUT_MAX_RELATIONSHIPS];  /*!< layout data of relationships */
    uint32_t relationship_count;  /*!< number of all layouted relationship records */
};

typedef struct pencil_input_data_layout_struct pencil_input_data_layout_t;

/*!
 *  \brief initializes the painter input data layout
 *
 *  \param this_ pointer to own object attributes
 */
void pencil_input_data_layout_init( pencil_input_data_layout_t *this_ );

/*!
 *  \brief re-initializes the painter input data layout
 *
 *  \param this_ pointer to own object attributes
 *  \param input_data pointer to the (cached) data to be layouted
 */
void pencil_input_data_layout_reinit( pencil_input_data_layout_t *this_, pencil_input_data_t *input_data );

/*!
 *  \brief destroys the painter input data layout
 *
 *  \param this_ pointer to own object attributes
 */
void pencil_input_data_layout_destroy( pencil_input_data_layout_t *this_ );

/* ================================ classifiers ================================ */

/*!
 *  \brief gets the outer geometry of a classifier
 *
 *  \param this_ pointer to own object attributes
 *  \param index index of the classifier bounds to retrieve; 0 <= index < PENCIL_INPUT_DATA_LAYOUT_MAX_CLASSIFIERS.
 *  \return pointer to geometry_rectangle_t.
 */
static inline geometry_rectangle_t *pencil_input_data_layout_get_classifier_bounds_ptr ( pencil_input_data_layout_t *this_, uint32_t index );

/*!
 *  \brief gets the inner space of a classifier
 *
 *  \param this_ pointer to own object attributes
 *  \param index index of the classifier space to retrieve; 0 <= index < PENCIL_INPUT_DATA_LAYOUT_MAX_CLASSIFIERS.
 *  \return pointer to geometry_rectangle_t.
 */
static inline geometry_rectangle_t *pencil_input_data_layout_get_classifier_space_ptr ( pencil_input_data_layout_t *this_, uint32_t index );

/*!
 *  \brief gets the number of visible classifiers within the painter layout data
 *
 *  \param this_ pointer to own object attributes
 */
static inline uint32_t pencil_input_data_layout_get_visible_classifier_count ( pencil_input_data_layout_t *this_ );

/* ================================ features ================================ */

/*!
 *  \brief gets the bounding box of a feature
 *
 *  \param this_ pointer to own object attributes
 *  \param index index of the featore bounds to retrieve; 0 <= index < PENCIL_INPUT_DATA_LAYOUT_MAX_FEATURES.
 *  \return pointer to geometry_rectangle_t.
 */
static inline geometry_rectangle_t *pencil_input_data_layout_get_feature_bounds_ptr ( pencil_input_data_layout_t *this_, uint32_t index );

/*!
 *  \brief gets the number of features within the painter layout data
 *
 *  \param this_ pointer to own object attributes
 */
static inline uint32_t pencil_input_data_layout_get_feature_count ( pencil_input_data_layout_t *this_ );

/* ================================ relationships ================================ */

/*!
 *  \brief gets the visibility of a relationship
 *
 *  \param this_ pointer to own object attributes
 *  \param index index of the relationship visibility to retrieve; 0 <= index < PENCIL_INPUT_DATA_LAYOUT_MAX_RELATIONSHIPS.
 *  \return true if visible.
 */
static inline pencil_visibility_t pencil_input_data_layout_get_relationship_visibility ( pencil_input_data_layout_t *this_, uint32_t index );

/*!
 *  \brief sets the visibility of a relationship
 *
 *  \param this_ pointer to own object attributes
 *  \param index index of the relationship visibility to set; 0 <= index < PENCIL_INPUT_DATA_LAYOUT_MAX_RELATIONSHIPS.
 *  \param visible true if visible.
 */
static inline void pencil_input_data_layout_set_relationship_visibility ( pencil_input_data_layout_t *this_, uint32_t index, pencil_visibility_t visible );

/*!
 *  \brief gets the geometry of a relationship
 *
 *  \param this_ pointer to own object attributes
 *  \param index index of the relationship shape to retrieve; 0 <= index < PENCIL_INPUT_DATA_LAYOUT_MAX_RELATIONSHIPS.
 *  \return pointer to geometry_rectangle_t.
 */
static inline geometry_connector_t *pencil_input_data_layout_get_relationship_shape_ptr ( pencil_input_data_layout_t *this_, uint32_t index );

/*!
 *  \brief gets the number of relationships within the painter layout data
 *
 *  \param this_ pointer to own object attributes
 */
static inline uint32_t pencil_input_data_layout_get_relationship_count ( pencil_input_data_layout_t *this_ );

#include "pencil_input_data_layout.inl"

#endif  /* PENCIL_INPUT_DATA_LAYOUT_H */


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
