/* File: pencil_input_data.c; Copyright and License: see below */

#include "pencil_input_data.h"
#include "trace.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

void pencil_input_data_init( pencil_input_data_t *this_ )
{
    TRACE_BEGIN();
    TRACE_INFO_INT( "sizeof(pencil_input_data_t):", sizeof(pencil_input_data_t) );

    data_diagram_init_empty( &((*this_).diagram) );
    (*this_).visible_classifier_count = 0;
    (*this_).feature_count = 0;
    (*this_).relationship_count = 0;

    TRACE_END();
}

void pencil_input_data_destroy( pencil_input_data_t *this_ )
{
    TRACE_BEGIN();

    data_diagram_destroy( &((*this_).diagram) );
    pencil_input_data_private_destroy_visible_classifiers( this_ );
    pencil_input_data_private_destroy_features( this_ );
    pencil_input_data_private_destroy_relationships( this_ );

    TRACE_END();
}

void pencil_input_data_load( pencil_input_data_t *this_, int64_t diagram_id, data_database_reader_t *db_reader )
{
    TRACE_BEGIN();
    assert( NULL != db_reader );

    if ( DATA_ID_VOID_ID == diagram_id )
    {
        /* re-init */
        data_diagram_reinit_empty( &((*this_).diagram) );

        pencil_input_data_private_destroy_visible_classifiers( this_ );
        pencil_input_data_private_destroy_features( this_ );
        pencil_input_data_private_destroy_relationships( this_ );
    }
    else
    {
        data_error_t db_err;

        data_diagram_destroy( &((*this_).diagram) );
        pencil_input_data_private_destroy_visible_classifiers( this_ );
        pencil_input_data_private_destroy_features( this_ );
        pencil_input_data_private_destroy_relationships( this_ );

        /* load diagram */
        db_err = data_database_reader_get_diagram_by_id ( db_reader, diagram_id, &((*this_).diagram) );

        if ( DATA_ERROR_NONE != (DATA_ERROR_MASK & DATA_ERROR_STRING_BUFFER_EXCEEDED & db_err) )
        {
            TSLOG_ERROR( "DATA_ERROR_STRING_BUFFER_EXCEEDED at loading a diagram" );
        }
        if ( DATA_ERROR_NONE != (DATA_ERROR_MASK & DATA_ERROR_ARRAY_BUFFER_EXCEEDED & db_err) )
        {
            TSLOG_ERROR( "DATA_ERROR_ARRAY_BUFFER_EXCEEDED at loading a diagram" );
        }
        if ( DATA_ERROR_NONE != (db_err & ~(DATA_ERROR_STRING_BUFFER_EXCEEDED|DATA_ERROR_ARRAY_BUFFER_EXCEEDED)) )
        {
            /* error at loading */
            data_diagram_reinit_empty( &((*this_).diagram) );
        }

        /* load classifiers */
        db_err = data_database_reader_get_classifiers_by_diagram_id ( db_reader,
                                                                      diagram_id,
                                                                      PENCIL_INPUT_DATA_MAX_CLASSIFIERS,
                                                                      &((*this_).visible_classifiers),
                                                                      &((*this_).visible_classifier_count)
                                                                    );

        if ( DATA_ERROR_NONE != (DATA_ERROR_MASK & DATA_ERROR_STRING_BUFFER_EXCEEDED & db_err) )
        {
            TSLOG_ERROR( "DATA_ERROR_STRING_BUFFER_EXCEEDED at loading visible classifiers of a diagram" );
        }
        if ( DATA_ERROR_NONE != (DATA_ERROR_MASK & DATA_ERROR_ARRAY_BUFFER_EXCEEDED & db_err) )
        {
            TSLOG_ERROR( "DATA_ERROR_ARRAY_BUFFER_EXCEEDED at loading visible classifiers of a diagram" );
        }
        if ( DATA_ERROR_NONE != (db_err & ~(DATA_ERROR_STRING_BUFFER_EXCEEDED|DATA_ERROR_ARRAY_BUFFER_EXCEEDED)) )
        {
            /* error at loading */
            (*this_).visible_classifier_count = 0;
        }

        /* load features */
        db_err = data_database_reader_get_features_by_diagram_id ( db_reader,
                                                                   diagram_id,
                                                                   PENCIL_INPUT_DATA_MAX_FEATURES,
                                                                   &((*this_).features),
                                                                   &((*this_).feature_count)
                                                                 );

        if ( DATA_ERROR_NONE != (DATA_ERROR_MASK & DATA_ERROR_STRING_BUFFER_EXCEEDED & db_err) )
        {
            TSLOG_ERROR( "DATA_ERROR_STRING_BUFFER_EXCEEDED at loading features of a diagram" );
        }
        if ( DATA_ERROR_NONE != (DATA_ERROR_MASK & DATA_ERROR_ARRAY_BUFFER_EXCEEDED & db_err) )
        {
            TSLOG_ERROR( "DATA_ERROR_ARRAY_BUFFER_EXCEEDED at loading features of a diagram" );
        }
        if ( DATA_ERROR_NONE != (db_err & ~(DATA_ERROR_STRING_BUFFER_EXCEEDED|DATA_ERROR_ARRAY_BUFFER_EXCEEDED)) )
        {
            /* error at loading */
            (*this_).feature_count = 0;
        }

        /* load relationships */
        db_err = data_database_reader_get_relationships_by_diagram_id ( db_reader,
                                                                        diagram_id,
                                                                        PENCIL_INPUT_DATA_MAX_RELATIONSHIPS,
                                                                        &((*this_).relationships),
                                                                        &((*this_).relationship_count)
                                                                      );

        if ( DATA_ERROR_NONE != (DATA_ERROR_MASK & DATA_ERROR_STRING_BUFFER_EXCEEDED & db_err) )
        {
            TSLOG_ERROR( "DATA_ERROR_STRING_BUFFER_EXCEEDED at loading relationships of a diagram" );
        }
        if ( DATA_ERROR_NONE != (DATA_ERROR_MASK & DATA_ERROR_ARRAY_BUFFER_EXCEEDED & db_err) )
        {
            TSLOG_ERROR( "DATA_ERROR_ARRAY_BUFFER_EXCEEDED at loading relationships of a diagram" );
        }
        if ( DATA_ERROR_NONE != (db_err & ~(DATA_ERROR_STRING_BUFFER_EXCEEDED|DATA_ERROR_ARRAY_BUFFER_EXCEEDED)) )
        {
            /* error at loading */
            (*this_).relationship_count = 0;
        }
    }

    /* update the containment cache */
    pencil_input_data_private_update_containment_cache( this_ );

    TRACE_END();
}

