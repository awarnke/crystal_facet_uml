/* File: test_main.c; Copyright and License: see below */

#include "unit/data_change_notifier_test.h"
#include "unit/data_small_set_test.h"
#include "unit/data_rules_test.h"
#include "unit/data_database_listener_test.h"
#include "unit/data_database_reader_test.h"
#include "unit/ctrl_diagram_controller_test.h"
#include "unit/ctrl_controller_test.h"
#include "unit/ctrl_classifier_controller_test.h"
#include "unit/ctrl_consistency_checker_test.h"
#include "unit/ctrl_undo_redo_list_test.h"
#include "unit/ctrl_diagram_policy_enforcer_test.h"
#include "unit/ctrl_classifier_policy_enforcer_test.h"
#include "unit/geometry_rectangle_test.h"
#include "unit/geometry_connector_test.h"
#include "unit/geometry_non_linear_scale_test.h"
#include "unit/gui_sketch_nav_tree_test.h"
#include "unit/txt_writer_test.h"
#include "unit/json_tokenizer_test.h"
#include "unit/json_import_to_database_test.h"
#include "unit/md_filter_test.h"
#include "integration/io_export_model_traversal_test.h"
#include "unit/pencil_layout_data_test.h"
#include "integration/pencil_layouter_test.h"
#include "integration/pencil_diagram_maker_test.h"
#include "unit/universal_array_index_sorter_test.h"
#include "unit/universal_array_list_test.h"
#include "unit/universal_memory_output_stream_test.h"
#include "unit/universal_escaping_output_stream_test.h"
#include "unit/utf8stringbuf_test.h"
#include "unit/utf8codepoint_test.h"
#include "unit/utf8codepointiterator_test.h"
#include "unit/utf8string_test.h"
/* #include "unit/utf8stringbuf_performance.h" -- performance measurements are not reliable - depend on system config+load */
#include "unit/utf8stringview_test.h"
#include "unit/utf8stringviewiterator_test.h"
#include "trace.h"
#include "tslog.h"
#include "meta/meta_info.h"
#include "meta/meta_version.h"
#include "util/string/utf8string.h"
#include "test_runner.h"
#include <gtk/gtk.h>
#include <stdbool.h>
#include <stdio.h>

/*!
 *  \brief main runs the unit tests
 */
