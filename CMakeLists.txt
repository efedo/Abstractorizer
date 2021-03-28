# (c) 2021 Eric Fedosejevs and contributors. Licensed under MIT license (see LICENSE for details)

cmake_minimum_required(VERSION 3.15)

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/../Utilogeny/CMakeLists.txt")
	set(UTILOGENY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../Utilogeny" CACHE STRING "Path to Utilogeny")
else()
	set(UTILOGENY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lib/Utilogeny" CACHE STRING "Path to Utilogeny" FORCE)
endif()

set(ABSTRACTORIZER_DIR "${CMAKE_CURRENT_SOURCE_DIR}" CACHE STRING "Path to Abstractorizer")
include(${UTILOGENY_DIR}/cmake/setup_project.cmake)

pre_project_boilerplate(NAME "Abstractorizer")

project(Abstractorizer VERSION 0.1
                  DESCRIPTION "Embedded external graphics engines and GUIs in Qt"
                  LANGUAGES CXX C)
				  
post_project_boilerplate(NAME "Abstractorizer")

# configure a header file to pass the version number to the source code
configure_file(src/cmakeversion.h.in src/cmakeversion.h)

#if(NOT TARGET UtilogenyCore)
#	add_subdirectory(${UTILOGENY_DIR} ${CMAKE_BINARY_DIR}/lib/Utilogeny)
#endif()

add_subdirectory(src)

install(TARGETS AbstractorizerGUI DESTINATION bin)