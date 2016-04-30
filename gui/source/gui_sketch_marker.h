/* File: gui_sketch_marker.h; Copyright and License: see below */

#ifndef GUI_SKETCH_MARKER_H
#define GUI_SKETCH_MARKER_H

/* public file for the doxygen documentation: */
/*! \file
 *  \brief References objects that shall be marked (displayed as selected/focused/highlighted)
 */

#include "data_small_set.h"
#include <stdbool.h>
#include <stdint.h>

/*!
 *  \brief attributes of the sketch marker
 */
struct gui_sketch_marker_struct {
    data_id_t focused;  /*!<  references the one focused object */
    data_id_t highlighted;  /*!<  references the one highlighted/mouse over object */
    data_small_set_t selected_set;  /*!<  references all selected objects */
};

typedef struct gui_sketch_marker_struct gui_sketch_marker_t;

/*!
 *  \brief initializes the gui_sketch_marker_t struct
 *
 *  \param this_ pointer to own object attributes
 */
static inline void gui_sketch_marker_init ( gui_sketch_marker_t *this_ );

/*!
 *  \brief destroys the gui_sketch_marker_t struct
 *
 *  \param this_ pointer to own object attributes
 */
static inline void gui_sketch_marker_destroy ( gui_sketch_marker_t *this_ );

/*!
 *  \brief gets the focused object id
 *
 *  \param this_ pointer to own object attributes
 */
static inline data_id_t gui_sketch_marker_get_focused ( gui_sketch_marker_t *this_ );

/*!
 *  \brief gets the highlighted object id
 *
 *  \param this_ pointer to own object attributes
 */
static inline data_id_t gui_sketch_marker_get_highlighted ( gui_sketch_marker_t *this_ );

/*!
 *  \brief gets the selected object ids.
 *
 *  Note: you may modify the set after retrieving the pointer to it.
 *
 *  \param this_ pointer to own object attributes
 */
static inline data_small_set_t *gui_sketch_marker_get_selected_set_ptr ( gui_sketch_marker_t *this_ );

/*!
 *  \brief sets the focused object id
 *
 *  \param this_ pointer to own object attributes
 *  \param row_id the id to set as focused
 */
static inline void gui_sketch_marker_set_focused ( gui_sketch_marker_t *this_, data_id_t row_id );

/*!
 *  \brief sets the highlighted object id
 *
 *  \param this_ pointer to own object attributes
 *  \param row_id the id to set as highlighted
 */
static inline void gui_sketch_marker_set_highlighted ( gui_sketch_marker_t *this_, data_id_t row_id );

/*!
 *  \brief un-sets the focused object id
 *
 *  \param this_ pointer to own object attributes
 */
static inline void gui_sketch_marker_clear_focused ( gui_sketch_marker_t *this_ );

/*!
 *  \brief un-sets the highlighted object id
 *
 *  \param this_ pointer to own object attributes
 */
static inline void gui_sketch_marker_clear_highlighted ( gui_sketch_marker_t *this_ );

#include "gui_sketch_marker.inl"

#endif  /* GUI_SKETCH_MARKER_H */


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