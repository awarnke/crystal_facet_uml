/* File: pencil_layout_data_test.c; Copyright and License: see below */

#include "pencil_layout_data_test.h"
#include "pencil_layout_data.h"
#include "set/data_visible_set.h"
#include "test_assert.h"

static void set_up(void);
static void tear_down(void);
static void test_empty_model(void);
static void test_normal_model(void);
static void test_too_big_model(void);
static void test_inconsistent_model(void);
static data_visible_set_t* init_empty_input_data();
static data_visible_set_t* init_fake_input_data( uint_fast32_t classifiers, uint_fast32_t features, uint_fast32_t relationships );

test_suite_t pencil_layout_data_test_get_list(void)
{
    test_suite_t result;
    test_suite_init( &result, "pencil_layout_data_test_get_list", &set_up, &tear_down );
    test_suite_add_test_case( &result, "test_empty_model", &test_empty_model );
    test_suite_add_test_case( &result, "test_normal_model", &test_normal_model );
    test_suite_add_test_case( &result, "test_too_big_model", &test_too_big_model );
    test_suite_add_test_case( &result, "test_inconsistent_model", &test_inconsistent_model );
    return result;
}

static void set_up(void)
{
}

static void tear_down(void)
{
}

static data_visible_set_t* init_empty_input_data()
{
    static data_visible_set_t empty_input_data;
    data_visible_set_init( &empty_input_data );
    data_visible_set_private_update_containment_cache ( &empty_input_data );
    TEST_ENVIRONMENT_ASSERT ( ! data_visible_set_is_valid ( &empty_input_data ) );
    return &empty_input_data;
}

static data_visible_set_t* init_fake_input_data( uint_fast32_t classifiers, uint_fast32_t features, uint_fast32_t relationships )
{
    data_error_t data_err;
    static data_visible_set_t fake_input_data;

    data_visible_set_init( &fake_input_data );

    /* initialize the fake_input_data.diagram */
    {

        data_err = data_diagram_init ( &(fake_input_data.diagram),
                                       3,  /* diagram_id */
                                       DATA_ROW_ID_VOID,  /* parent_diagram_id */
                                       DATA_DIAGRAM_TYPE_UML_CLASS_DIAGRAM,  /* diagram_type */
                                       "diagram_name",
                                       "diagram_description",
                                       32000  /* list_order */
                                     );
        TEST_ENVIRONMENT_ASSERT( data_err == DATA_ERROR_NONE );
    }

    /* initialize the fake_input_data.visible_classifiers */
    fake_input_data.visible_classifier_count = classifiers;
    for ( uint_fast32_t c_idx = 0; c_idx < classifiers; c_idx ++ )
    {
        data_visible_classifier_t *current = &(fake_input_data.visible_classifiers[c_idx]);

        data_visible_classifier_init_empty ( current );

        data_classifier_t *classifier = data_visible_classifier_get_classifier_ptr ( current );
        data_diagramelement_t *diagele = data_visible_classifier_get_diagramelement_ptr ( current );

        data_err = data_classifier_init ( classifier,
                                          c_idx/2,  /* id */
                                          DATA_CLASSIFIER_TYPE_CLASS,  /* main_type */
                                          "stereotype",
                                          "name",
                                          "description",
                                          1000*c_idx,  /* x_order */
                                          -300*c_idx,  /* y_order */
                                          4000*c_idx  /* list_order */
                                        );
        TEST_ENVIRONMENT_ASSERT( data_err == DATA_ERROR_NONE );

        data_diagramelement_init ( diagele,
                                   c_idx,  /* id */
                                   3,  /* diagram_id */
                                   c_idx/2,  /* classifier_id */
                                   DATA_DIAGRAMELEMENT_FLAG_ANONYMOUS_INSTANCE | DATA_DIAGRAMELEMENT_FLAG_EMPHASIS,  /* display_flags */
                                   DATA_ROW_ID_VOID  /* focused_feature_id */
                                 );

        TEST_ENVIRONMENT_ASSERT( data_visible_classifier_is_valid( current ) );
    }

    const uint_fast32_t classifier_mod = ((classifiers/2)==0) ? 1 : (classifiers/2);

    /* initialize the fake_input_data.features */
    fake_input_data.feature_count = features;
    for ( uint_fast32_t f_idx = 0; f_idx < features; f_idx ++ )
    {
        data_feature_t *current = &(fake_input_data.features[f_idx]);

        data_err = data_feature_init ( current,
                                       f_idx,  /* feature_id */
                                       DATA_FEATURE_TYPE_OPERATION,  /* feature_main_type */
                                       f_idx % classifier_mod,  /* classifier_id */
                                       "feature_key",
                                       "feature_value",
                                       "feature_description",
                                       6000*f_idx  /* list_order */
                                     );
        TEST_ENVIRONMENT_ASSERT( data_err == DATA_ERROR_NONE );

        TEST_ENVIRONMENT_ASSERT( data_feature_is_valid( current ) );
    }

    /* initialize the fake_input_data.relationships */
    fake_input_data.relationship_count = relationships;
    for ( uint_fast32_t r_idx = 0; r_idx < relationships; r_idx ++ )
    {
        data_relationship_t *current = &(fake_input_data.relationships[r_idx]);

        data_err = data_relationship_init ( current,
                                            r_idx,  /* relationship_id */
                                            DATA_RELATIONSHIP_TYPE_UML_ASSOCIATION,  /* relationship_main_type */
                                            r_idx % classifier_mod,  /* from_classifier_id */
                                            (r_idx*r_idx) % classifier_mod,  /* to_classifier_id */
                                            "relationship_name",
                                            "relationship_description",
                                            1500*r_idx,  /* list_order */
                                            DATA_ROW_ID_VOID,  /* from_feature_id */
                                            DATA_ROW_ID_VOID  /* to_feature_id */
                                          );
        TEST_ENVIRONMENT_ASSERT( data_err == DATA_ERROR_NONE );

        TEST_ENVIRONMENT_ASSERT( data_relationship_is_valid( current ) );
    }

    data_visible_set_private_update_containment_cache ( &fake_input_data );

    TEST_ENVIRONMENT_ASSERT ( data_visible_set_is_valid ( &fake_input_data ) );
    return &fake_input_data;
}

