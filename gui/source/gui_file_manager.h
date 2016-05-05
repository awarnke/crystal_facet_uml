/* File: gui_file_manager.h; Copyright and License: see below */

#ifndef GUI_FILE_MANAGER_H
#define GUI_FILE_MANAGER_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Switches the currently used database, exports all diagrams
 */

#include "storage/data_database.h"
#include "ctrl_controller.h"
#include <gtk/gtk.h>

/*!
 *  \brief attributes of the file manager
 */
struct gui_file_manager_struct {
    void *dummy;
};

typedef struct gui_file_manager_struct gui_file_manager_t;

/*!
 *  \brief initializes the main window
 *
 *  \param this_ pointer to own object attributes
 */
void gui_file_manager_init( gui_file_manager_t *this_, ctrl_controller_t *controller, data_database_t *database );

/*!
 *  \brief destroys the main window
 *
 *  \param this_ pointer to own object attributes
 */
void gui_file_manager_destroy( gui_file_manager_t *this_ );

#endif  /* GUI_FILE_MANAGER_H */


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
