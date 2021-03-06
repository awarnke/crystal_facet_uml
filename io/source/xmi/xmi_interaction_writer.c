/* File: xmi_interaction_writer.c; Copyright and License: see below */

#include "xmi/xmi_interaction_writer.h"
#include "xmi/xmi_element_info_map.h"
#include "xmi/xmi_element_info.h"
#include "xmi/xmi_element_part.h"
#include "xmi/xmi_xml.h"
#include "util/string/utf8string.h"
#include "data_id.h"
#include "data_classifier_type.h"
#include "trace.h"
#include "tslog.h"
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

/* Note: when writing, each method typically starts with writing a newline, it does not end with writing a newline. */

void xmi_interaction_writer_init ( xmi_interaction_writer_t *this_,
                                   data_database_reader_t *db_reader,
                                   data_stat_t *io_export_stat,
                                   xml_writer_t *out_writer  )
{
    TRACE_BEGIN();
    assert( NULL != db_reader );
    assert( NULL != io_export_stat );
    assert( NULL != out_writer );

    (*this_).mode = IO_WRITER_PASS_BASE;
    (*this_).export_stat = io_export_stat;
    (*this_).xml_writer = out_writer;
    xmi_type_converter_init( &((*this_).xmi_types) );
    xmi_atom_writer_init( &((*this_).atom_writer), db_reader, (*this_).xml_writer );

    TRACE_END();
}

void xmi_interaction_writer_destroy( xmi_interaction_writer_t *this_ )
{
    TRACE_BEGIN();

    xmi_atom_writer_destroy( &((*this_).atom_writer) );
    xmi_type_converter_destroy( &((*this_).xmi_types) );
    xml_writer_destroy( (*this_).xml_writer );

    TRACE_END();
}

