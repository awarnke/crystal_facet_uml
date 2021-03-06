/* File: gui_file_use_db_dialog.c; Copyright and License: see below */

#include "gui_file_use_db_dialog.h"
#include "trace.h"
#include <gdk/gdk.h>
#include <gtk/gtk.h>
#include <stdio.h>
#include <stdbool.h>

void gui_file_use_db_dialog_init ( gui_file_use_db_dialog_t *this_,
                                   ctrl_controller_t *controller,
                                   data_database_t *database,
                                   GtkWindow *parent_window,
                                   gui_simple_message_to_user_t *message_to_user )
{
    TRACE_BEGIN();

    (*this_).use_db_file_chooser = gtk_file_chooser_dialog_new ( "Select DB to use",
                                                                 parent_window,
                                                                 GTK_FILE_CHOOSER_ACTION_SAVE,
                                                                 "Cancel",
                                                                 GTK_RESPONSE_CANCEL,
                                                                 "Create/Use DB-File",
                                                                 GTK_RESPONSE_ACCEPT,
                                                                 NULL
                                                               );
    gtk_file_chooser_set_current_name( GTK_FILE_CHOOSER( (*this_).use_db_file_chooser ), "untitled.cfu1" );

    gui_file_db_manager_init( &((*this_).file_manager), controller, database, message_to_user );

    g_signal_connect( G_OBJECT((*this_).use_db_file_chooser), "response", G_CALLBACK(gui_file_db_manager_use_db_response_callback), &((*this_).file_manager) );
    g_signal_connect( G_OBJECT((*this_).use_db_file_chooser), "delete-event", G_CALLBACK(gtk_widget_hide_on_delete), NULL );

    TRACE_END();
}

void gui_file_use_db_dialog_destroy( gui_file_use_db_dialog_t *this_ )
{
    TRACE_BEGIN();

    gtk_widget_destroy( (*this_).use_db_file_chooser );
    /* no need to g_object_unref ( (*this_).use_db_file_chooser ); here */

    gui_file_db_manager_destroy( &((*this_).file_manager) );

    TRACE_END();
}

void gui_file_use_db_dialog_show( gui_file_use_db_dialog_t *this_ )
{
    TRACE_BEGIN();

    gtk_widget_show_all( GTK_WIDGET( (*this_).use_db_file_chooser ) );

    TRACE_END();
}


/*
Copyright 2019-2021 Andreas Warnke

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
