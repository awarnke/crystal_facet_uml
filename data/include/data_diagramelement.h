/* File: data_diagramelement.h; Copyright and License: see below */

#ifndef DATA_DIAGRAMELEMENT_H
#define DATA_DIAGRAMELEMENT_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Defines in which diagram which classifier is visible
 */

#include "data_diagramelement_flag.h"
#include <stdint.h>
#include <stdbool.h>

/*!
 *  \brief all data attributes needed for the diagramelement functions
 */
struct data_diagramelement_struct {
    int64_t id;  /*!< id of the diagramelement_t relation */
    int64_t diagram_id;  /*!< id of the referenced diagram_t */
    int64_t classifier_id;  /*!< id of the referenced classifier_t */
    data_diagramelement_flag_t display_flags;  /*!< flags that influence the way how the classifier is displayed in the diagram. \see data_diagramelement_display_flag_enum */
    int64_t focused_feature_id;  /*!< focused_feature_id is optional; -1 if not used */
};

typedef struct data_diagramelement_struct data_diagramelement_t;

/*!
 *  \brief initializes the data_diagramelement_t struct with id, classifier_id and diagram_id DATA_ID_VOID_ID.
 *
 *  \param this_ pointer to own object attributes
 */
static inline void data_diagramelement_init_empty ( data_diagramelement_t *this_ );

/*!
 *  \brief re-initializes the data_diagramelement_t struct with id, classifier_id and diagram_id DATA_ID_VOID_ID.
 *
 *  \param this_ pointer to own object attributes
 */
static inline void data_diagramelement_reinit_empty ( data_diagramelement_t *this_ );

/*!
 *  \brief initializes the data_diagramelement_t struct
 *
 *  id is initialized to DATA_ID_VOID_ID;
 *
 *  \param this_ pointer to own object attributes
 *  \param diagram_id id of the referenced diagram_t
 *  \param classifier_id id of the referenced classifier_t
 *  \param display_flags flags how to display the classifier in the context of this diagram. \see data_diagramelement_display_flag_enum
 *  \param focused_feature_id id of the focused feature_t or DATA_ID_VOID_ID if no feature is focused
 */
static inline void data_diagramelement_init_new ( data_diagramelement_t *this_,
                                                  int64_t diagram_id,
                                                  int64_t classifier_id,
                                                  data_diagramelement_flag_t display_flags,
                                                  int64_t focused_feature_id
                                                );

/*!
 *  \brief initializes the data_diagramelement_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \param id id of the data_diagramelement_t
 *  \param diagram_id id of the referenced diagram_t
 *  \param classifier_id id of the referenced classifier_t
 *  \param display_flags flags how to display the classifier in the context of this diagram. \see data_diagramelement_display_flag_enum
 *  \param focused_feature_id id of the focused feature_t or DATA_ID_VOID_ID if no feature is focused
 */
static inline void data_diagramelement_init ( data_diagramelement_t *this_,
                                              int64_t id,
                                              int64_t diagram_id,
                                              int64_t classifier_id,
                                              data_diagramelement_flag_t display_flags,
                                              int64_t focused_feature_id
                                            );

/*!
 *  \brief re-initializes the data_diagramelement_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \param id id of the data_diagramelement_t
 *  \param diagram_id id of the referenced diagram_t
 *  \param classifier_id id of the referenced classifier_t
 *  \param display_flags flags how to display the classifier in the context of this diagram. \see data_diagramelement_display_flag_enum
 *  \param focused_feature_id id of the focused feature_t or DATA_ID_VOID_ID if no feature is focused
 */
static inline void data_diagramelement_reinit ( data_diagramelement_t *this_,
                                                int64_t id,
                                                int64_t diagram_id,
                                                int64_t classifier_id,
                                                data_diagramelement_flag_t display_flags,
                                                int64_t focused_feature_id
                                              );

/*!
 *  \brief initializes the data_diagramelement_t struct with a copy
 *
 *  \param this_ pointer to own object attributes
 *  \param original pointer to object attributes to copy
 */
static inline void data_diagramelement_copy ( data_diagramelement_t *this_, const data_diagramelement_t *original );

/*!
 *  \brief overwrites the data_diagramelement_t struct with a copy
 *
 *  \param this_ pointer to own object attributes
 *  \param that pointer to object attributes to copy
 */
static inline void data_diagramelement_replace ( data_diagramelement_t *this_, const data_diagramelement_t *that );

/*!
 *  \brief destroys the data_diagramelement_t struct
 *
 *  \param this_ pointer to own object attributes
 */
static inline void data_diagramelement_destroy ( data_diagramelement_t *this_ );

/*!
 *  \brief checks if attribute id is not DATA_ID_VOID_ID
 *
 *  \param this_ pointer to own object attributes
 *  \return true if the id of the diagramelement is not DATA_ID_VOID_ID
 */
static inline bool data_diagramelement_is_valid ( const data_diagramelement_t *this_ );

/*!
 *  \brief prints the data_diagramelement_t struct to the trace output
 *
 *  \param this_ pointer to own object attributes
 */
static inline void data_diagramelement_trace ( const data_diagramelement_t *this_ );

/*!
 *  \brief gets the attribute id
 *
 *  \param this_ pointer to own object attributes
 *  \return attribute id, DATA_ID_VOID_ID if this_ is not valid
 */
static inline int64_t data_diagramelement_get_id ( const data_diagramelement_t *this_ );

/*!
 *  \brief sets the attribute id
 *
 *  \param this_ pointer to own object attributes
 *  \param id new id of this object
 */
static inline void data_diagramelement_set_id ( data_diagramelement_t *this_, int64_t id );

/*!
 *  \brief gets the attribute diagram_id
 *
 *  \param this_ pointer to own object attributes
 *  \return attribute diagram_id, DATA_ID_VOID_ID if the referenced diagram is not valid
 */
static inline int64_t data_diagramelement_get_diagram_id ( const data_diagramelement_t *this_ );

/*!
 *  \brief gets the attribute classifier_id
 *
 *  \param this_ pointer to own object attributes
 *  \return attribute classifier_id, DATA_ID_VOID_ID if the referenced classifier is not valid
 */
static inline int64_t data_diagramelement_get_classifier_id ( const data_diagramelement_t *this_ );

/*!
 *  \brief gets the attribute display_flags
 *
 *  \param this_ pointer to own object attributes
 *  \return attribute display_flags
 */
static inline data_diagramelement_flag_t data_diagramelement_get_display_flags ( const data_diagramelement_t *this_ );

/*!
 *  \brief sets the attribute display_flags
 *
 *  \param this_ pointer to own object attributes
 *  \param display_flags new attribute display_flags
 */
static inline void data_diagramelement_set_display_flags ( data_diagramelement_t *this_, data_diagramelement_flag_t display_flags );

/*!
 *  \brief gets the attribute focused_feature_id
 *
 *  \param this_ pointer to own object attributes
 *  \return attribute focused_feature_id, DATA_ID_VOID_ID if there is no focused feature
 */
static inline int64_t data_diagramelement_get_focused_feature_id ( const data_diagramelement_t *this_ );

/*!
 *  \brief sets the attribute focused_feature_id
 *
 *  \param this_ pointer to own object attributes
 *  \param focused_feature_id new attribute focused_feature_id
 */
static inline void data_diagramelement_set_focused_feature_id ( data_diagramelement_t *this_, int64_t focused_feature_id );

#include "data_diagramelement.inl"

#endif  /* DATA_DIAGRAMELEMENT_H */


/*
Copyright 2016-2019 Andreas Warnke

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
