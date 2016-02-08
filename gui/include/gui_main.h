/* File: gui_main.h; Copyright and License: see below */

#ifndef GUI_MAIN_H
#define GUI_MAIN_H

/* public file for the doxygen documentation: */
/*! \file */

#include "data_database.h"
#include "ctrl_controller.h"
#include <gtk/gtk.h>

/*!
 * \brief initializes the gui widgets
 */
void gui_main ( int argc, char *argv[], ctrl_controller_t *controller, data_database_t *database );

#endif  /* GUI_MAIN_H */


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
