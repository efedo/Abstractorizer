cmake_minimum_required(VERSION 3.14)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_library(AbstractorizerOgre
    ogre_widget.h
	ogre_widget.cpp
	ogre_widget_internal.h
	ogre_widget_internal.cpp
)

target_include_qt(TARGET AbstractorizerOgre COMPONENTS ${QT_LIBRARIES_REQUIRED})
target_link_sdl2(AbstractorizerOgre)