/* File: gui_sketch_tools.h; Copyright and License: see below */

#ifndef GUI_SKETCH_TOOLS_H
#define GUI_SKETCH_TOOLS_H

/* public file for the doxygen documentation: */
/*! \file
 *  \brief Informs the listener on the currently selected tool
 */

#include "gui_sketch_marker.h"
#include "gui_simple_message_to_user.h"
#include "gui_serializer_deserializer.h"
#include "ctrl_controller.h"
#include "util/string/utf8stringbuf.h"
#include <gtk/gtk.h>

/*!
 *  \brief enumeration on tools
 */
enum gui_sketch_tools_tool_enum {
    GUI_SKETCH_TOOLS_NAVIGATE,
    GUI_SKETCH_TOOLS_EDIT,
    GUI_SKETCH_TOOLS_CREATE_OBJECT,
    GUI_SKETCH_TOOLS_CREATE_DIAGRAM,
};

typedef enum gui_sketch_tools_tool_enum gui_sketch_tools_tool_t;

/*!
 *  \brief attributes of the sketch tools object
 */
struct gui_sketch_tools_struct {
    gui_sketch_tools_tool_t selected_tool;
    GObject *listener;
    data_database_reader_t *db_reader;  /*!< pointer to external data_database_reader */
    ctrl_controller_t *controller;  /*!< pointer to external controller */
    gui_sketch_marker_t *marker;  /*!< pointer to external sketch marker */
    gui_simple_message_to_user_t *message_to_user;  /*!< pointer to external message-displayer */
    gui_serializer_deserializer_t serdes;  /*!< own instance of a serializer deserializer object */

    GtkToolItem *tool_navigate;  /*!< pointer to external GtkRadioToolButton */
    GtkToolItem *tool_edit;  /*!< pointer to external GtkRadioToolButton */
    GtkToolItem *tool_new_obj;  /*!< pointer to external GtkRadioToolButton */
    GtkToolItem *tool_new_view;  /*!< pointer to external GtkRadioToolButton */

    GtkClipboard *the_clipboard;  /*!< pointer to external GtkClipboard */
    utf8stringbuf_t clipboard_stringbuf;  /*!< stringbuffer to read and write to/from the clipboard */
    char private_clipboard_buffer[128*1024];  /*!< stringbuffer to read and write to/from the clipboard */
};

typedef struct gui_sketch_tools_struct gui_sketch_tools_t;

extern const char *GUI_SKETCH_TOOLS_GLIB_SIGNAL_NAME;

/*!
 *  \brief initializes the gui_sketch_tools_t struct
 *
 *  \param this_ pointer to own object attributes
 *  \param tool_navigate the GTK widget
 *  \param tool_edit the GTK widget
 *  \param tool_new_obj the GTK widget
 *  \param tool_new_view the GTK widget
 *  \param clipboard pointer to the main/primary GtkClipboard
 *  \param marker pointer to the set of marked items
 *  \param message_to_user pointer to an object that can show a message to the user
 *  \param db_reader pointer to a database reader
 *  \param controller pointer to a controller object which can modify the database
 */
void gui_sketch_tools_init ( gui_sketch_tools_t *this_,
                             GtkToolItem *tool_navigate,
                             GtkToolItem *tool_edit,
                             GtkToolItem *tool_new_obj,
                             GtkToolItem *tool_new_view,
                             GtkClipboard *clipboard,
                             gui_sketch_marker_t *marker,
                             gui_simple_message_to_user_t *message_to_user,
                             data_database_reader_t *db_reader,
                             ctrl_controller_t *controller
                           );

/*!
 *  \brief destroys the gui_sketch_tools_t struct
 *
 *  \param this_ pointer to own object attributes
 */
void gui_sketch_tools_destroy ( gui_sketch_tools_t *this_ );

/*!
 *  \brief gets the selected tool
 *
 *  \param this_ pointer to own object attributes
 */
static inline gui_sketch_tools_tool_t gui_sketch_tools_get_selected_tool ( gui_sketch_tools_t *this_ );

/*!
 *  \brief sets the selected tool
 *
 *  \param this_ pointer to own object attributes
 *  \param tool tool to be selected
 */
