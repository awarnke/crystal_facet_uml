PROJECT(crystal-facet-uml LANGUAGES C)
CMAKE_MINIMUM_REQUIRED(VERSION 3.0)

#helper to find include directories and libraries:
FIND_PACKAGE(PkgConfig REQUIRED)

# ==== ==== ==== ==== ==== ==== ==== ==== Build Environment Settings ==== ==== ==== ==== ==== ==== ==== ====

SET(COMPILE_IN_INCLUDE_DIR "" CACHE FILEPATH "Path to additional include directory")
SET(COMPILE_IN_SOURCE_FILE "" CACHE FILEPATH "Path to additional source file, e.g. sqlite3.c")
INCLUDE_DIRECTORIES(${COMPILE_IN_INCLUDE_DIR})

# crystal-facet-uml

INCLUDE_DIRECTORIES(gui/include)
INCLUDE_DIRECTORIES(main/include)
INCLUDE_DIRECTORIES(tslog/include)
INCLUDE_DIRECTORIES(trace/include)
INCLUDE_DIRECTORIES(ctrl/include)
INCLUDE_DIRECTORIES(data/include)
INCLUDE_DIRECTORIES(utf8stringbuf/include)
INCLUDE_DIRECTORIES(pencil/include)
INCLUDE_DIRECTORIES(io/include)
INCLUDE_DIRECTORIES(universal/include)

#sqlite3 dependencies

PKG_SEARCH_MODULE(SQLITE3 REQUIRED sqlite3)
INCLUDE_DIRECTORIES(${SQLITE3_INCLUDE_DIRS})
#MESSAGE("SQLITE3: ${SQLITE3_INCLUDE_DIRS}")

#gtk3 dependencies

PKG_SEARCH_MODULE(GLIB2 REQUIRED glib-2.0)
INCLUDE_DIRECTORIES(${GLIB2_INCLUDE_DIRS})
#MESSAGE("GLIB2:   ${GLIB2_INCLUDE_DIRS}")

PKG_SEARCH_MODULE(GTK3 REQUIRED gtk+-3.0)
INCLUDE_DIRECTORIES(${GTK3_INCLUDE_DIRS})
#MESSAGE("GTK3:    ${GTK3_INCLUDE_DIRS}")
#MESSAGE("GTK3:    ${GTK3_FOUND}")
#MESSAGE("GTK3:    ${GTK3_LIBRARIES}")
#MESSAGE("GTK3:    ${GTK3_LIBRARY_DIRS}")
#MESSAGE("GTK3:    ${GTK3_LDFLAGS}")
#MESSAGE("GTK3:    ${GTK3_LDFLAGS_OTHER}")
#MESSAGE("GTK3:    ${GTK3_CFLAGS}")
#MESSAGE("GTK3:    ${GTK3_CFLAGS_OTHER}")

PKG_SEARCH_MODULE(PANGOCAIRO REQUIRED pangocairo)
INCLUDE_DIRECTORIES(${PANGOCAIRO_INCLUDE_DIRS})
#MESSAGE("PANGOCAIRO: ${PANGOCAIRO_INCLUDE_DIRS}")

