/* File: gui_textedit.inl; Copyright and License: see below */

static inline GtkTreeModel *gui_textedit_get_diagram_types_ptr ( gui_textedit_t *this_ )
{
    return GTK_TREE_MODEL( (*this_).diagram_types );
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