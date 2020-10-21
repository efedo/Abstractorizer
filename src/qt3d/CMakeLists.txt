cmake_minimum_required(VERSION 3.15)

link_directories(${CMAKE_LIBRARY_OUTPUT_DIRECTORY})

find_package(QT NAMES Qt6 Qt5 COMPONENTS 3DCore 3DExtras 3DRender 3DInput REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS 3DCore 3DExtras 3DRender 3DInput REQUIRED)

set(SOURCES 
    imgui/imgui_demo.cpp
    imgui/imgui_draw.cpp
    imgui/imgui.cpp
    gui.h
    gui.cpp
    qt3d_imgui_manager.h
    qt3d_imgui_manager.cpp
    qt3d_imgui_window.h
    qt3d_imgui_window.cpp
)

add_library(qt3d ${SOURCES})