FILE(GLOB commonSource gui/source/*.c
                       gui/source/sketch_area/*.c
                       trace/source/*.c
                       ctrl/source/*.c
                       data/source/*.c
                       data/source/storage/*.c
                       data/source/set/*.c
                       utf8stringbuf/source/util/string/*.c
                       universal/source/stream/*.c
                       pencil/source/*.c
                       pencil/source/util/geometry/*.c
                       pencil/source/draw/*.c
                       io/source/*.c
                       io/source/image/*.c
                       io/source/json/*.c
                       io/source/xml/*.c
                       io/source/txt/*.c
                       io/source/xmi/*.c
                       io/source/md/*.c)
FILE(GLOB appSource    main/source/*.c)
FILE(GLOB utestSource  main/test/*.c
                       data/test/unit/*.c
                       ctrl/test/unit/*.c
                       gui/test/unit/*.c
                       io/test/unit/*.c
                       io/test/integration/*.c
                       pencil/test/unit/*.c
                       pencil/test/integration/*.c
                       universal/test/unit/*.c
                       utf8stringbuf/test/unit/*.c)

# ==== ==== ==== ==== ==== ==== ==== ==== Check if CMAKE_BUILD_TYPE is set ==== ==== ==== ==== ==== ==== ==== ====

IF(NOT CMAKE_BUILD_TYPE)
  MESSAGE( "Warn: CMAKE_BUILD_TYPE no set!                 setting(CMAKE_BUILD_TYPE \"Release\")")
  SET(CMAKE_BUILD_TYPE "Release")
ENDIF(NOT CMAKE_BUILD_TYPE)
MESSAGE( "Note:                                          CMAKE_BUILD_TYPE = ${CMAKE_BUILD_TYPE}" )
MESSAGE( "Note: to build a release, run                  cmake -DCMAKE_BUILD_TYPE=Release " ${CMAKE_CURRENT_SOURCE_DIR} )
MESSAGE( "Note: to build with asserts and traces, run    cmake -DCMAKE_BUILD_TYPE=Debug " ${CMAKE_CURRENT_SOURCE_DIR} )

# ==== ==== ==== ==== ==== ==== ==== ==== Common Settings ==== ==== ==== ==== ==== ==== ==== ====

SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11 -D_XOPEN_SOURCE=700 -O2 -pedantic -g -Wall -Wno-overlength-strings ${GTK3_CFLAGS_OTHER}" )
# Note: The GIMP-generated images are longer that 4095 characters, therefore -Wno-overlength-strings is required here.

# ==== ==== ==== ==== ==== ==== ==== ==== EXE Target Settings ==== ==== ==== ==== ==== ==== ==== ====

ADD_EXECUTABLE(crystal-facet-uml ${appSource} ${commonSource} ${COMPILE_IN_SOURCE_FILE})
TARGET_LINK_LIBRARIES(crystal-facet-uml ${GTK3_LIBRARIES} ${GLIB2_LIBRARIES} ${PANGOCAIRO_LIBRARIES} ${SQLITE3_LIBRARIES} m)
SET_TARGET_PROPERTIES(crystal-facet-uml PROPERTIES LINKER_LANGUAGE C)

# ==== ==== ==== ==== ==== ==== ==== ==== test Target Settings ==== ==== ==== ==== ==== ==== ==== ====

ADD_EXECUTABLE(test_crystal-facet-uml ${utestSource} ${commonSource} ${COMPILE_IN_SOURCE_FILE})
TARGET_INCLUDE_DIRECTORIES(test_crystal-facet-uml PUBLIC test_fw/include)
TARGET_INCLUDE_DIRECTORIES(test_crystal-facet-uml PUBLIC data/test)
TARGET_INCLUDE_DIRECTORIES(test_crystal-facet-uml PUBLIC ctrl/test)
TARGET_INCLUDE_DIRECTORIES(test_crystal-facet-uml PUBLIC gui/test)
TARGET_INCLUDE_DIRECTORIES(test_crystal-facet-uml PUBLIC io/test)
TARGET_INCLUDE_DIRECTORIES(test_crystal-facet-uml PUBLIC pencil/test)
TARGET_INCLUDE_DIRECTORIES(test_crystal-facet-uml PUBLIC universal/test)
TARGET_INCLUDE_DIRECTORIES(test_crystal-facet-uml PUBLIC utf8stringbuf/test)
TARGET_LINK_LIBRARIES(test_crystal-facet-uml ${GTK3_LIBRARIES} ${GLIB2_LIBRARIES} ${PANGOCAIRO_LIBRARIES} ${SQLITE3_LIBRARIES} m)
SET_TARGET_PROPERTIES(test_crystal-facet-uml PROPERTIES LINKER_LANGUAGE C)

# ==== ==== ==== ==== ==== ==== ==== ==== Install Settings ==== ==== ==== ==== ==== ==== ==== ====

INSTALL(TARGETS crystal-facet-uml DESTINATION "/usr/bin/")
INSTALL(FILES "${CMAKE_CURRENT_SOURCE_DIR}/installation_linux/crystal-facet-uml.desktop" DESTINATION "/usr/share/applications/")
INSTALL(FILES "${CMAKE_CURRENT_SOURCE_DIR}/installation_linux/crystal-facet-uml.metainfo.xml" DESTINATION "/usr/share/metainfo/")
INSTALL(FILES "${CMAKE_CURRENT_SOURCE_DIR}/gui/source/resources/crystal_facet_uml_80x80.png" DESTINATION "/usr/share/pixmaps/" RENAME "crystal-facet-uml.png")
INSTALL(FILES "${CMAKE_CURRENT_SOURCE_DIR}/user_doc/crystal-facet-uml.1.gz" DESTINATION "/usr/share/man/man1/")

# ==== ==== ==== ==== ==== ==== ==== ==== gcov Target Settings ==== ==== ==== ==== ==== ==== ==== ====

if(CFU_ADD_GCOV_TARGET)
    if(CMAKE_COMPILER_IS_GNUCC)
        SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fprofile-arcs -ftest-coverage" )
        SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fprofile-arcs" )
        ADD_EXECUTABLE(gcov_crystal-facet-uml ${utestSource} ${commonSource} ${COMPILE_IN_SOURCE_FILE})
        TARGET_INCLUDE_DIRECTORIES(gcov_crystal-facet-uml PUBLIC test_fw/include)
        TARGET_INCLUDE_DIRECTORIES(gcov_crystal-facet-uml PUBLIC data/test)
        TARGET_INCLUDE_DIRECTORIES(gcov_crystal-facet-uml PUBLIC ctrl/test)
        TARGET_INCLUDE_DIRECTORIES(gcov_crystal-facet-uml PUBLIC gui/test)
        TARGET_INCLUDE_DIRECTORIES(gcov_crystal-facet-uml PUBLIC io/test)
        TARGET_INCLUDE_DIRECTORIES(gcov_crystal-facet-uml PUBLIC pencil/test)
        TARGET_INCLUDE_DIRECTORIES(gcov_crystal-facet-uml PUBLIC universal/test)
        TARGET_INCLUDE_DIRECTORIES(gcov_crystal-facet-uml PUBLIC utf8stringbuf/test)
        TARGET_LINK_LIBRARIES(gcov_crystal-facet-uml ${GTK3_LIBRARIES} ${GLIB2_LIBRARIES} ${PANGOCAIRO_LIBRARIES} ${SQLITE3_LIBRARIES} m)
        SET_TARGET_PROPERTIES(gcov_crystal-facet-uml PROPERTIES LINKER_LANGUAGE C)
        SET_TARGET_PROPERTIES(gcov_crystal-facet-uml PROPERTIES EXCLUDE_FROM_ALL TRUE)
    endif(CMAKE_COMPILER_IS_GNUCC)
endif(CFU_ADD_GCOV_TARGET)
