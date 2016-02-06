/* File: gui_main_window.h; Copyright and License: see below */

#ifndef GUI_MAIN_WINDOW_H
#define GUI_MAIN_WINDOW_H

/* public file for the doxygen documentation: */
/*! \file */

#include "gui_sketch_area.h"
#include "gui_sketch_tools.h"
#include <gtk/gtk.h>

struct gui_main_window_struct {
    GtkWidget *window;
    GtkWidget *sketcharea;
    gui_sketch_area_t sketcharea_data;
    gui_sketch_tools_t sketchtools_data;
    GtkWidget *layout;

    GtkWidget *toolbar;
    GtkToolItem *tool_navigate;
    GtkToolItem *tool_edit;
    GtkToolItem *tool_new_obj;
    GtkToolItem *tool_new_view;

    GtkWidget *clear;

    GtkWidget *characterEntry;
};

typedef struct gui_main_window_struct gui_main_window_t;

/*!
 *  \brief initializes the main window
 */
void gui_main_window_init( gui_main_window_t *this_ );

/*!
 *  \brief destroys the main window
 */
void gui_main_window_destroy( gui_main_window_t *this_ );

/*!
 *  \brief callback that is executed when the main window is destroyed
 */
void gui_main_window_destroy_event_callback(GtkWidget *widget, gpointer data );

/*!
 *  \brief callback that is executed when the main window is requeted to be deleted
 */
gboolean gui_main_window_delete_event_callback(GtkWidget *widget, GdkEvent *event, gpointer data );

#endif  /* GUI_MAIN_WINDOW_H */


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
