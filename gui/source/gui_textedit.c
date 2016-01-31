/* File: gui_textedit.c; Copyright 2016-2016: Andreas Warnke; License: Apache 2.0 */

#include "gui_textedit.h"
#include "trace.h"
#include <gtk/gtk.h>

void btn_clear_callback( GtkButton* button, gpointer data ) {
    TRACE_BEGIN();

    gui_textedit_t (*this_) = (gui_textedit_t*) data;
    /*TRACE_DEBUG("clear\n");*/

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
