cmake_minimum_required(VERSION 3.14)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_library(AbstractorizerSDL
    sdl_widget.h
	sdl_widget.cpp
	sdl_widget_internal.h
	sdl_widget_internal.cpp
	sdl_utils.h 
	sdl_utils.cpp)

set_target_properties(AbstractorizerSDL PROPERTIES
        CXX_STANDARD 17
        CXX_EXTENSIONS OFF
        )

target_compile_options(AbstractorizerSDL PUBLIC "/Zc:__cplusplus")

target_include_qt_or_cs(AbstractorizerSDL)
target_link_sdl2(AbstractorizerSDL)
target_config_compiler(AbstractorizerSDL)