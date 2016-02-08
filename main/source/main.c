/* File: main.c; Copyright and License: see below */

#include "gui_main.h"
#include "data_database.h"
#include "ctrl_controller.h"
#include "trace.h"
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

static data_database_t database;
static ctrl_controller_t controller;

/*!
 *  \brief main starts the gui
 */
int main (int argc, char *argv[]) {
    TRACE_BEGIN();
    int exit_code = 0;

    TRACE_INFO("starting DB...");
    data_database_init( &database );
    data_database_open( &database, "crystal_facet_uml_default.cfu.sqlite3" );
    
    TRACE_INFO("initializing controller...");
    ctrl_controller_init( &controller, &database );
    
    TRACE_INFO("running GUI...");
    gui_main( argc, argv, &controller, &database );
    TRACE_INFO("GUI stopped.");
    
    TRACE_INFO("destroying controller...");
    ctrl_controller_destroy( &controller );
    
    TRACE_INFO("stopping DB...");
    data_database_close( &database );
    data_database_destroy( &database );
    
    TRACE_END_ERR(exit_code);
    return exit_code;
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
