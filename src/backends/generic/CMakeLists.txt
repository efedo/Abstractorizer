cmake_minimum_required(VERSION 3.14)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_library(AbstractorizerGeneric
    engine_widget.h
	engine_spawner.h
	engine_widget_internal.h
	engine_widget_internal.cpp
    imgui_manager.h
	engine_features.h
)

set_target_properties(AbstractorizerGeneric PROPERTIES
    CXX_STANDARD 17
    CXX_EXTENSIONS OFF
)

set_target_properties(AbstractorizerGeneric PROPERTIES LINKER_LANGUAGE CXX)

target_compile_options(AbstractorizerGeneric PUBLIC "/Zc:__cplusplus")

target_include_qt_or_cs(AbstractorizerGeneric)