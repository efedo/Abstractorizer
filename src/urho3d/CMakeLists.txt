cmake_minimum_required(VERSION 3.14)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

#set(URHO3D_DIR "C:/Programming/lib/Urho3D" CACHE STRING "Urho3D directory")

if (COMMAND cmake_policy)
    # Libraries linked via full path no longer produce linker search paths
    cmake_policy (SET CMP0003 NEW)
    # INTERFACE_LINK_LIBRARIES defines the link interface
    cmake_policy (SET CMP0022 NEW)
    # Disallow use of the LOCATION target property - so we set to OLD as we still need it
    cmake_policy (SET CMP0026 OLD)
    # MACOSX_RPATH is enabled by default
    cmake_policy (SET CMP0042 NEW)
    # Honor the visibility properties for SHARED target types only
    cmake_policy (SET CMP0063 OLD)
endif ()

add_library(AbstractorizerUrho3D
    "urho3d_widget_internal.h"
    "urho3d_widget_internal.cpp"
    "urho3d_widget.h" 
    "urho3d_widget.cpp"
 )
 
target_include_qt(TARGET AbstractorizerUrho3D COMPONENTS ${QT_LIBRARIES_REQUIRED})
target_include_urho3d(AbstractorizerUrho3D)