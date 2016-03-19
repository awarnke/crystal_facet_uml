/* File: pencil_input_data.inl; Copyright and License: see below */

static inline data_diagram_t *pencil_input_data_get_diagram_ptr ( pencil_input_data_t *this_ )
{
    return &((*this_).diagram);
}

static inline bool pencil_input_data_is_valid ( pencil_input_data_t *this_ )
{
    return data_diagram_is_valid( &((*this_).diagram) );
}

static inline void pencil_input_data_invalidate ( pencil_input_data_t *this_ )
{
    data_diagram_init_empty( &((*this_).diagram) );
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
