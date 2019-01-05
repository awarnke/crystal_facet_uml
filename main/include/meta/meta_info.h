/* File: meta_info.h; Copyright and License: see below */

#ifndef META_INFO_H
#define META_INFO_H

/* public file for the doxygen documentation: */
/*!
 *  \file
 *  \brief Defines the own program name and the version
 */

#define META_INFO_VERSION_STR "1.9.0"
#define META_INFO_PROGRAM_NAME_STR "crystal facet uml"
#define META_INFO_PROGRAM_ID_STR "crystal_facet_uml"
#define META_INFO_LICENSE_STR "Apache-2.0"
#define META_INFO_COPYRIGHT_STR "2016-2019 A.Warnke"
#define META_INFO_LIB_LICENSES_STR "sqlite 3.21.0 (Public Domain)"
#define META_INFO_UNITTEST_LIB_LICENSES_STR "embunit 1.0.1 (by yusuke sasaki[arms22], MIT/X Consortium License)"

/* Note: when changing META_INFO_VERSION_STR, change also */
/*       release_notes.txt : new entry */
/*       doxygen_build/Doxyfile : PROJECT_NUMBER */
/*       user_doc/doc/crystal_facet_user_documentation.xml : date */
/*       package/build/source_pack.sh : VERSIONSTR */
/*       package/build/debian_pack.sh : VERSIONSTR */
/*       package_build/openSUSE_build_service_config/crystal_facet_uml.spec : Version */
/*       package_build/debian_config/debian/changelog : new entry, e.g. use dch -i in folder debian_config */

#endif  /* META_INFO_H */


/*
Copyright 2016-2019 Andreas Warnke

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
