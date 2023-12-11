cmake_minimum_required(VERSION 3.14)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_library(AbstractorizerQt3D
    imgui/imgui_demo.cpp
    imgui/imgui_draw.cpp
    imgui/imgui.cpp
    gui.h
    gui.cpp
    qt3d_imgui_manager.h
    qt3d_imgui_manager.cpp
    qt3d_window.cpp
	qt3d_window.h
	qt3d_widget.cpp
    qt3d_widget.h 
    qt3d_widget_internal.cpp
	qt3d_widget_internal.h
)

set_target_properties(AbstractorizerQt3D PROPERTIES
    CXX_STANDARD 17
    CXX_EXTENSIONS OFF
)

target_compile_options(AbstractorizerQt3D PUBLIC "/Zc:__cplusplus")

target_include_qt(TARGET AbstractorizerQt3D COMPONENTS ${QT_LIBRARIES_REQUIRED})