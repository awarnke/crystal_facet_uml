/* File: pencil_feat_label_layouter.c; Copyright and License: see below */

#include "pencil_feat_label_layouter.h"
#include "trace.h"

void pencil_feat_label_layouter_init( pencil_feat_label_layouter_t *this_,
                                      pencil_layout_data_t *layout_data,
                                      pencil_size_t *pencil_size
                                    )
{
    TRACE_BEGIN();
    assert( NULL != layout_data );
    assert( NULL != pencil_size );

    (*this_).layout_data = layout_data;
    (*this_).pencil_size = pencil_size;

    TRACE_END();
}

void pencil_feat_label_layouter_destroy( pencil_feat_label_layouter_t *this_ )
{
    TRACE_BEGIN();

    TRACE_END();
}

void pencil_feat_label_layouter_private_do_layout ( pencil_feat_label_layouter_t *this_ )
{
    TRACE_BEGIN();
    assert ( (unsigned int) UNIVERSAL_ARRAY_INDEX_SORTER_MAX_ARRAY_SIZE >= (unsigned int) PENCIL_LAYOUT_DATA_MAX_FEATURES );

    universal_array_index_sorter_t sorted;
    universal_array_index_sorter_init( &sorted );

    /* sort the features by their label-box layouting needs, drop invisible relations */
    pencil_feat_label_layouter_private_propose_processing_order ( this_, &sorted );

    /* layout the features label-boxes */
    uint32_t count_sorted;
    count_sorted = universal_array_index_sorter_get_count( &sorted );
    for ( uint32_t sort_index = 0; sort_index < count_sorted; sort_index ++ )
    {
        /* declaration of list of options */
        uint32_t solutions_count = 0;
        static const uint32_t SOLUTIONS_MAX = 8;
        geometry_rectangle_t solution[8];

        /* propose options */
        pencil_feat_label_layouter_private_propose_solutions ( this_,
                                                               &sorted,
                                                               sort_index,
                                                               SOLUTIONS_MAX,
                                                               solution,
                                                               &solutions_count
                                                             );

        /* select best option */
        uint32_t index_of_best;
        if ( 1 == solutions_count )
        {
            index_of_best = 0;
        }
        else
        {
            pencil_feat_label_layouter_private_select_solution ( this_,
                                                                 &sorted,
                                                                 sort_index,
                                                                 solutions_count,
                                                                 solution,
                                                                 &index_of_best
                                                               );
        }

        /* store best option to (*this_).layout_data */
        uint32_t index;
        index = universal_array_index_sorter_get_array_index( &sorted, sort_index );
        /* copy the feature shape */
        layout_feature_t *the_feature;
        the_feature = pencil_layout_data_get_feature_ptr( (*this_).layout_data, index );
        geometry_rectangle_t *feature_label_box;
        feature_label_box = layout_feature_get_label_box_ptr( the_feature );
        geometry_rectangle_copy( feature_label_box, &(solution[index_of_best]) );
    }

    universal_array_index_sorter_destroy( &sorted );

    TRACE_END();
}

void pencil_feat_label_layouter_private_propose_processing_order ( pencil_feat_label_layouter_t *this_, universal_array_index_sorter_t *out_sorted )
{
    TRACE_BEGIN();
    assert( NULL != out_sorted );

    TRACE_END();
}

void pencil_feat_label_layouter_private_propose_solutions ( pencil_feat_label_layouter_t *this_,
                                                            const universal_array_index_sorter_t *sorted,
                                                            uint32_t sort_index,
                                                            uint32_t solutions_max,
                                                            geometry_rectangle_t out_solutions[],
                                                            uint32_t *out_solutions_count )
{
    TRACE_BEGIN();
    assert( NULL != sorted );
    assert( NULL != out_solutions );
    assert( NULL != out_solutions_count );

    TRACE_END();
}


void pencil_feat_label_layouter_private_select_solution ( pencil_feat_label_layouter_t *this_,
                                                          const universal_array_index_sorter_t *sorted,
                                                          uint32_t sort_index,
                                                          uint32_t solutions_count,
                                                          const geometry_rectangle_t solutions[],
                                                          uint32_t *out_index_of_best )
{
    TRACE_BEGIN();
    assert( NULL != sorted );
    assert( NULL != solutions );
    assert( NULL != out_index_of_best );

    TRACE_END();
}


/*
 * Copyright 2019-2019 Andreas Warnke
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
