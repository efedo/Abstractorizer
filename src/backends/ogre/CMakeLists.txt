cmake_minimum_required(VERSION 3.14)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_library(AbstractorizerOgre
    ogre_widget.h
	ogre_widget.cpp
	ogre_widget_internal.h
	ogre_widget_internal.cpp
)

target_include_qt_or_cs(AbstractorizerOgre)
target_link_sdl2(AbstractorizerOgre)