void pencil_input_data_private_update_containment_cache ( pencil_input_data_t *this_ )
{
    TRACE_BEGIN();
    assert( (*this_).relationship_count <= PENCIL_INPUT_DATA_MAX_RELATIONSHIPS );
    assert( (*this_).visible_classifier_count <= PENCIL_INPUT_DATA_MAX_CLASSIFIERS );

    /* reset the containment array */
    memset( &((*this_).containment_cache), '\0', sizeof( (*this_).containment_cache ) );

    static const int MAX_SEARCH_PASSES = 4;  /* with 4 passes, we can find ancesters and decendants which are related via 2*4=8 links and more */
    bool new_containments_found = true;

    for ( int pass = 0; ( pass < MAX_SEARCH_PASSES ) && new_containments_found; pass ++ )
    {
        new_containments_found = false;

        for ( uint32_t rel_idx = 0; rel_idx < (*this_).relationship_count; rel_idx ++ )
        {
            data_relationship_t *the_relationship;
            the_relationship = &((*this_).relationships[rel_idx]);

            data_relationship_type_t the_type;
            the_type = data_relationship_get_main_type ( the_relationship );

            if ( DATA_RELATIONSHIP_TYPE_UML_CONTAINMENT == the_type )
            {
                int64_t parent_id;
                int64_t child_id;
                parent_id = data_relationship_get_from_classifier_id ( the_relationship );
                child_id = data_relationship_get_to_classifier_id ( the_relationship );
                int32_t parent_index;
                int32_t child_index;
                parent_index = pencil_input_data_get_classifier_index ( this_, parent_id );
                child_index = pencil_input_data_get_classifier_index ( this_, child_id );
                if ( ( parent_index != -1 )&&( child_index != -1 ) )
                {
                    assert ( 0 <= parent_index );
                    assert ( parent_index < (*this_).visible_classifier_count );
                    assert ( 0 <= child_index );
                    assert ( child_index < (*this_).visible_classifier_count );

                    /* add the current relation to the containment_cache */
                    (*this_).containment_cache[parent_index][child_index] = true;

                    /* all ancestors of parent are ancestors of child */
                    for ( uint32_t ancestor_index = 0; ancestor_index < (*this_).visible_classifier_count; ancestor_index ++ )
                    {
                        if ( (*this_).containment_cache[ancestor_index][parent_index] )
                        {
                            if ( ! (*this_).containment_cache[ancestor_index][child_index] )
                            {
                                new_containments_found = true;
                                (*this_).containment_cache[ancestor_index][child_index] = true;
                            }
                        }
                    }

                    /* all descendants of child are descendants of parent */
                    for ( uint32_t descendant_index = 0; descendant_index < (*this_).visible_classifier_count; descendant_index ++ )
                    {
                        if ( (*this_).containment_cache[child_index][descendant_index] )
                        {
                            if ( ! (*this_).containment_cache[parent_index][descendant_index] )
                            {
                                new_containments_found = true;
                                (*this_).containment_cache[parent_index][descendant_index] = true;
                            }
                        }
                    }

                }
                /* else relation points outside the set of classifiers visible in current diagram */
            }
        }
    }

    TRACE_END();
}


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