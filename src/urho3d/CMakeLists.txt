cmake_minimum_required(VERSION 3.14)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_library(AbstractorizerUrho3D
    "urho3d_widget_internal.h"
    "urho3d_widget_internal.cpp"
    "urho3d_widget.h" 
    "urho3d_widget.cpp"
 )
 
target_include_qt(TARGET AbstractorizerUrho3D COMPONENTS ${QT_LIBRARIES_REQUIRED})
target_include_urho3d(AbstractorizerUrho3D)