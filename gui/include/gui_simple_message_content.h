/* File: gui_simple_message_content.h; Copyright and License: see below */

#ifndef GUI_SIMPLE_MESSAGE_CONTENT_H
#define GUI_SIMPLE_MESSAGE_CONTENT_H

/* public file for the doxygen documentation: */
/*! \file
 *  \brief Defines a list of message text-ids
 */

#include "set/data_stat.h"

/*!
 *  \brief enumeration on message text-ids
 *
 *  The infix 0 informs that no parameter is expected.
 *  The infix E informs that a error-description parameter is expected.
 *  The infix L informs that a list-of-names parameter is expected.
 *  The infix N informs that a name parameter is expected.
 *  The infix P informs that a stream/string position parameter is expected.
 *  The infix Q informs that a quantitity parameter is expected.
 */
enum gui_simple_message_content_enum {
    GUI_SIMPLE_MESSAGE_CONTENT_0_NO_MESSAGE,  /*!< there is no message */
    GUI_SIMPLE_MESSAGE_CONTENT_0_ABOUT,  /*!< shows the about text */
    GUI_SIMPLE_MESSAGE_CONTENT_0_STRING_TRUNCATED,  /*!< a stringlength is exceeded, a string was truncated */
    GUI_SIMPLE_MESSAGE_CONTENT_0_NO_SELECTION,  /*!< nothing is selected, action could not be performed */
    GUI_SIMPLE_MESSAGE_CONTENT_0_DELETING_NOT_POSSIBLE,  /*!< some objects could not be deleted because they are still referenced */
    GUI_SIMPLE_MESSAGE_CONTENT_0_NO_MORE_UNDO,  /*!< undo failed, nothing more to undo */
    GUI_SIMPLE_MESSAGE_CONTENT_0_UNDO_NOT_POSSIBLE,  /*!< undo failed, list bounds of undo exceeded */
    GUI_SIMPLE_MESSAGE_CONTENT_0_NO_MORE_REDO,  /*!< redo failed, nothing more to undo */
    GUI_SIMPLE_MESSAGE_CONTENT_0_NO_INPUT_DATA,  /*!< paste failed, no input data */
    GUI_SIMPLE_MESSAGE_CONTENT_0_SET_PARTLY_UNSUITABLE,  /*!< operation cannot be performed on some elements in the set */
    GUI_SIMPLE_MESSAGE_CONTENT_0_DB_FILE_WRITE_ERROR,  /*!< database could not write to file */
    GUI_SIMPLE_MESSAGE_CONTENT_0_ANCESTOR_IS_NOT_DESCENDANT,  /*!< root/ancestor/parent diagram cannot move to descendant tree location */
    GUI_SIMPLE_MESSAGE_CONTENT_0_DEBUG_MODE,  /*!< program was compiled in debug mode and may therefore be slower */
    GUI_SIMPLE_MESSAGE_CONTENT_0_NO_RELATIONSHIPS,  /*!< current diagram type does not allow to create relationships */
    GUI_SIMPLE_MESSAGE_CONTENT_0_NO_FEATURES,  /*!< current diagram type does not allow to create features */
    GUI_SIMPLE_MESSAGE_CONTENT_0_FEATURELESS_CLASSIFIER,  /*!< current classifier type does not allow to create features */
    GUI_SIMPLE_MESSAGE_CONTENT_0_IS_ALWAYS_INSTANCE,  /*!< current classifier type does not allow to remove the instance flag */
};

typedef enum gui_simple_message_content_enum gui_simple_message_content_t;

/*! \brief tag-type to identify constants that may be used only at gui_simple_message_to_user_show_message_with_quantity method.
 *
 *  This type allows the compiler to check if a constant is used in the right function context.
 *
 *  The idea is similar to C++ tag dispatching.
 */
typedef struct gui_simple_message_content_quantity_struct { const int quantity; } gui_simple_message_content_quantity_t;

extern const gui_simple_message_content_quantity_t *GUI_SIMPLE_MESSAGE_CONTENT_DB_INCONSISTENT;  /*!< the current database file is inconsistent */
extern const gui_simple_message_content_quantity_t *GUI_SIMPLE_MESSAGE_CONTENT_MAX_WINDOWS_ALREADY_OPEN;  /*!< the maximum number of windows is already open */

/*! \brief tag-type to identify constants that may be used only at gui_simple_message_to_user_show_message_with_position method.
 *
 *  This type allows the compiler to check if a constant is used in the right function context.
 *
 *  The idea is similar to C++ tag dispatching.
 */
