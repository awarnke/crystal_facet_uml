/* File: io_diagram_text_exporter.h; Copyright and License: see below */

#ifndef IO_DIAGRAM_TEXT_EXPORTER_H
#define IO_DIAGRAM_TEXT_EXPORTER_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Renders diagram and classifier descriptions to an output stream
 */

#include "io_format_writer.h"
#include "io_filter_flag.h"
#include "set/data_visible_set.h"
#include "data_table.h"
#include "data_rules.h"
#include "universal_array_list.h"
#include <stdio.h>

/*!
 *  \brief attributes of the description writer
 */
struct io_diagram_text_exporter_struct {
    /*data_database_reader_t *db_reader;*/  /* !< pointer to external database reader */
    const data_visible_set_t *input_data;  /*!< pointer to an external data cache */
    data_rules_t filter_rules;  /*!< own instance of uml and sysml consistency rules */
    io_filter_flag_t filter_flags;  /*!< flags indicating which elements shall be exported */
    
    data_id_t written_id_set_buf[50000];  /*!< buffer for list of already exported element ids */
    universal_array_list_t written_id_set;  /*!< list of already exported element ids */
};

typedef struct io_diagram_text_exporter_struct io_diagram_text_exporter_t;

/*!
 *  \brief initializes the io_diagram_text_exporter_t
 *
 *  \param this_ pointer to own object attributes
 *  \param filter_flags flags indicating which elements shall be exported
 *  \param input_data pointer to the (cached) data to be rendered
 */
void io_diagram_text_exporter_init( io_diagram_text_exporter_t *this_, 
                                    io_filter_flag_t filter_flags, 
                                    const data_visible_set_t *input_data
                                  );

/*!
 *  \brief re-initializes the io_diagram_text_exporter_t
 *
 *  \param this_ pointer to own object attributes
 *  \param filter_flags flags indicating which elements shall be exported
 *  \param input_data pointer to the (cached) data to be rendered
 */
void io_diagram_text_exporter_reinit( io_diagram_text_exporter_t *this_, 
                                      io_filter_flag_t filter_flags, 
                                      const data_visible_set_t *input_data 
                                    );

/*!
 *  \brief destroys the io_diagram_text_exporter_t
 *
 *  \param this_ pointer to own object attributes
 */
void io_diagram_text_exporter_destroy( io_diagram_text_exporter_t *this_ );

/*!
 *  \brief prints names and descriptions of the diagram and contained classifiers to the output stream
 *
 *  \param this_ pointer to own object attributes
 *  \param format_writer writer to format the data and stream it out to a file
 *  \return -1 in case of error, 0 in case of success
 */
int io_diagram_text_exporter_write_all ( io_diagram_text_exporter_t *this_, io_format_writer_t *format_writer );

/*!
 *  \brief prints names and descriptions of the classifiers to the output stream
 *
 *  \param this_ pointer to own object attributes
 *  \param format_writer writer to format the data and stream it out to a file
 *  \return -1 in case of error, 0 in case of success
 */
int io_diagram_text_exporter_write_classifiers ( io_diagram_text_exporter_t *this_, io_format_writer_t *format_writer );

/*!
 *  \brief prints names and descriptions of the classifier to the output stream
 *
 *  \param this_ pointer to own object attributes
 *  \param classifier_id id of the classifier of which the features are written
 *  \param format_writer writer to format the data and stream it out to a file
 *  \return -1 in case of error, 0 in case of success
 */
int io_diagram_text_exporter_private_write_features_of_classifier ( io_diagram_text_exporter_t *this_,
                                                                    data_id_t classifier_id,
                                                                    io_format_writer_t *format_writer
                                                                  );

/*!
 *  \brief prints names and descriptions of the relationships to the output stream
 *
 *  \param this_ pointer to own object attributes
 *  \param from_classifier_id id of the classifier of which the relationships are written
 *  \param format_writer writer to format the data and stream it out to a file
 *  \return -1 in case of error, 0 in case of success
 */
int io_diagram_text_exporter_private_write_relations_of_classifier ( io_diagram_text_exporter_t *this_,
                                                                     data_id_t from_classifier_id,
                                                                     io_format_writer_t *format_writer
                                                                   );

#endif  /* IO_DIAGRAM_TEXT_EXPORTER_H */


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