int xmi_interaction_writer_start_diagram( xmi_interaction_writer_t *this_,
                                          data_classifier_type_t parent_type, 
                                          const data_diagram_t *diagram_ptr )
{
    TRACE_BEGIN();
    assert ( NULL != diagram_ptr );
    int export_err = 0;

    const char *const diagram_name = data_diagram_get_name_ptr(diagram_ptr);
    const char *const diagram_descr = data_diagram_get_description_ptr(diagram_ptr);
    const size_t diagram_descr_len = utf8string_get_length(diagram_descr);
    const data_id_t diagram_id = data_diagram_get_data_id(diagram_ptr);

    if ( (*this_).mode == IO_WRITER_PASS_BASE )
    {
        /* determine nesting tag */
        const char* nesting_property;
        const int nesting_err
            = xmi_type_converter_get_xmi_nesting_property_of_classifier( &((*this_).xmi_types),
                                                                         parent_type,
                                                                         DATA_CLASSIFIER_TYPE_INTERACTION,  /* fake child type */
                                                                         &nesting_property
                                                                       );
        if ( nesting_err != 0 )
        {
            /* The caller requested to write an interaction to an illegal place. */
            /* This should not have happened, because latest the model is a valid nesting container */
            TRACE_INFO("xmi_element_writer: request to write an interaction to an illegal place!")
            assert(false);
            /* update export statistics */
            data_stat_inc_count ( (*this_).export_stat, DATA_TABLE_CLASSIFIER, DATA_STAT_SERIES_WARNING );
            /* inform the user via an XML comment: */
            export_err |= xmi_atom_writer_report_illegal_container( &((*this_).atom_writer),
                                                                    diagram_id,
                                                                    DATA_CLASSIFIER_TYPE_INTERACTION,  /* fake child type */
                                                                    parent_type
                                                                  );
            /* use a fallback */
            nesting_property = XMI_ELEMENT_PART_FALLBACK_NESTING_ELEMENT;
        }

        /* write nesting tag */
        export_err |= xml_writer_write_plain ( (*this_).xml_writer, XML_WRITER_NL );
        export_err |= xml_writer_write_plain ( (*this_).xml_writer, XML_WRITER_START_TAG_START );
        export_err |= xml_writer_write_plain ( (*this_).xml_writer, nesting_property );
        export_err |= xml_writer_write_plain ( (*this_).xml_writer, XML_WRITER_ATTR_SEPARATOR );
        xml_writer_increase_indent ( (*this_).xml_writer );

        /* write type attribute */
        export_err |= xml_writer_write_plain ( (*this_).xml_writer, XMI_XML_ATTR_TYPE_START );
        export_err |= xml_writer_write_plain ( (*this_).xml_writer, XMI_XML_NS_UML );
        /* TODO possibly a uml:Collaboration needs to be placed arounf the DATA_CLASSIFIER_TYPE_INTERACTION? */
        const char* c_type = xmi_type_converter_get_xmi_type_of_classifier ( &((*this_).xmi_types),
                                                                             parent_type,
                                                                             DATA_CLASSIFIER_TYPE_INTERACTION,  /* fake child type */
                                                                             XMI_SPEC_UML
                                                                           );
        export_err |= xml_writer_write_xml_enc ( (*this_).xml_writer, c_type );
        export_err |= xml_writer_write_plain ( (*this_).xml_writer, XMI_XML_ATTR_TYPE_END );

        /* write id attribute */
        export_err |= xml_writer_write_plain ( (*this_).xml_writer, XMI_XML_ATTR_ID_START );
        export_err |= xmi_atom_writer_encode_xmi_id( &((*this_).atom_writer), diagram_id );
        export_err |= xml_writer_write_plain ( (*this_).xml_writer, XMI_XML_ATTR_ID_END );

        /* write name attribute */
        export_err |= xml_writer_write_plain ( (*this_).xml_writer, XMI_XML_ATTR_NAME_START );
        export_err |= xml_writer_write_xml_enc ( (*this_).xml_writer, diagram_name );
        export_err |= xml_writer_write_plain ( (*this_).xml_writer, XMI_XML_ATTR_NAME_END );

        export_err |= xml_writer_write_plain ( (*this_).xml_writer, XML_WRITER_START_TAG_END );

        /* write real id comment */
        export_err |= xml_writer_write_plain ( (*this_).xml_writer, XML_WRITER_COMMENT_START );
        export_err |= xml_writer_write_plain_id( (*this_).xml_writer, diagram_id );
        export_err |= xml_writer_write_plain ( (*this_).xml_writer, XML_WRITER_COMMENT_END );

        /* write description */
        if ( 0 != diagram_descr_len )
        {
            export_err |= xmi_atom_writer_write_xmi_comment( &((*this_).atom_writer),
                                                             diagram_id,
                                                             "specification",
                                                             diagram_descr
                                                           );
        }

        /* update export statistics, report as classifier because DATA_CLASSIFIER_TYPE_INTERACTION is a classifier */
        data_stat_inc_count ( (*this_).export_stat, DATA_TABLE_CLASSIFIER, DATA_STAT_SERIES_EXPORTED );
    }

    TRACE_END_ERR( export_err );
    return export_err;
}

int xmi_interaction_writer_end_diagram( xmi_interaction_writer_t *this_,
                                        data_classifier_type_t parent_type )
{
    TRACE_BEGIN();
    int export_err = 0;

    if ( (*this_).mode == IO_WRITER_PASS_BASE )
    {
        /* determine nesting tag */
        const char* nesting_property;
        const int nesting_err
            = xmi_type_converter_get_xmi_nesting_property_of_classifier( &((*this_).xmi_types),
                                                                         parent_type,
                                                                         DATA_CLASSIFIER_TYPE_INTERACTION,  /* fake child type */
                                                                         &nesting_property
                                                                       );
        if ( nesting_err != 0 )
        {
            /* The caller requested to write a classifier to an illegal place */
            /* use a fallback */
            nesting_property = XMI_ELEMENT_PART_FALLBACK_NESTING_ELEMENT;
        }

        /* adjust indentation, write end tag */
        xml_writer_decrease_indent ( (*this_).xml_writer );
        export_err |= xml_writer_write_plain ( (*this_).xml_writer, XML_WRITER_NL );
        export_err |= xml_writer_write_plain ( (*this_).xml_writer, XML_WRITER_END_TAG_START );
        export_err |= xml_writer_write_plain ( (*this_).xml_writer, nesting_property );
        export_err |= xml_writer_write_plain ( (*this_).xml_writer, XML_WRITER_END_TAG_END );
    }

    TRACE_END_ERR( export_err );
    return export_err;
}

