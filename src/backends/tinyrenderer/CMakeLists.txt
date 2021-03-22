cmake_minimum_required(VERSION 3.14)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_library(AbstractorizerTinyRenderer
    tinyrenderer_widget.h
	tinyrenderer_widget.cpp
	tinyrenderer_widget_internal.h
	tinyrenderer_widget_internal.cpp
	lib/geometry.cpp
	lib/geometry.h
	lib/main.h
	lib/model.cpp
	lib/model.h
	lib/our_gl.cpp
	lib/our_gl.h
	lib/tgaimage.cpp
	lib/tgaimage.h
	 "argb.h")

target_include_qt(TARGET AbstractorizerTinyRenderer COMPONENTS ${QT_LIBRARIES_REQUIRED})
target_link_sdl2(AbstractorizerTinyRenderer)