static inline gui_sketch_tools_tool_t gui_sketch_tools_set_selected_tool ( gui_sketch_tools_t *this_, gui_sketch_tools_tool_t tool );

/*!
 *  \brief callback that informs that the tool button was pressed
 */
void gui_sketch_tools_navigate_btn_callback( GtkWidget* button, gpointer data );

/*!
 *  \brief callback that informs that the tool button was pressed
 */
void gui_sketch_tools_edit_btn_callback( GtkWidget* button, gpointer data );

/*!
 *  \brief callback that informs that the tool button was pressed
 */
void gui_sketch_tools_create_object_btn_callback( GtkWidget* button, gpointer data );

/*!
 *  \brief callback that informs that the tool button was pressed
 */
void gui_sketch_tools_create_diagram_btn_callback( GtkWidget* button, gpointer data );

/*!
 *  \brief callback that informs that the tool button was pressed
 */
void gui_sketch_tools_cut_btn_callback( GtkWidget* button, gpointer data );

/*!
 *  \brief callback that informs that the tool button was pressed
 */
void gui_sketch_tools_copy_btn_callback( GtkWidget* button, gpointer data );

/*!
 *  \brief callback that informs that the tool button was pressed
 */
void gui_sketch_tools_paste_btn_callback( GtkWidget* button, gpointer data );

/*!
 *  \brief callback that informs that the text from the clipboard is now available
 */
void gui_sketch_tools_clipboard_text_received_callback ( GtkClipboard *clipboard, const gchar *text, gpointer data );

/*!
 *  \brief callback that informs that the tool button was pressed
 */
void gui_sketch_tools_delete_btn_callback( GtkWidget* button, gpointer data );

/*!
 *  \brief callback that informs that the tool button was pressed
 */
void gui_sketch_tools_highlight_btn_callback( GtkWidget* button, gpointer data );

/*!
 *  \brief callback that informs that the tool button was pressed
 */
void gui_sketch_tools_instantiate_btn_callback( GtkWidget* button, gpointer data );

/*!
 *  \brief callback that informs that the tool button was pressed
 */
void gui_sketch_tools_undo_btn_callback( GtkWidget* button, gpointer data );

/*!
 *  \brief callback that informs that the tool button was pressed
 */
void gui_sketch_tools_redo_btn_callback( GtkWidget* button, gpointer data );

/*!
 *  \brief sets the listener
 *
 *  \param this_ pointer to own object attributes
 *  \param listener pointer the one listener to change tool events
 */
static inline void gui_sketch_tools_set_listener ( gui_sketch_tools_t *this_, GObject *listener );

/*!
 *  \brief removes the listener
 *
 *  \param this_ pointer to own object attributes
 */
static inline void gui_sketch_tools_remove_listener ( gui_sketch_tools_t *this_ );

/*!
 *  \brief notifies the listener
 *
 *  \param this_ pointer to own object attributes
 */
void gui_sketch_tools_private_notify_listener( gui_sketch_tools_t *this_ );

/*!
 *  \brief copies the clipboard contents to the database
 */
void gui_sketch_tools_private_copy_clipboard_to_db( gui_sketch_tools_t *this_, const char *json_text );

/*!
 *  \brief copies the clipboard contents to the focused diagram
 */
void gui_sketch_tools_private_copy_clipboard_to_diagram( gui_sketch_tools_t *this_, const char *json_text, int64_t diagram_id );

/*!
 *  \brief deletes a set of objects
 */
void gui_sketch_tools_private_delete_set( gui_sketch_tools_t *this_, data_small_set_t *set_to_be_deleted );

/*!
 *  \brief toggles display flags in a set of objects
 */
void gui_sketch_tools_private_toggle_display_flag_in_set( gui_sketch_tools_t *this_, data_small_set_t *set_to_be_toggled, data_diagramelement_flag_t flag_bits_to_toggle );

#include "gui_sketch_tools.inl"

#endif  /* GUI_SKETCH_TOOLS_H */


/*
Copyright 2016-2017 Andreas Warnke

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