int xmi_interaction_writer_assemble_feature( xmi_interaction_writer_t *this_,
                                             data_id_t reference_id,
                                             data_classifier_type_t parent_type,
                                             const data_feature_t *feature_ptr )
{
    TRACE_BEGIN();
    assert ( NULL != feature_ptr );
    int export_err = 0;

    const data_id_t feature_id = data_feature_get_data_id( feature_ptr );

    if ( (*this_).mode == IO_WRITER_PASS_BASE )
    {
        export_err |= xml_writer_write_plain ( (*this_).xml_writer, XML_WRITER_NL );
        export_err |= xml_writer_write_plain ( (*this_).xml_writer, XML_WRITER_EMPTY_TAG_START );
        export_err |= xml_writer_write_xml_enc ( (*this_).xml_writer, "represents" );
        export_err |= xml_writer_write_plain ( (*this_).xml_writer, XML_WRITER_ATTR_SEPARATOR );
        
        /* write type attribute */
        export_err |= xml_writer_write_plain ( (*this_).xml_writer, XMI_XML_ATTR_TYPE_START );
        export_err |= xml_writer_write_plain ( (*this_).xml_writer, XMI_XML_NS_UML );
        export_err |= xml_writer_write_plain ( (*this_).xml_writer, "Property" );
        export_err |= xml_writer_write_plain ( (*this_).xml_writer, XMI_XML_ATTR_TYPE_END );

        /* write id attribute */
        export_err |= xml_writer_write_plain ( (*this_).xml_writer, XMI_XML_ATTR_ID_START );
        export_err |= xmi_atom_writer_encode_xmi_id( &((*this_).atom_writer), feature_id );
        export_err |= xml_writer_write_plain ( (*this_).xml_writer, "#ref" );
        export_err |= xml_writer_write_plain ( (*this_).xml_writer, XMI_XML_ATTR_ID_END );

        /* write lifeline id attribute */
        export_err |= xml_writer_write_plain ( (*this_).xml_writer, "type=\"" );
        export_err |= xmi_atom_writer_encode_xmi_id( &((*this_).atom_writer), reference_id );
        export_err |= xml_writer_write_plain ( (*this_).xml_writer, "\" " );

        export_err |= xml_writer_write_plain ( (*this_).xml_writer, XML_WRITER_EMPTY_TAG_END );
    }

    TRACE_END_ERR( export_err );
    return export_err;
}

