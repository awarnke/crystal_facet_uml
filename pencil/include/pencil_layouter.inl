/* File: pencil_layouter.inl; Copyright and License: see below */

#include "tslog.h"
#include <assert.h>

static inline pencil_input_data_layout_t *pencil_layouter_get_layout_data_ptr ( pencil_layouter_t *this_ )
{
    return &((*this_).layout_data);
}

static inline pencil_size_t *pencil_layouter_get_pencil_size_ptr ( pencil_layouter_t *this_ )
{
    return &((*this_).pencil_size);
}

static inline geometry_rectangle_t *pencil_layouter_get_diagram_bounds_ptr ( pencil_layouter_t *this_ )
{
    return &((*this_).diagram_bounds);
}

static inline universal_int32_pair_t pencil_layouter_get_order_at_pos ( pencil_layouter_t *this_,
                                                                        double x,
                                                                        double y )
{
    universal_int32_pair_t result;
    int32_t x_order = geometry_non_linear_scale_get_order( &((*this_).x_scale), x, 3.0 );
    int32_t y_order = geometry_non_linear_scale_get_order( &((*this_).y_scale), y, 3.0 );
    universal_int32_pair_init( &result, x_order, y_order );

    return result;
}

static inline universal_bool_list_t pencil_layouter_is_pos_on_grid ( pencil_layouter_t *this_,
                                                                     double x,
                                                                     double y )
{
    universal_bool_list_t result;
    double x_dist = geometry_non_linear_scale_get_closest_fix_location( &((*this_).x_scale), x ) - x;
    double y_dist = geometry_non_linear_scale_get_closest_fix_location( &((*this_).y_scale), y ) - y;
    universal_bool_list_init_pair( &result, ((-3.0 < x_dist)&&( x_dist < 3.0)), ((-3.0 < y_dist)&&( y_dist < 3.0)) );

    return result;
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