cmake_minimum_required(VERSION 3.14)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_subdirectory(lib)

add_library(AbstractorizerZauonlokRenderer
    zauonlokrenderer_widget.h
	zauonlokrenderer_widget.cpp
	zauonlokrenderer_widget_internal.h
	zauonlokrenderer_widget_internal.cpp
)

set_target_properties(AbstractorizerZauonlokRenderer PROPERTIES
    CXX_STANDARD 17
    CXX_EXTENSIONS OFF
)

target_compile_options(AbstractorizerZauonlokRenderer PUBLIC "/Zc:__cplusplus")

target_include_qt_or_cs(AbstractorizerZauonlokRenderer)
target_link_sdl2(AbstractorizerZauonlokRenderer)
target_link_libraries(AbstractorizerZauonlokRenderer PUBLIC ZauonlokViewer)