int xmi_interaction_writer_assemble_relationship( xmi_interaction_writer_t *this_,
                                                  data_id_t interaction_id,
                                                  data_classifier_type_t parent_type,
                                                  const data_relationship_t *relation_ptr,
                                                  data_classifier_type_t from_c_type,
                                                  data_feature_type_t from_f_type,
                                                  data_classifier_type_t to_c_type,
                                                  data_feature_type_t to_f_type )
{
    TRACE_BEGIN();
    assert ( NULL != relation_ptr );
    /* NULL is allowed here: dest_classifier_ptr */
    int export_err = 0;

    const data_id_t relation_id = data_relationship_get_data_id( relation_ptr );
    const char *const relation_descr = data_relationship_get_description_ptr( relation_ptr );
    const size_t relation_descr_len = utf8string_get_length(relation_descr);
    const data_id_t from_classifier_id = data_relationship_get_from_classifier_data_id( relation_ptr );
    const data_id_t from_feature_id = data_relationship_get_from_feature_data_id( relation_ptr );
    const data_id_t to_classifier_id = data_relationship_get_to_classifier_data_id( relation_ptr );
    const data_id_t to_feature_id = data_relationship_get_to_feature_data_id( relation_ptr );
    const data_relationship_type_t relation_type = data_relationship_get_main_type( relation_ptr );

    if ( (*this_).mode == IO_WRITER_PASS_BASE )
    {
        if ( 0 != relation_descr_len )
        {
            export_err |= xmi_atom_writer_write_xmi_comment( &((*this_).atom_writer),
                                                             relation_id,
                                                             "specification",
                                                             relation_descr
                                                           );
        }

        /* source */
        /* determine from type tag */
        const char* from_type_tag;
        const int from_type_err
            = xmi_type_converter_get_xmi_from_property_of_relationship ( &((*this_).xmi_types),
                                                                         parent_type,
                                                                         relation_type,
                                                                         from_c_type,
                                                                         from_f_type,
                                                                         &from_type_tag
                                                                       );
        if ( from_type_err != 0 )
        {
            /* The caller requested to write a relationship of illegal source end type */
            TRACE_INFO("xmi_interaction_writer: request to write a relationship connecting an illegal source end type!")
            /* update export statistics */
            data_stat_inc_count ( (*this_).export_stat, DATA_TABLE_RELATIONSHIP, DATA_STAT_SERIES_WARNING );
            /* inform the user via an XML comment: */
            export_err |= xmi_atom_writer_report_illegal_relationship_end ( &((*this_).atom_writer),
                                                                            relation_id,
                                                                            relation_type,
                                                                            parent_type,
                                                                            true /* = from_end */,
                                                                            from_c_type,
                                                                            from_f_type
                                                                          );                
        }

        export_err |= xml_writer_write_plain ( (*this_).xml_writer, XML_WRITER_NL );
        export_err |= xml_writer_write_plain ( (*this_).xml_writer, XML_WRITER_EMPTY_TAG_START );
        export_err |= xml_writer_write_xml_enc ( (*this_).xml_writer, from_type_tag );
        export_err |= xml_writer_write_plain ( (*this_).xml_writer, XML_WRITER_ATTR_SEPARATOR );
        
        /* write type attribute */
        export_err |= xml_writer_write_plain ( (*this_).xml_writer, XMI_XML_ATTR_TYPE_START );
        export_err |= xml_writer_write_plain ( (*this_).xml_writer, XMI_XML_NS_UML );
        export_err |= xml_writer_write_plain ( (*this_).xml_writer, XMI_ELEMENT_PART_TYPE_MSG_OCCURRENCE_SPEC );
        export_err |= xml_writer_write_plain ( (*this_).xml_writer, XMI_XML_ATTR_TYPE_END );

        /* write id attribute */
        export_err |= xml_writer_write_plain ( (*this_).xml_writer, XMI_XML_ATTR_ID_START );
        export_err |= xmi_atom_writer_encode_xmi_id( &((*this_).atom_writer), relation_id );
        export_err |= xml_writer_write_plain ( (*this_).xml_writer, XMI_ELEMENT_PART_ID_FRAGMENT_SOURCE_END );
        export_err |= xml_writer_write_plain ( (*this_).xml_writer, XMI_XML_ATTR_ID_END );

        /* write lifeline id attribute */
        export_err |= xml_writer_write_plain ( (*this_).xml_writer, XMI_ELEMENT_PART_PROPERTY_OCCURRENCE_SPEC_COVERED );
        export_err |= xml_writer_write_plain ( (*this_).xml_writer, XML_WRITER_ATTR_VALUE_START );
        if ( data_id_is_valid( &from_feature_id ) )
        {
            export_err |= xmi_atom_writer_encode_xmi_id( &((*this_).atom_writer), from_feature_id );
        }
        else
        {
            export_err |= xmi_atom_writer_encode_xmi_id( &((*this_).atom_writer), from_classifier_id );
        }
        export_err |= xml_writer_write_plain ( (*this_).xml_writer, XML_WRITER_ATTR_VALUE_END );
        export_err |= xml_writer_write_plain ( (*this_).xml_writer, XML_WRITER_ATTR_SEPARATOR );

        /* write lifeline id attribute */
        export_err |= xml_writer_write_plain ( (*this_).xml_writer, "enclosingInteraction=\"" );
        export_err |= xmi_atom_writer_encode_xmi_id( &((*this_).atom_writer), interaction_id );
        export_err |= xml_writer_write_plain ( (*this_).xml_writer, "\" " );

        export_err |= xml_writer_write_plain ( (*this_).xml_writer, XML_WRITER_EMPTY_TAG_END );

        /* destination */
        /* determine from type tag */
        const char* to_type_tag;
        const int to_type_err
            = xmi_type_converter_get_xmi_to_property_of_relationship ( &((*this_).xmi_types),
                                                                       parent_type,
                                                                       relation_type,
                                                                       to_c_type,
                                                                       to_f_type,
                                                                       &to_type_tag
                                                                     );
        if ( to_type_err != 0 )
        {
            /* The caller requested to write a relationship of illegal target end type */
            TRACE_INFO("xmi_interaction_writer: request to write a relationship connecting an illegal target end type!")
            /* update export statistics */
            data_stat_inc_count ( (*this_).export_stat, DATA_TABLE_RELATIONSHIP, DATA_STAT_SERIES_WARNING );
            /* inform the user via an XML comment: */
            export_err |= xmi_atom_writer_report_illegal_relationship_end ( &((*this_).atom_writer),
                                                                            relation_id,
                                                                            relation_type,
                                                                            parent_type,
                                                                            false /* = from_end */,
                                                                            to_c_type,
                                                                            to_f_type
                                                                          );                
        }
            
        export_err |= xml_writer_write_plain ( (*this_).xml_writer, XML_WRITER_NL );
        export_err |= xml_writer_write_plain ( (*this_).xml_writer, XML_WRITER_EMPTY_TAG_START );
        export_err |= xml_writer_write_xml_enc ( (*this_).xml_writer, to_type_tag );
        export_err |= xml_writer_write_plain ( (*this_).xml_writer, XML_WRITER_ATTR_SEPARATOR );

        /* write type attribute */
        export_err |= xml_writer_write_plain ( (*this_).xml_writer, XMI_XML_ATTR_TYPE_START );
        export_err |= xml_writer_write_plain ( (*this_).xml_writer, XMI_XML_NS_UML );
        export_err |= xml_writer_write_plain ( (*this_).xml_writer, XMI_ELEMENT_PART_TYPE_MSG_OCCURRENCE_SPEC );
        export_err |= xml_writer_write_plain ( (*this_).xml_writer, XMI_XML_ATTR_TYPE_END );

        /* write id attribute */
        export_err |= xml_writer_write_plain ( (*this_).xml_writer, XMI_XML_ATTR_ID_START );
        export_err |= xmi_atom_writer_encode_xmi_id( &((*this_).atom_writer), relation_id );
        export_err |= xml_writer_write_plain ( (*this_).xml_writer, XMI_ELEMENT_PART_ID_FRAGMENT_TARGET_END );
        export_err |= xml_writer_write_plain ( (*this_).xml_writer, XMI_XML_ATTR_ID_END );

        /* write lifeline id attribute */
        export_err |= xml_writer_write_plain ( (*this_).xml_writer, XMI_ELEMENT_PART_PROPERTY_OCCURRENCE_SPEC_COVERED );
        export_err |= xml_writer_write_plain ( (*this_).xml_writer, XML_WRITER_ATTR_VALUE_START );
        if ( data_id_is_valid( &to_feature_id ) )
        {
            export_err |= xmi_atom_writer_encode_xmi_id( &((*this_).atom_writer), to_feature_id );
        }
        else
        {
            export_err |= xmi_atom_writer_encode_xmi_id( &((*this_).atom_writer), to_classifier_id );
        }
        export_err |= xml_writer_write_plain ( (*this_).xml_writer, XML_WRITER_ATTR_VALUE_END );
        export_err |= xml_writer_write_plain ( (*this_).xml_writer, XML_WRITER_ATTR_SEPARATOR );

        /* write lifeline id attribute */
        export_err |= xml_writer_write_plain ( (*this_).xml_writer, "enclosingInteraction=\"" );
        export_err |= xmi_atom_writer_encode_xmi_id( &((*this_).atom_writer), interaction_id );
        export_err |= xml_writer_write_plain ( (*this_).xml_writer, "\" " );

        export_err |= xml_writer_write_plain ( (*this_).xml_writer, XML_WRITER_EMPTY_TAG_END );

    }

    TRACE_END_ERR( export_err );
    return export_err;
}


/*
Copyright 2020-2021 Andreas Warnke

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