static const int DUPLICATE_PARENT_CLASSIFIER=2;
static const int DUPLICATE_FROM_CLASSIFIER=2;
static const int DUPLICATE_TO_CLASSIFIER=2;

static void test_empty_model(void)
{
    data_visible_set_t* empty_input_data;
    empty_input_data = init_empty_input_data();

    static pencil_layout_data_t testee;
    pencil_layout_data_init( &testee, empty_input_data );
    TEST_ASSERT( NULL != pencil_layout_data_get_diagram_ptr ( &testee ) )
    TEST_ASSERT_EQUAL_INT( 0, pencil_layout_data_get_visible_classifier_count ( &testee ) );
    TEST_ASSERT_EQUAL_INT( 0, pencil_layout_data_get_feature_count ( &testee ) );
    TEST_ASSERT_EQUAL_INT( 0, pencil_layout_data_get_relationship_count ( &testee ) );
    TEST_ASSERT ( ! pencil_layout_data_is_valid( &testee ) );

    data_visible_set_t *fake_input_data;
    fake_input_data = init_fake_input_data(0,10,10);

    pencil_layout_data_reinit( &testee, fake_input_data );
    TEST_ASSERT( NULL != pencil_layout_data_get_diagram_ptr ( &testee ) );
    TEST_ASSERT_EQUAL_INT( 0, pencil_layout_data_get_visible_classifier_count ( &testee ) );
    TEST_ASSERT_EQUAL_INT( 0, pencil_layout_data_get_feature_count ( &testee ) );
    TEST_ASSERT_EQUAL_INT( 0, pencil_layout_data_get_relationship_count ( &testee ) );
    TEST_ASSERT ( pencil_layout_data_is_valid( &testee ) );

    pencil_layout_data_destroy( &testee );
}

