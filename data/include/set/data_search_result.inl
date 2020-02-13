/* File: data_search_result.inl; Copyright and License: see below */

#include <assert.h>

static inline void data_search_result_init ( data_search_result_t *this_, data_id_t match_id, data_id_t diagram_id )
{
    assert ( DATA_TABLE_DIAGRAM != data_id_get_table( &diagram_id ) );

    (*this_).match_object_id = match_id;
    data_id_init_void( &((*this_).src_classifier_id) );
    data_id_init_void( &((*this_).dst_classifier_id) );
    (*this_).diagram_id = diagram_id;
}

static inline void data_search_result_init_feature ( data_search_result_t *this_,
                                                     data_id_t match_id,
                                                     data_id_t classifier_id,
                                                     data_id_t diagram_id )
{
    assert ( DATA_TABLE_CLASSIFIER != data_id_get_table( &classifier_id ) );
    assert ( DATA_TABLE_DIAGRAM != data_id_get_table( &diagram_id ) );

    (*this_).match_object_id = match_id;
    (*this_).src_classifier_id = classifier_id;
    data_id_init_void( &((*this_).dst_classifier_id) );
    (*this_).diagram_id = diagram_id;
}

static inline void data_search_result_init_relationship ( data_search_result_t *this_,
                                                          data_id_t match_id,
                                                          data_id_t from_classifier_id,
                                                          data_id_t to_classifier_id,
                                                          data_id_t diagram_id )
{
    assert ( DATA_TABLE_CLASSIFIER != data_id_get_table( &from_classifier_id ) );
    assert ( DATA_TABLE_CLASSIFIER != data_id_get_table( &to_classifier_id ) );
    assert ( DATA_TABLE_DIAGRAM != data_id_get_table( &diagram_id ) );

    (*this_).match_object_id = match_id;
    (*this_).src_classifier_id = from_classifier_id;
    (*this_).dst_classifier_id = to_classifier_id;
    (*this_).diagram_id = diagram_id;
}

static inline void data_search_result_destroy ( data_search_result_t *this_ )
{
    data_id_destroy( &((*this_).match_object_id) );
    data_id_destroy( &((*this_).src_classifier_id) );
    data_id_destroy( &((*this_).dst_classifier_id) );
    data_id_destroy( &((*this_).diagram_id) );
}

static inline data_id_t data_search_result_get_match_id ( const data_search_result_t *this_ )
{
    return (*this_).match_object_id;
}

static inline data_id_t *data_search_result_get_match_id_ptr ( data_search_result_t *this_ )
{
    return &((*this_).match_object_id);
}

static inline data_id_t data_search_result_get_diagram_id ( const data_search_result_t *this_ )
{
    return (*this_).diagram_id;
}

static inline data_id_t *data_search_result_get_diagram_id_ptr ( data_search_result_t *this_ )
{
    return &((*this_).diagram_id);
}

static inline void data_search_result_trace ( const data_search_result_t *this_ )
{
    data_id_trace( &((*this_).match_object_id) );
    data_id_trace( &((*this_).src_classifier_id) );
    data_id_trace( &((*this_).dst_classifier_id) );
    data_id_trace( &((*this_).diagram_id) );
}


/*
Copyright 2020-2020 Andreas Warnke

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
