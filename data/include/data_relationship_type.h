/* File: data_relationship_type.h; Copyright and License: see below */

#ifndef DATA_RELATIONSHIP_TYPE_H
#define DATA_RELATIONSHIP_TYPE_H

/* public file for the doxygen documentation: */
/*! \file */

/*!
 *  \brief enumeration of all relationship types
 */
enum data_relationship_type_enum {
    DATA_RELATIONSHIP_TYPE_UML_DEPENDENCY = 0,
    /* class diagram */
    DATA_RELATIONSHIP_TYPE_UML_ASSOCIATION = 200,
    DATA_RELATIONSHIP_TYPE_UML_AGGREGATION = 201,
    DATA_RELATIONSHIP_TYPE_UML_COMPOSITION = 202,
    DATA_RELATIONSHIP_TYPE_UML_GENERALIZATION = 210,  /*!< equals inherits from */
    DATA_RELATIONSHIP_TYPE_UML_REALIZATION = 211,  /*!< equals abstraction, use this also for interface realization */
    /* sequence diagram */
    DATA_RELATIONSHIP_TYPE_UML_ASYNC_CALL = 220,  /*!< equals message or signal */
    DATA_RELATIONSHIP_TYPE_UML_SYNC_CALL = 221,  /*!< equals function call */
    DATA_RELATIONSHIP_TYPE_UML_RETURN_CALL = 222,  /*!< equals function result */
    /* composition diagram */
    DATA_RELATIONSHIP_TYPE_UML_COMMUNICATION_PATH = 230,
    /* activity diagram */
    DATA_RELATIONSHIP_TYPE_UML_CONTROL_FLOW = 240,
    DATA_RELATIONSHIP_TYPE_UML_OBJECT_FLOW = 241,
    /* deployment diagram */
    DATA_RELATIONSHIP_TYPE_UML_DEPLOY = 250,
    DATA_RELATIONSHIP_TYPE_UML_MANIFEST = 251,
    /* use case diagram */
    DATA_RELATIONSHIP_TYPE_UML_EXTEND = 260,
    DATA_RELATIONSHIP_TYPE_UML_INCLUDE = 261,
};

typedef enum data_relationship_type_enum data_relationship_type_t;

#endif  /* DATA_RELATIONSHIP_TYPE_H */


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