static void test_normal_model(void)
{
    data_visible_set_t *fake_input_data;
    fake_input_data = init_fake_input_data(15,30,20);
    /* make the 0-th relation a from-feature-to-feature relation: */
    data_relationship_t *linked_rel = data_visible_set_get_relationship_ptr ( fake_input_data, 0 /*index*/ );
    data_relationship_set_from_feature_row_id ( linked_rel, 0 /* not DATA_ROW_ID_VOID */ );
    data_relationship_set_to_feature_row_id ( linked_rel, 0 /* not DATA_ROW_ID_VOID */ );

    static pencil_layout_data_t testee;
    pencil_layout_data_init( &testee, fake_input_data );
    TEST_ASSERT( NULL != pencil_layout_data_get_diagram_ptr ( &testee ) );
    TEST_ASSERT_EQUAL_INT( 15, pencil_layout_data_get_visible_classifier_count ( &testee ) );
    TEST_ASSERT_EQUAL_INT( 30*DUPLICATE_PARENT_CLASSIFIER, pencil_layout_data_get_feature_count ( &testee ) );
    TEST_ASSERT_EQUAL_INT( 20*DUPLICATE_FROM_CLASSIFIER*DUPLICATE_TO_CLASSIFIER, pencil_layout_data_get_relationship_count ( &testee ) );
    TEST_ASSERT ( pencil_layout_data_is_valid( &testee ) );

    pencil_layout_data_destroy( &testee );
}

static void test_too_big_model(void)
{
    data_visible_set_t *fake_input_data;
    fake_input_data = init_fake_input_data( DATA_VISIBLE_SET_MAX_CLASSIFIERS,
                                            DATA_VISIBLE_SET_MAX_FEATURES,
                                            DATA_VISIBLE_SET_MAX_RELATIONSHIPS
                                          );

    static pencil_layout_data_t testee;
    pencil_layout_data_init( &testee, fake_input_data );
    TEST_ASSERT( NULL != pencil_layout_data_get_diagram_ptr ( &testee ) );
    TEST_ASSERT_EQUAL_INT( DATA_VISIBLE_SET_MAX_CLASSIFIERS, pencil_layout_data_get_visible_classifier_count ( &testee ) );
    TEST_ASSERT_EQUAL_INT( PENCIL_LAYOUT_DATA_MAX_FEATURES, pencil_layout_data_get_feature_count ( &testee ) );
    TEST_ASSERT_EQUAL_INT( PENCIL_LAYOUT_DATA_MAX_RELATIONSHIPS, pencil_layout_data_get_relationship_count ( &testee ) );
    TEST_ASSERT ( pencil_layout_data_is_valid( &testee ) );

    pencil_layout_data_destroy( &testee );
}

static void test_inconsistent_model(void)
{
    data_visible_set_t *fake_input_data;
    fake_input_data = init_fake_input_data(5,5,5);
    /* make some wrong ids and links */
    data_feature_t *illegal_feat1 = data_visible_set_get_feature_ptr ( fake_input_data, 4 /*index*/ );
    data_feature_set_classifier_row_id ( illegal_feat1, 12000 /*non-existing classifier_id*/ );
    data_relationship_t *illegal_rel1 = data_visible_set_get_relationship_ptr ( fake_input_data, 0 /*index*/ );
    data_relationship_set_from_feature_row_id ( illegal_rel1, 1 /* feature id 1 does not belong to from classifier */ );
    data_relationship_set_to_feature_row_id ( illegal_rel1, 0 /* not DATA_ROW_ID_VOID */ );
    data_relationship_t *illegal_rel2 = data_visible_set_get_relationship_ptr ( fake_input_data, 1 /*index*/ );
    data_relationship_set_from_classifier_row_id ( illegal_rel2, 12000 /*non-existing classifier_id*/ );
    data_relationship_t *illegal_rel3 = data_visible_set_get_relationship_ptr ( fake_input_data, 2 /*index*/ );
    data_relationship_set_to_classifier_row_id ( illegal_rel3, 12000 /*non-existing classifier_id*/ );

    static pencil_layout_data_t testee;
    pencil_layout_data_init( &testee, fake_input_data );
    TEST_ASSERT( NULL != pencil_layout_data_get_diagram_ptr ( &testee ) );
    TEST_ASSERT_EQUAL_INT( 5, pencil_layout_data_get_visible_classifier_count ( &testee ) );
    TEST_ASSERT_EQUAL_INT( (5-1)*DUPLICATE_PARENT_CLASSIFIER, pencil_layout_data_get_feature_count ( &testee ) );
    TEST_ASSERT_EQUAL_INT( (5-3)*DUPLICATE_FROM_CLASSIFIER*DUPLICATE_TO_CLASSIFIER, pencil_layout_data_get_relationship_count ( &testee ) );
    TEST_ASSERT ( pencil_layout_data_is_valid( &testee ) );

    pencil_layout_data_destroy( &testee );
}


/*
 * Copyright 2019-2021 Andreas Warnke
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
