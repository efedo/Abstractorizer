cmake_minimum_required(VERSION 3.10)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

link_directories(${CMAKE_LIBRARY_OUTPUT_DIRECTORY})

#message(STATUS "Include directories for " ${CMAKE_CURRENT_SOURCE_DIR} " :")
#get_directory_property(dirs INCLUDE_DIRECTORIES)
#foreach(dir ${dirs})
#  message(STATUS "dir='${dir}'")
#endforeach()

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

foreach(CurrTargetType ${AllTargetTypes})
    add_library(qt3d-${CurrTargetType} ${SOURCES})
endforeach()