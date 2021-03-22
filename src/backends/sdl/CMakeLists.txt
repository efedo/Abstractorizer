cmake_minimum_required(VERSION 3.14)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_library(AbstractorizerSDL
    sdl_widget.h
	sdl_widget.cpp
	sdl_widget_internal.h
	sdl_widget_internal.cpp
	 "sdl_utils.h" "sdl_utils.cpp")

target_include_qt(TARGET AbstractorizerSDL COMPONENTS ${QT_LIBRARIES_REQUIRED})
target_link_sdl2(AbstractorizerSDL)