typedef struct gui_simple_message_content_position_struct { const int stream_position; } gui_simple_message_content_position_t;

extern const gui_simple_message_content_position_t *GUI_SIMPLE_MESSAGE_CONTENT_INVALID_INPUT_DATA;  /*!< paste failed, parser error at input data */

/*! \brief tag-type to identify constants that may be used only at gui_simple_message_to_user_show_message_with_name method.
 *
 *  This type allows the compiler to check if a constant is used in the right function context.
 *
 *  The idea is similar to C++ tag dispatching.
 */
typedef struct gui_simple_message_content_name_struct { const char *name; } gui_simple_message_content_name_t;

extern const gui_simple_message_content_name_t *GUI_SIMPLE_MESSAGE_CONTENT_DB_FILE_NOT_OPENED;  /*!< the chosen database file cannot be used */
extern const gui_simple_message_content_name_t *GUI_SIMPLE_MESSAGE_CONTENT_DB_FILE_OPENED_WITH_ERROR;  /*!< the chosen database file was opened but with a warning/error */
extern const gui_simple_message_content_name_t *GUI_SIMPLE_MESSAGE_CONTENT_FILE_EXPORT_FAILED;  /*!< some or all diagrams could not be exported */
extern const gui_simple_message_content_name_t *GUI_SIMPLE_MESSAGE_CONTENT_NAME_NOT_UNIQUE;  /*!< object not created or name not changed */

/*! \brief tag-type to identify constants that may be used only at gui_simple_message_to_user_show_message_with_names method.
 *
 *  This type allows the compiler to check if a constant is used in the right function context.
 *
 *  The idea is similar to C++ tag dispatching.
 */
typedef struct gui_simple_message_content_names_struct { const char *list_of_names; } gui_simple_message_content_names_t;

/*! \brief tag-type to identify constants that may be used only at gui_simple_message_to_user_show_message_with_error method.
 *
 *  This type allows the compiler to check if a constant is used in the right function context.
 *
 *  The idea is similar to C++ tag dispatching.
 */
typedef struct gui_simple_message_content_error_struct { const char *error_message; } gui_simple_message_content_error_t;

extern const gui_simple_message_content_error_t *GUI_SIMPLE_MESSAGE_CONTENT_NOT_YET_IMPLEMENTED;  /*!< the feature is not yet implemented */

/*! \brief tag-type to identify constants that may be used only at gui_simple_message_to_user_show_message_with_stat method.
 *
 *  This type allows the compiler to check if a constant is used in the right function context.
 *
 *  The idea is similar to C++ tag dispatching.
 */
typedef struct gui_simple_message_content_stat_struct { const data_stat_t *stat; } gui_simple_message_content_stat_t;

extern const gui_simple_message_content_stat_t *GUI_SIMPLE_MESSAGE_CONTENT_CUT_TO_CLIPBOARD;  /*!< show statistics on cut elements */
extern const gui_simple_message_content_stat_t *GUI_SIMPLE_MESSAGE_CONTENT_COPY_TO_CLIPBOARD;  /*!< show statistics on copied elements */
extern const gui_simple_message_content_stat_t *GUI_SIMPLE_MESSAGE_CONTENT_PASTE_FROM_CLIPBOARD;  /*!< show statistics on pasted elements */
extern const gui_simple_message_content_stat_t *GUI_SIMPLE_MESSAGE_CONTENT_DELETE;  /*!< show statistics on deleted elements */
extern const gui_simple_message_content_stat_t *GUI_SIMPLE_MESSAGE_CONTENT_UNDO;  /*!< show statistics on undo actions */
extern const gui_simple_message_content_stat_t *GUI_SIMPLE_MESSAGE_CONTENT_REDO;  /*!< show statistics on redo actions */

/*! \brief tag-type to identify constants that may be used only at gui_simple_message_to_user_show_message_with_names_and_stat method.
 *
 *  This type allows the compiler to check if a constant is used in the right function context.
 *
 *  The idea is similar to C++ tag dispatching.
 */
typedef struct gui_simple_message_content_names_stat_struct { const char *list_of_names; const data_stat_t *stat; } gui_simple_message_content_names_stat_t;

extern const gui_simple_message_content_names_stat_t *GUI_SIMPLE_MESSAGE_CONTENT_EXPORT_FINISHED;  /*!< files exported successfully */

#endif  /* GUI_SIMPLE_MESSAGE_CONTENT_H */


/*
Copyright 2016-2020 Andreas Warnke

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
