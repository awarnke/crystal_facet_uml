/* File: gui_resources.inl; Copyright and License: see below */

static inline GdkPixbuf *gui_resources_get_tool_create_diagram ( gui_resources_t *this_ )
{
    return (*this_).tool_create_diagram;
}

static inline GdkPixbuf *gui_resources_get_tool_navigate ( gui_resources_t *this_ )
{
    return (*this_).tool_navigate;
}

static inline GdkPixbuf *gui_resources_get_tool_create_object ( gui_resources_t *this_ )
{
    return (*this_).tool_create_object;
}

static inline GdkPixbuf *gui_resources_get_tool_edit ( gui_resources_t *this_ )
{
    return (*this_).tool_edit;
}

static inline GdkPixbuf *gui_resources_get_edit_commit ( gui_resources_t *this_ )
{
    return (*this_).edit_commit;
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