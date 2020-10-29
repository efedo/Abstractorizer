cmake_minimum_required(VERSION 3.14)

# incoming variables (that user should set)
set(QT_PREFIX "C:/Qt/5.15.1/msvc2019_64")
set(SDL_DIR "C:/Programming/lib/SDL2-2.0.12")
set(MAGNUM_DIR "C:/Programming/lib/magnum")
#set(URHO3D_DIR "C:/Programming/lib/Urho3D")
# Urho3D is just too painful to use, forget it

# set the project name and version
set(ProjectName Abstractorizer)
project(${ProjectName} VERSION 1.0)
set(DebugSuffix "debug")
set(ReleaseSuffix "release")

if (${CMAKE_BUILD_TYPE} STREQUAL "release" OR ${CMAKE_BUILD_TYPE} STREQUAL "RelWithDebInfo")
    set(AllTargetTypes ${ReleaseSuffix})
elseif(${CMAKE_BUILD_TYPE} STREQUAL "debug")
    set(AllTargetTypes ${DebugSuffix})
else()
    set(AllTargetTypes ${DebugSuffix} ${ReleaseSuffix})
endif()

message(STATUS "Configuring ${ProjectName} (version ${CMAKE_PROJECT_VERSION}) with targets:")
foreach(AllTargetTypes ${AllTargetTypes})
    message(STATUS "  ${AllTargetTypes}")
endforeach()

set(CMAKE_INCLUDE_CURRENT_DIR ON)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHc /std:c++17") # MSVC flags


# setup Qt uic, moc, and rcc
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# Qt prefix
set(CMAKE_PREFIX_PATH ${QT_PREFIX})

#Libraries required. Probably the same as above minus the '::'. find_package() will be called on these
set(QT_LIBRARIES_REQUIRED Core Gui Widgets 3DCore 3DExtras 3DRender 3DLogic 3DAnimation 3DInput Network Gamepad Concurrent) #CANNOT FIND Qt5Charts

## Note: there is a bug with find_package for Qt5.15. The below does not always correctly retrieve requested components, and fails silently
## Manually loading individual packages as well below to get around this
find_package(QT5 REQUIRED COMPONENTS $QT_LIBRARIES_REQUIRED)

if( QT5_FOUND )
    message(STATUS "Found QT5 version: ${QT5_VERSION_MAJOR}.${QT5_VERSION_MINOR}")
else()
    message(FATAL_ERROR "QT5 not found")
endif()

# Find the individual Qt libraries (in case find package above failed for one or more components)
foreach(QT_LIBRARIES_REQUIRED ${QT_LIBRARIES_REQUIRED})
    find_package( Qt5${QT_LIBRARIES_REQUIRED} REQUIRED )
    include_directories( ${Qt5${QT_LIBRARIES_REQUIRED}_INCLUDE_DIRS} )
    if ( Qt5${QT_LIBRARIES_REQUIRED}_FOUND )
            message(STATUS "  Qt5${QT_LIBRARIES_REQUIRED} found")
    else()
        message(FATAL_ERROR "  Qt5${QT_LIBRARIES_REQUIRED} not found")
    endif()
endforeach()

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})
include(${CMAKE_SOURCE_DIR}/cmake/PrintIncludes.cmake)
print_includes()

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

# configure a header file to pass the version number to the source code
configure_file(src/cmakeversion.h.in src/cmakeversion.h)

add_subdirectory(src)