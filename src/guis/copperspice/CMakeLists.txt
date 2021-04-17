cmake_minimum_required(VERSION 3.14)

set(WITH_GUI ON)
set(WITH_OPENGL ON)
set(WITH_MULTIMEDIA OFF)
set(WITH_WEBKIT OFF)
set(WITH_SVG OFF)
set(WITH_SQL OFF)
set(WITH_XML OFF)
set(WITH_XMLPATTERNS OFF)
set(WITH_NETWORK OFF)
set(WITH_SCRIPT OFF)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

#add_subdirectory(${ABSTRACTORIZER_DIR}/lib/CopperSpice "${CMAKE_BINARY_DIR}/lib/CopperSpice")

add_library(AbstractorizerCopperSpice STATIC
	copperspice.cpp
)

target_link_copperspice(TARGET AbstractorizerCopperSpice COMPONENTS "${COPPERSPICE_LIBRARIES_REQUIRED}")
#target_link_lvgl(AbstractorizerLVGL)
#target_link_sdl2(AbstractorizerLVGL)