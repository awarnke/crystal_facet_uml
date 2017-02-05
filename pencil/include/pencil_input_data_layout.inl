/* File: pencil_input_data_layout.inl; Copyright and License: see below */

#include "tslog.h"
#include <assert.h>

static inline void pencil_input_data_layout_init( pencil_input_data_layout_t *this_ )
{
    for ( int idx = 0; idx < PENCIL_INPUT_DATA_LAYOUT_MAX_CLASSIFIERS; idx ++ )
    {
        geometry_rectangle_init_empty( &((*this_).classifier_bounds[idx]) );
    }
    for ( int idx = 0; idx < PENCIL_INPUT_DATA_LAYOUT_MAX_CLASSIFIERS; idx ++ )
    {
        geometry_rectangle_init_empty( &((*this_).classifier_space[idx]) );
    }
    for ( int idx = 0; idx < PENCIL_INPUT_DATA_LAYOUT_MAX_FEATURES; idx ++ )
    {
        geometry_rectangle_init_empty( &((*this_).feature_bounds[idx]) );
    }
    for ( int idx = 0; idx < PENCIL_INPUT_DATA_LAYOUT_MAX_RELATIONSHIPS; idx ++ )
    {
        geometry_connector_init_empty( &((*this_).relationship_shape[idx]) );
    }
}

static inline void pencil_input_data_layout_destroy( pencil_input_data_layout_t *this_ )
{
    for ( int idx = 0; idx < PENCIL_INPUT_DATA_LAYOUT_MAX_CLASSIFIERS; idx ++ )
    {
        geometry_rectangle_destroy( &((*this_).classifier_bounds[idx]) );
    }
    for ( int idx = 0; idx < PENCIL_INPUT_DATA_LAYOUT_MAX_CLASSIFIERS; idx ++ )
    {
        geometry_rectangle_destroy( &((*this_).classifier_space[idx]) );
    }
    for ( int idx = 0; idx < PENCIL_INPUT_DATA_LAYOUT_MAX_FEATURES; idx ++ )
    {
        geometry_rectangle_destroy( &((*this_).feature_bounds[idx]) );
    }
    for ( int idx = 0; idx < PENCIL_INPUT_DATA_LAYOUT_MAX_RELATIONSHIPS; idx ++ )
    {
        geometry_connector_destroy( &((*this_).relationship_shape[idx]) );
    }
}

static inline geometry_rectangle_t *pencil_input_data_layout_get_classifier_bounds_ptr ( pencil_input_data_layout_t *this_, uint32_t index )
{
    assert( index < PENCIL_INPUT_DATA_LAYOUT_MAX_CLASSIFIERS );
    return &((*this_).classifier_bounds[index]);
}

static inline geometry_rectangle_t *pencil_input_data_layout_get_classifier_space_ptr ( pencil_input_data_layout_t *this_, uint32_t index )
{
    assert( index < PENCIL_INPUT_DATA_LAYOUT_MAX_CLASSIFIERS );
    return &((*this_).classifier_space[index]);
}

static inline geometry_rectangle_t *pencil_input_data_layout_get_feature_bounds_ptr ( pencil_input_data_layout_t *this_, uint32_t index )
{
    assert( index < PENCIL_INPUT_DATA_LAYOUT_MAX_FEATURES );
    return &((*this_).feature_bounds[index]);
}

static inline geometry_connector_t *pencil_input_data_layout_get_relationship_shape_ptr ( pencil_input_data_layout_t *this_, uint32_t index )
{
    assert( index < PENCIL_INPUT_DATA_LAYOUT_MAX_RELATIONSHIPS );
    return &((*this_).relationship_shape[index]);
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