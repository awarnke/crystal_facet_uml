/* File: gui_resources.c; Copyright and License: see below */

#include "gui_resources.h"
#include "trace.h"
#include <gtk/gtk.h>
#include <stdbool.h>

/* including resource files */
#include "resources/crystal_facet_uml.c"
#include "resources/edit_commit.c"
#include "resources/file_new_window.c"
#include "resources/message_error.c"
#include "resources/message_info.c"
#include "resources/message_warn.c"
#include "resources/tool_create_diagram.c"
#include "resources/tool_navigate.c"
#include "resources/tool_create_object.c"
#include "resources/tool_edit.c"

void gui_resources_init ( gui_resources_t *this_ )
{
    TRACE_BEGIN();

    (*this_).crystal_facet_uml = gdk_pixbuf_new_from_data(
        &(crystal_facet_uml.pixel_data[0]),
        GDK_COLORSPACE_RGB,
        true /* alpha */,
        8,
        crystal_facet_uml.width,
        crystal_facet_uml.height,
        crystal_facet_uml.width * crystal_facet_uml.bytes_per_pixel,
        NULL,
        NULL
    );

    (*this_).edit_commit = gdk_pixbuf_new_from_data(
        &(edit_commit.pixel_data[0]),
        GDK_COLORSPACE_RGB,
        true /* alpha */,
        8,
        edit_commit.width,
        edit_commit.height,
        edit_commit.width * edit_commit.bytes_per_pixel,
        NULL,
        NULL
    );

    (*this_).file_new_window = gdk_pixbuf_new_from_data(
        &(file_new_window.pixel_data[0]),
        GDK_COLORSPACE_RGB,
        true /* alpha */,
        8,
        file_new_window.width,
        file_new_window.height,
        file_new_window.width * file_new_window.bytes_per_pixel,
        NULL,
        NULL
    );

    (*this_).message_error = gdk_pixbuf_new_from_data(
        &(message_error.pixel_data[0]),
        GDK_COLORSPACE_RGB,
        true /* alpha */,
        8,
        message_error.width,
        message_error.height,
        message_error.width * message_error.bytes_per_pixel,
        NULL,
        NULL
    );

    (*this_).message_info = gdk_pixbuf_new_from_data(
        &(message_info.pixel_data[0]),
        GDK_COLORSPACE_RGB,
        true /* alpha */,
        8,
        message_info.width,
        message_info.height,
        message_info.width * message_info.bytes_per_pixel,
        NULL,
        NULL
    );

    (*this_).message_warn = gdk_pixbuf_new_from_data(
        &(message_warn.pixel_data[0]),
        GDK_COLORSPACE_RGB,
        true /* alpha */,
        8,
        message_warn.width,
        message_warn.height,
        message_warn.width * message_warn.bytes_per_pixel,
        NULL,
        NULL
    );

    (*this_).tool_create_diagram = gdk_pixbuf_new_from_data(
        &(tool_create_diagram.pixel_data[0]),
        GDK_COLORSPACE_RGB,
        true /* alpha */,
        8,
        tool_create_diagram.width,
        tool_create_diagram.height,
        tool_create_diagram.width * tool_create_diagram.bytes_per_pixel,
        NULL,
        NULL
    );

    (*this_).tool_navigate = gdk_pixbuf_new_from_data(
        &(tool_navigate.pixel_data[0]),
        GDK_COLORSPACE_RGB,
        true /* alpha */,
        8,
        tool_navigate.width,
        tool_navigate.height,
        tool_navigate.width * tool_navigate.bytes_per_pixel,
        NULL,
        NULL
    );

    (*this_).tool_create_object = gdk_pixbuf_new_from_data(
        &(tool_create_object.pixel_data[0]),
        GDK_COLORSPACE_RGB,
        true /* alpha */,
        8,
        tool_create_object.width,
        tool_create_object.height,
        tool_create_object.width * tool_create_object.bytes_per_pixel,
        NULL,
        NULL
    );

    (*this_).tool_edit = gdk_pixbuf_new_from_data(
        &(tool_edit.pixel_data[0]),
        GDK_COLORSPACE_RGB,
        true /* alpha */,
        8,
        tool_edit.width,
        tool_edit.height,
        tool_edit.width * tool_edit.bytes_per_pixel,
        NULL,
        NULL
    );

    TRACE_END();
}

/*!
 *  \brief destroys the gui_resources_t
 */
void gui_resources_destroy ( gui_resources_t *this_ )
{
    TRACE_BEGIN();
    TRACE_END();
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