int main (int argc, char *argv[]) {
    TRACE_BEGIN();
    TRACE_TIMESTAMP();
    TRACE_INFO( "--------------------" );
    int exit_code = 0;
    TSLOG_INIT(META_INFO_PROGRAM_ID_STR);
    
    /* print id, license and copyrights */
    {
        fprintf( stdout, "    + %s %s\n", META_INFO_PROGRAM_ID_STR, META_VERSION_STR );
        fprintf( stdout, "    + Copyright: %s\n", META_INFO_COPYRIGHT_STR );
        fprintf( stdout, "    + License: %s\n", META_INFO_LICENSE_STR );
    }

    bool do_unit_tests = false;
    bool do_integration_tests = false;

    /* handle options */
    if ( argc == 2 )
    {
        if ( utf8string_equals_str( argv[1], "-h" ) )
        {
            fprintf( stdout, "\nUsage:\n" );
            fprintf( stdout, "    %s -h for help\n", argv[0] );
            fprintf( stdout, "    %s -u to run the unit-tests (test functions of single software units)\n", argv[0] );
            fprintf( stdout, "    %s -i to run the integration tests (test interactions between several units)\n", argv[0] );
            fprintf( stdout, "    %s -a to run all tests\n", argv[0] );
        }
        if ( utf8string_equals_str( argv[1], "-u" ) )
        {
            do_unit_tests = true;
        }
        if ( utf8string_equals_str( argv[1], "-i" ) )
        {
            do_integration_tests = true;
        }
        if ( utf8string_equals_str( argv[1], "-m" ) )  /* compatibility to 1.27.3 and older */
        {
            do_integration_tests = true;
        }
        if ( utf8string_equals_str( argv[1], "-a" ) )
        {
            do_unit_tests = true;
            do_integration_tests = true;
        }
    }

    /* initialize the base libraries: gobject, gio, glib, gdk and gtk */
    {
        gboolean success = gtk_init_check(&argc, &argv);
        if ( ! success )
        {
            TSLOG_WARNING("gtk could not be initialized.");
        }
    }

    /* start tests */
    test_runner_t runner;
    test_runner_init ( &runner );

    /* unit-tests */
    if ( do_unit_tests )
    {
        test_runner_run_suite( &runner, data_small_set_test_get_list() );
        test_runner_run_suite( &runner, data_rules_test_get_list() );
        test_runner_run_suite( &runner, data_change_notifier_test_get_list() );
        test_runner_run_suite( &runner, data_database_listener_test_get_list() );
        test_runner_run_suite( &runner, geometry_rectangle_test_get_list() );
        test_runner_run_suite( &runner, geometry_connector_test_get_list() );
        test_runner_run_suite( &runner, geometry_non_linear_scale_test_get_list() );
        test_runner_run_suite( &runner, txt_writer_test_get_list() );
        test_runner_run_suite( &runner, json_tokenizer_test_get_list() );
        test_runner_run_suite( &runner, pencil_layout_data_test_get_list() );
        test_runner_run_suite( &runner, universal_array_index_sorter_test_get_list() );
        test_runner_run_suite( &runner, universal_array_list_test_get_list() );
        test_runner_run_suite( &runner, universal_memory_output_stream_test_get_list() );
        test_runner_run_suite( &runner, universal_escaping_output_stream_test_get_list() );
        test_runner_run_suite( &runner, utf8codepoint_test_get_list() );
        test_runner_run_suite( &runner, utf8codepointiterator_test_get_list() );
        test_runner_run_suite( &runner, utf8stringbuf_test_get_list() );
        test_runner_run_suite( &runner, utf8string_test_get_list() );
        /* test_runner_run_suite( &runner, utf8stringbuf_performance_get_list() ); -- performance measurements are not reliable - depend on system config+load */
        test_runner_run_suite( &runner, utf8stringview_test_get_list() );
        test_runner_run_suite( &runner, utf8stringviewiterator_test_get_list() );
        test_runner_run_suite( &runner, gui_sketch_nav_tree_test_get_list() );
    }

    /* integration tests which involve multiple software units */
    if ( do_integration_tests )
    {
        test_runner_run_suite( &runner, data_database_reader_test_get_list() );
        test_runner_run_suite( &runner, ctrl_controller_test_get_list() );
        test_runner_run_suite( &runner, ctrl_diagram_controller_test_get_list() );
        test_runner_run_suite( &runner, ctrl_classifier_controller_test_get_list() );
        test_runner_run_suite( &runner, ctrl_consistency_checker_test_get_list() );
        test_runner_run_suite( &runner, ctrl_undo_redo_list_test_get_list() );
        test_runner_run_suite( &runner, ctrl_diagram_policy_enforcer_test_get_list() );
        test_runner_run_suite( &runner, ctrl_classifier_policy_enforcer_test_get_list() );
        test_runner_run_suite( &runner, pencil_layouter_test_get_list() );
        test_runner_run_suite( &runner, pencil_diagram_maker_test_get_list() );
        test_runner_run_suite( &runner, json_import_to_database_test_get_list() );
        test_runner_run_suite( &runner, md_filter_test_get_list() );
        test_runner_run_suite( &runner, io_export_model_traversal_test_get_list() );
    }

    /* fetch failures */
    test_result_t res = test_get_result( &runner );
    fprintf( stdout,
             "ALL TESTS - RESULT: total %d, failed: %d\n",
             test_result_get_total( &res ),
             test_result_get_failed( &res )
           );
    exit_code = test_result_get_failed( &res );

    TSLOG_DESTROY();
    TRACE_INFO( "--------------------" );
    TRACE_TIMESTAMP();
    TRACE_END_ERR( exit_code );
    return exit_code;
}


/*
 * Copyright 2016-2021 Andreas Warnke
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
