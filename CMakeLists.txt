# Copyright 2021 Eric Fedosejevs
#

cmake_minimum_required(VERSION 3.15)

# These are temporary and will be replaced with find_package eventually
set(UTILOGENY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../Utilogeny" CACHE STRING "Path to Utilogeny" FORCE)
set(ABSTRACTORIZER_DIR "${CMAKE_CURRENT_SOURCE_DIR}" CACHE STRING "Path to Abstractorizer" FORCE)
include(${UTILOGENY_DIR}/cmake/setup_project.cmake)

pre_project_boilerplate(NAME "Abstractorizer")

project(Abstractorizer VERSION 0.1
                  DESCRIPTION "Compare embedded graphics/game engines in Qt"
                  LANGUAGES CXX)
				  
post_project_boilerplate(NAME "Abstractorizer")

# configure a header file to pass the version number to the source code
configure_file(src/cmakeversion.h.in src/cmakeversion.h)

#if(NOT TARGET UtilogenyCore)
#	add_subdirectory(${UTILOGENY_DIR} ${CMAKE_BINARY_DIR}/lib/Utilogeny)
#endif()

add_subdirectory(src)

install(TARGETS AbstractorizerGUI DESTINATION bin)