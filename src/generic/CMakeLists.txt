cmake_minimum_required(VERSION 3.14)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_library(AbstractorizerGeneric
    engine_widget.h
    imgui_manager.h
)

set_target_properties(AbstractorizerGeneric PROPERTIES LINKER_LANGUAGE CXX)

target_include_qt(TARGET AbstractorizerGeneric COMPONENTS ${QT_LIBRARIES_REQUIRED})