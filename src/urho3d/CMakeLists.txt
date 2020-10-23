cmake_minimum_required(VERSION 3.10)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

link_directories(${CMAKE_LIBRARY_OUTPUT_DIRECTORY})

set(SOURCES 
    "../generic/engine_window.h"
    ../generic/imgui_manager.h
    urho3d_window.h
    urho3d_window.cpp
 "urho3d_widget.h" "urho3d_widget.cpp")

foreach(CurrTargetType ${AllTargetTypes})
    add_library(urho3d-${CurrTargetType} ${SOURCES})
endforeach()

# Set CMake modules search path
set (CMAKE_MODULE_PATH "C:/Programming/lib/Urho3D/CMake/Modules")

# Include Urho3D Cmake common module
include (UrhoCommon)

foreach(CurrTargetType ${AllTargetTypes})

    # Define target name
    #set (TARGET_NAME ${ProjectName}-${CurrTargetType})

    # Define source files
    #define_source_files ()

    # Setup target with resource copying
    #setup_main_executable ()

endforeach()

#add_subdirectory("C:/Programming/lib/Urho3D/" [${CMAKE_LIBRARY_OUTPUT_DIRECTORY}])