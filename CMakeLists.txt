cmake_minimum_required(VERSION 3.5)
project(GPR) # PROJECT_NAME variable
#set(CMAKE_VERBOSE_MAKEFILE ON)

###############################################################################
# Variables
###############################################################################

# Documentation repository
set(DOC_REP "${CMAKE_CURRENT_SOURCE_DIR}/doc")
# Graph repository
set(GRAPH_REP "${CMAKE_CURRENT_SOURCE_DIR}/extras/graph")
# Include respository
set(INC_REP "${CMAKE_CURRENT_SOURCE_DIR}/include")
# Library respository
set(LIB_REP "${CMAKE_CURRENT_SOURCE_DIR}/lib")
# Object repository
set(OBJ_REP "${CMAKE_CURRENT_SOURCE_DIR}/obj")
# Source respository
set(SRC_REP "${CMAKE_CURRENT_SOURCE_DIR}/src")
# Tests repository
set(TEST_REP "${CMAKE_CURRENT_SOURCE_DIR}/tests")

# Graph script
set(GRAPH_NAME "grapher.py")

# Headers
file(GLOB HEADERS CONFIGURE_DEPENDS "${INC_REP}/*.h")
# Sources
file(GLOB SOURCES CONFIGURE_DEPENDS "${SRC_REP}/*.c")
# Tests
file(GLOB TESTS CONFIGURE_DEPENDS "${TEST_REP}/*.c")

###############################################################################
# API targets
###############################################################################

# Build shared API
add_library(GPR SHARED ${SOURCES})
target_include_directories(GPR PUBLIC ${INC_REP})
target_compile_options(GPR PRIVATE -Wall -Wextra -Werror -O3)

# Build static API
add_library(GPR_static STATIC ${SOURCES})
target_include_directories(GPR_static PUBLIC ${INC_REP})
target_compile_options(GPR_static PRIVATE -Wall -Wextra -Werror -O3)

# Add debug flags
if (${CMAKE_BUILD_TYPE} MATCHES DEBUG)
    target_compile_options(GPR PRIVATE -g -DDEBUG)
    target_compile_options(GPR_static PRIVATE -g -DDEBUG)
endif ()

# Add system flag
if (WIN32)
    target_compile_options(GPR PRIVATE -DWIN32)
    target_compile_options(GPR_static PRIVATE -DWIN32)
endif()

if (APPLE)
    target_compile_options(GPR PRIVATE -DOSX)
    target_compile_options(GPR_static PRIVATE -DOSX)
endif()

if (UNIX AND NOT APPLE)
    target_compile_options(GPR PRIVATE -DUNIX)
    target_compile_options(GPR_static PRIVATE -DUNIX)
endif()

if (CYGWIN)
    target_compile_options(GPR PRIVATE -DCYGWIN)
    target_compile_options(GPR_static PRIVATE -DCYGWIN)
endif()

# Install shared API
if (WIN32)
    install(TARGETS GPR LIBRARY DESTINATION lib RUNTIME DESTINATION bin)
else()
    install(TARGETS GPR LIBRARY DESTINATION lib)
endif()

# Install static API
install(TARGETS GPR_static LIBRARY DESTINATION lib)

# Install headers API
install(DIRECTORY ${INC_REP}/ DESTINATION include/GPR)

# Build API unit tests
foreach(file ${TESTS})
    get_filename_component(UTNAME ${file} NAME_WLE)
    add_executable(${UTNAME} ${file})
    target_link_libraries(${UTNAME} PRIVATE GPR)
endforeach()

# Install API unit tests
foreach(file ${TESTS})
    get_filename_component(UTNAME ${file} NAME_WLE)
    install(TARGETS ${UTNAME} DESTINATION bin)
endforeach()

###############################################################################
# Custom targets
###############################################################################

find_package(Doxygen)
find_package(Python3 COMPONENTS Interpreter)

# make clear
add_custom_target(clear
    COMMAND ${CMAKE_MAKE_PROGRAM} clean)

if (DOXYGEN_FOUND)
    # make doc
    add_custom_target(doc
        COMMAND doxygen ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Generating API documentation with Doxygen..."
        VERBATIM)

    # make cleandoc
    add_custom_target(cleandoc
        COMMAND rm -rf ${DOC_REP}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Cleaning API documentation...")

    add_dependencies(clear cleandoc)
endif()

if (Python3_FOUND)
    # make graph
    add_custom_target(graph
        COMMAND python3 ${GRAPH_REP}/${GRAPH_NAME} ${INC_REP} ${SRC_REP} -n ${PROJECT_NAME} -m module -o ${GRAPH_REP} --known-only --view
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Generating API graph...")

    # make cleangraph
    add_custom_target(cleangraph
        COMMAND rm -rf ${GRAPH_REP}/*.gv ${GRAPH_REP}/*.pdf
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Cleaning API graph...")

    add_dependencies(clear cleangraph)
endif()

# make indent
add_custom_target(indent
    COMMAND clang-format -i -verbose -style=file ${HEADERS} ${SOURCES} ${TESTS}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Indenting API files...")

# make uninstall
add_custom_target(uninstall
    COMMAND xargs rm < install_manifest.txt
    COMMENT "Uninstalling API...")
