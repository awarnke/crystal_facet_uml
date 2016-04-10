/* File: shape_int_rectangle.inl; Copyright and License: see below */

#include "trace.h"

static inline void shape_int_rectangle_init ( shape_int_rectangle_t *this_, int32_t left, int32_t top, uint32_t width, uint32_t height )
{
    (*this_).left = left;
    (*this_).top = top;
    (*this_).width = width;
    (*this_).height = height;
}

static inline void shape_int_rectangle_destroy ( shape_int_rectangle_t *this_ )
{
}

static inline int32_t shape_int_rectangle_get_left ( shape_int_rectangle_t *this_ )
{
    return (*this_).left;
}

static inline int32_t shape_int_rectangle_get_top ( shape_int_rectangle_t *this_ )
{
    return (*this_).top;
}

static inline int32_t shape_int_rectangle_get_right ( shape_int_rectangle_t *this_ )
{
    return (*this_).left + (*this_).width;
}

static inline int32_t shape_int_rectangle_get_bottom ( shape_int_rectangle_t *this_ )
{
    return (*this_).top + (*this_).height;
}

static inline uint32_t shape_int_rectangle_get_width ( shape_int_rectangle_t *this_ )
{
    return (*this_).width;
}

static inline uint32_t shape_int_rectangle_get_height ( shape_int_rectangle_t *this_ )
{
    return (*this_).height;
}

static inline bool shape_int_rectangle_contains ( shape_int_rectangle_t *this_, int32_t x, int32_t y )
{
    bool result;
    result = ( x >= (*this_).left )&&( y >= (*this_).top )&&( x < (*this_).left+(*this_).width )&&( y < (*this_).top+(*this_).height );
    return result;
}

static inline void shape_int_rectangle_shrink_by_border ( shape_int_rectangle_t *this_, int32_t border )
{
    int32_t double_border;
    double_border = 2 * border;
    if ( double_border > (*this_).width )
    {
        (*this_).left += ((*this_).width/2);  /* take the h-center of the old rectangle */
        (*this_).width = 0;
    }
    else
    {
        (*this_).left += border;
        (*this_).width -= double_border;
    }
    if ( double_border > (*this_).height )
    {
        (*this_).top += ((*this_).height/2);  /* take the v-middle of the old rectangle */
        (*this_).height = 0;
    }
    else
    {
        (*this_).top += border;
        (*this_).height -= double_border;
    }
}

static inline void shape_int_rectangle_shrink_to_ratio ( shape_int_rectangle_t *this_, uint32_t ratio_width, uint32_t ratio_height, shape_alignment_t align )
{
    if ( (*this_).width * ratio_height == (*this_).height * ratio_width )
    {
        /* nothing to do - and the case for the div by 0 problem of ratio_width==0 and ratio_height==0 */
    }
    else if ( (*this_).width * ratio_height > (*this_).height * ratio_width )
    {
        /* the rectangle needs to shrink at left and/or right */
        uint32_t new_width;
        new_width = ( (*this_).height * ratio_width ) / ratio_height;
        switch ( align & SHAPE_ALIGNMENT_HORIZONTAL_MASK )
        {
            case SHAPE_ALIGNMENT_HORIZONTAL_LEFT:
                break;
            case SHAPE_ALIGNMENT_HORIZONTAL_RIGHT:
                (*this_).left += ((*this_).width - new_width);
                break;
            case SHAPE_ALIGNMENT_HORIZONTAL_UNSPECIFIED: /* and */
            case SHAPE_ALIGNMENT_HORIZONTAL_CENTER: /* and */
            default:
                (*this_).left += ( ((*this_).width - new_width)/2 );
        }
        (*this_).width = new_width;
    }
    else
    {
        /* the rectangle needs to shrink at top and/or bottom */
        uint32_t new_height;
        new_height = ( (*this_).width * ratio_height ) / ratio_width;
        switch ( align & SHAPE_ALIGNMENT_VERTICAL_MASK )
        {
            case SHAPE_ALIGNMENT_VERTICAL_TOP:
                break;
            case SHAPE_ALIGNMENT_VERTICAL_BOTTOM:
                (*this_).top += ((*this_).height - new_height);
                break;
            case SHAPE_ALIGNMENT_VERTICAL_UNSPECIFIED: /* and */
            case SHAPE_ALIGNMENT_VERTICAL_MIDDLE: /* and */
            default:
                (*this_).top += ( ((*this_).height - new_height)/2 );
        }
        (*this_).height = new_height;
    }
}

static inline void shape_int_rectangle_trace ( shape_int_rectangle_t *this_ )
{
    TRACE_INFO( "shape_int_rectangle_t" );
    TRACE_INFO_INT( "- left:", (*this_).left );
    TRACE_INFO_INT( "- top:", (*this_).top );
    TRACE_INFO_INT( "- width:", (*this_).width );
    TRACE_INFO_INT( "- height:", (*this_).height );
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