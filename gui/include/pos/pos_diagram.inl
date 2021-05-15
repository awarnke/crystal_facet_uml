/* File: pos_diagram.inl; Copyright and License: see below */

#include <assert.h>

static inline void pos_diagram_init ( pos_diagram_t *this_, const data_diagram_t *diagram_data )
{
    assert ( NULL != diagram_data );

    shape_int_rectangle_init_empty( &((*this_).icon_box) );
    shape_int_rectangle_init_empty( &((*this_).label_box) );
    (*this_).data = diagram_data;
}

static inline void pos_diagram_destroy ( pos_diagram_t *this_ )
{
    shape_int_rectangle_destroy( &((*this_).icon_box) );
    shape_int_rectangle_destroy( &((*this_).label_box) );
    (*this_).data = NULL;
}

static inline bool pos_diagram_is_valid ( const pos_diagram_t *this_ )
{
    bool result;
    if ( (*this_).data == NULL )
    {
        assert(false);
        result = false;  /* cannot happen on initialized objects */
    }
    else
    {
        result = data_diagram_is_valid( (*this_).data );
    }
    return result;
}

static inline shape_int_rectangle_t *pos_diagram_get_icon_box_ptr ( pos_diagram_t *this_ )
{
    return &((*this_).icon_box);
}

static inline const shape_int_rectangle_t *pos_diagram_get_icon_box_const ( const pos_diagram_t *this_ )
{
    return &((*this_).icon_box);
}

static inline void pos_diagram_set_icon_box ( pos_diagram_t *this_, const shape_int_rectangle_t *icon_box )
{
    shape_int_rectangle_replace( &((*this_).icon_box), icon_box );
}

static inline shape_int_rectangle_t *pos_diagram_get_label_box_ptr ( pos_diagram_t *this_ )
{
    return &((*this_).label_box);
}

static inline const shape_int_rectangle_t *pos_diagram_get_label_box_const ( const pos_diagram_t *this_ )
{
    return &((*this_).label_box);
}

static inline void pos_diagram_set_label_box ( pos_diagram_t *this_, const shape_int_rectangle_t *label_box )
{
    shape_int_rectangle_replace( &((*this_).label_box), label_box );
}

static inline const data_diagram_t *pos_diagram_get_data_const ( const pos_diagram_t *this_ )
{
    assert ( NULL != (*this_).data );
    return (*this_).data;
}

static inline data_row_id_t pos_diagram_get_diagram_id ( const pos_diagram_t *this_ )
{
    return data_diagram_get_row_id( (*this_).data );
}


/*
Copyright 2021-2021 Andreas Warnke

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
