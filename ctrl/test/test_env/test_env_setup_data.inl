/* File: test_env_setup_data.c; Copyright and License: see below */

#include "ctrl_controller.h"
#include "storage/data_database.h"
#include "storage/data_database_writer.h"
#include "storage/data_database_reader.h"
#include "trace.h"
#include "tslog.h"
#include "test_assert.h"
#include <glib.h>
#include <assert.h>


static data_row_id_t test_env_setup_data_create_diagram( data_row_id_t parent_diagram_id, const char* name, ctrl_controller_t *controller )
{
    ctrl_error_t ctrl_err;
    data_error_t data_err;
    ctrl_diagram_controller_t *diagram_ctrl;
    diagram_ctrl = ctrl_controller_get_diagram_control_ptr( controller );

    /* create a diagram */
    data_row_id_t root_diag_id;
    {
        static data_diagram_t new_diagram;  /* static ok for a single-threaded test case and preserves stack space, which is important for 32bit systems */
        data_err = data_diagram_init ( &new_diagram,
                                       DATA_ROW_ID_VOID /* diagram_id is ignored */,
                                       parent_diagram_id,
                                       DATA_DIAGRAM_TYPE_UML_SEQUENCE_DIAGRAM,  /* default is an interaction diagram */
                                       name,
                                       "diagram_description-root",
                                       -13 /* list_order */
                                     );
        TEST_ENVIRONMENT_ASSERT( DATA_ERROR_NONE == data_err );

        root_diag_id = DATA_ROW_ID_VOID;
        ctrl_err = ctrl_diagram_controller_create_diagram ( diagram_ctrl,
                                                            &new_diagram,
                                                            CTRL_UNDO_REDO_ACTION_BOUNDARY_START_NEW,
                                                            &root_diag_id
                                                        );
        TEST_ENVIRONMENT_ASSERT( CTRL_ERROR_NONE == ctrl_err );
        data_diagram_destroy ( &new_diagram );
    }
    TEST_ENVIRONMENT_ASSERT( DATA_ROW_ID_VOID != root_diag_id );

    return root_diag_id;
}

static data_row_id_t test_env_setup_data_create_classifier( const char* name, 
                                                            ctrl_controller_t *controller )
{
    ctrl_error_t ctrl_err;
    data_error_t data_err;
    ctrl_classifier_controller_t *classifier_ctrl;
    classifier_ctrl = ctrl_controller_get_classifier_control_ptr( controller );
    
    /* create a classifier */
    data_row_id_t classifier_id;
    {
        static data_classifier_t new_classifier;  /* static ok for a single-threaded test case and preserves stack space, which is important for 32bit systems */
        data_err = data_classifier_init( &new_classifier,
                                         DATA_ROW_ID_VOID /* classifier_id is ignored */,
                                         DATA_CLASSIFIER_TYPE_COMPONENT,
                                         "",  /* stereotype */
                                         name,
                                         "",  /* description */
                                         45,
                                         4500,
                                         450000
                                       );
        TEST_ENVIRONMENT_ASSERT( DATA_ERROR_NONE == data_err );
        ctrl_err = ctrl_classifier_controller_create_classifier ( classifier_ctrl,
                                                                  &new_classifier,
                                                                  CTRL_UNDO_REDO_ACTION_BOUNDARY_START_NEW,
                                                                  &classifier_id
                                                                );
        TEST_ENVIRONMENT_ASSERT( CTRL_ERROR_NONE == ctrl_err );
        data_classifier_destroy ( &new_classifier );
    }
    TEST_ENVIRONMENT_ASSERT( DATA_ROW_ID_VOID != classifier_id );
    
    return classifier_id;
}
    
