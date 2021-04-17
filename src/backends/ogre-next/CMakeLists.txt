cmake_minimum_required(VERSION 3.14)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_library(AbstractorizerOgreNext
    ogrenext_widget.h
	ogrenext_widget.cpp
	ogrenext_widget_internal.h
	ogrenext_widget_internal.cpp
)

target_include_qt_or_cs(AbstractorizerOgreNext)
target_link_sdl2(AbstractorizerOgreNext)