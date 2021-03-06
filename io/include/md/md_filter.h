/* File: md_filter.h; Copyright and License: see below */

#ifndef MD_FILTER_H
#define MD_FILTER_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief parses some md-like and link features of the input string to an xml writer.
 *
 *  This class is a filter in the sense of the pipes-and-filters architecture pattern:
 *  It modifies and enriches the piped-through data.
 */

#include "xml/xml_writer.h"
#include "util/string/utf8stringbuf.h"
#include "storage/data_database_reader.h"
#include "data_diagram.h"

/*!
 *  \brief constants for max string sizes
 */
enum md_filter_max_enum {
    MD_FILTER_MAX_STRING_SIZE = DATA_DIAGRAM_MAX_DESCRIPTION_LENGTH + DATA_CLASSIFIER_MAX_DESCRIPTION_LENGTH,
};

/*!
 *  \brief attributes of the xml writer
 */
struct md_filter_struct {
    data_database_reader_t *db_reader;  /*!< pointer to external database reader */
    xml_writer_t *sink;  /*!< output sink */

    const char * tag_linebreak;  /*!< tag for linebreak */
    const char * tag_xref_start;  /*!< tag for xref_start */
    const char * tag_xref_middle;  /*!< tag for xref_middle */
    const char * tag_xref_end;  /*!< tag for xref_end */

    data_diagram_t temp_diagram;  /*!< memory for temporary diagram storage */
};

typedef struct md_filter_struct md_filter_t;

/*!
 *  \brief initializes the document exporter
 *
 *  \param this_ pointer to own object attributes
 *  \param db_reader pointer to a database reader object
 *  \param tag_linebreak tag for linebreak
 *  \param tag_xref_start tag for xref_start
 *  \param tag_xref_middle tag for xref_middle
 *  \param tag_xref_end tag for xref_end
 *  \param sink stream sink where to write the output to
 */
void md_filter_init( md_filter_t *this_,
                     data_database_reader_t *db_reader,
                     const char * tag_linebreak,
                     const char * tag_xref_start,
                     const char * tag_xref_middle,
                     const char * tag_xref_end,
                     xml_writer_t *sink
                   );

/*!
 *  \brief destroys the document exporter
 *
 *  \param this_ pointer to own object attributes
 */
void md_filter_destroy( md_filter_t *this_ );

/*!
 *  \brief parses the string, enriches it by links and paragraph-breaks and writes it to the sink
 *
 *  \param this_ pointer to own object attributes
 *  \param text string to parse
 *  \result 0 in case of success, -1 otherwise
 */
int md_filter_transform ( md_filter_t *this_, const char *text );

#endif  /* MD_FILTER_H */


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