static data_row_id_t test_env_setup_data_create_diagramelement( data_row_id_t diagram_id, 
                                                                data_row_id_t classifier_id, 
                                                                data_row_id_t focused_feature_id, 
                                                                ctrl_controller_t *controller )
{
    ctrl_error_t ctrl_err;
    ctrl_diagram_controller_t *diagram_ctrl;
    diagram_ctrl = ctrl_controller_get_diagram_control_ptr( controller );
    
    /* create a diagramelement */
    data_row_id_t diagele_id;
    {
        static data_diagramelement_t new_diagele;  /* static ok for a single-threaded test case and preserves stack space, which is important for 32bit systems */
        data_diagramelement_init ( &new_diagele,
                                   DATA_ROW_ID_VOID /* diagramelement_id is ignored */,
                                   diagram_id,
                                   classifier_id,
                                   DATA_DIAGRAMELEMENT_FLAG_NONE,
                                   DATA_ROW_ID_VOID
                                 );
        ctrl_err = ctrl_diagram_controller_create_diagramelement ( diagram_ctrl,
                                                                   &new_diagele,
                                                                   CTRL_UNDO_REDO_ACTION_BOUNDARY_START_NEW,
                                                                   &diagele_id
                                                                 );
        TEST_ENVIRONMENT_ASSERT( CTRL_ERROR_NONE == ctrl_err );
        data_diagramelement_destroy ( &new_diagele );
    }
    TEST_ENVIRONMENT_ASSERT( DATA_ROW_ID_VOID != diagele_id );
    
    return diagele_id;
}

static data_row_id_t test_env_setup_data_create_feature( data_row_id_t parent_classifier_id, 
                                                         const char* name,
                                                         ctrl_controller_t *controller )
{
    ctrl_error_t ctrl_err;
    data_error_t data_err;
    ctrl_classifier_controller_t *classifier_ctrl;
    classifier_ctrl = ctrl_controller_get_classifier_control_ptr( controller );
    
    /* create a feature */
    data_row_id_t new_feature_id;
    {
        static data_feature_t new_feature;  /* static ok for a single-threaded test case and preserves stack space, which is important for 32bit systems */
        data_err = data_feature_init( &new_feature,
                                      DATA_ROW_ID_VOID, /* feature_id is ignored*/
                                      DATA_FEATURE_TYPE_PROPERTY, /* feature_main_type */
                                      parent_classifier_id,
                                      name, /* feature_key */
                                      "uint64_t", /* feature_value */
                                      "time in nano seconds to start", /* feature_description */
                                      5000000 /* list order */
                                    );
        TEST_ENVIRONMENT_ASSERT( DATA_ERROR_NONE == data_err );

        /* 1. create the feature in the db */
        ctrl_err = ctrl_classifier_controller_create_feature( classifier_ctrl,
                                                              &new_feature,
                                                              CTRL_UNDO_REDO_ACTION_BOUNDARY_START_NEW,
                                                              &new_feature_id
                                                            );
        TEST_ENVIRONMENT_ASSERT( CTRL_ERROR_NONE == ctrl_err );
    }
    TEST_ENVIRONMENT_ASSERT( DATA_ROW_ID_VOID != new_feature_id );
    
    return new_feature_id;
}

static data_row_id_t test_env_setup_data_create_relationship( data_row_id_t from_classifier_id, 
                                                              data_row_id_t from_feature_id,
                                                              data_row_id_t to_classifier_id, 
                                                              data_row_id_t to_feature_id,
                                                              const char* name, 
                                                              ctrl_controller_t *controller )
{
    ctrl_error_t ctrl_err;
    data_error_t data_err;
    ctrl_classifier_controller_t *classifier_ctrl;
    classifier_ctrl = ctrl_controller_get_classifier_control_ptr( controller );
    
    /* create a relationship */
    data_row_id_t new_relationship_id;
    {
        static data_relationship_t new_relationship;  /* static ok for a single-threaded test case and preserves stack space, which is important for 32bit systems */
        data_err = data_relationship_init( &new_relationship,
                                           DATA_ROW_ID_VOID, /* relationship_id is ignored */
                                           DATA_RELATIONSHIP_TYPE_UML_COMPOSITION, /* relationship_main_type */
                                           from_classifier_id,
                                           to_classifier_id,
                                           name,
                                           "my description", /* relationship_description */
                                           -66000, /* list_order */
                                           from_feature_id,
                                           to_feature_id
                                        );
        TEST_ENVIRONMENT_ASSERT( DATA_ERROR_NONE == data_err );

        /* 3b. create the relationship in the db */
        ctrl_err = ctrl_classifier_controller_create_relationship( classifier_ctrl,
                                                                   &new_relationship,
                                                                   CTRL_UNDO_REDO_ACTION_BOUNDARY_START_NEW,
                                                                   &new_relationship_id
                                                                 );
        TEST_ENVIRONMENT_ASSERT( CTRL_ERROR_NONE == ctrl_err );
    }
    TEST_ENVIRONMENT_ASSERT( DATA_ROW_ID_VOID != new_relationship_id );
    
    return new_relationship_id;
}
    
    
/*
 * Copyright 2020-2021 Andreas Warnke
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
