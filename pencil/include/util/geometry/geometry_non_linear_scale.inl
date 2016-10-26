/* File: geometry_non_linear_scale.inl; Copyright and License: see below */

#include "trace.h"

static inline void geometry_non_linear_scale_reinit ( geometry_non_linear_scale_t *this_, double lower_bound, double upper_bound )
{
    geometry_non_linear_scale_init( this_, lower_bound, upper_bound );
}

static inline void geometry_non_linear_scale_destroy ( geometry_non_linear_scale_t *this_ )
{
}

static inline double geometry_non_linear_scale_get_location ( geometry_non_linear_scale_t *this_, int32_t order )
{
    double result = 0.0;

    return result;
}

static inline int32_t geometry_non_linear_scale_get_order ( geometry_non_linear_scale_t *this_, double location )
{
    int32_t result = 0;

    return result;
}


/*
Copyright 2016-2016 Andreas Warnke

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
