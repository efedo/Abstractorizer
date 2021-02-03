cmake_minimum_required(VERSION 3.14)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

link_directories(${CMAKE_LIBRARY_OUTPUT_DIRECTORY})

set(URHO3D_INSTALL_DEBUG "${URHO3D_DIR}/out/install/x64-Debug")
set(URHO3D_INSTALL_RELEASE "${URHO3D_DIR}/out/install/x64-Release")
set(CMAKE_MODULE_PATH "${URHO3D_DIR}/CMake/Modules" ${CMAKE_MODULE_PATH})

foreach(CurrTargetType ${AllTargetTypes})
	set(CurrTarget ${ProjectName}-${CurrTargetType})
    if (${CurrTargetType} STREQUAL "release")
        set(CMAKE_PREFIX_PATH ${URHO3D_INSTALL_RELEASE} ${CMAKE_PREFIX_PATH})
        include_directories( ${URHO3D_INSTALL_RELEASE}/include )
        message(STATUS "Adding release include directory: '${URHO3D_INSTALL_RELEASE}/include'")
        set(URHO3D_HOME ${URHO3D_INSTALL_RELEASE})
    else()
        set(CMAKE_PREFIX_PATH ${URHO3D_INSTALL_DEBUG} ${CMAKE_PREFIX_PATH})
        include_directories( ${URHO3D_INSTALL_DEBUG}/include )
        message(STATUS "Adding debug include directory: '${URHO3D_INSTALL_DEBUG}/include'")
        set(URHO3D_HOME ${URHO3D_INSTALL_DEBUG})
    endif()
endforeach()

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

# Include UrhoCommon.cmake module after setting project name
include (UrhoCommon)

set(SOURCES 
    "urho3d_widget_internal.h"
    "urho3d_widget_internal.cpp"
    "urho3d_widget.h" 
    "urho3d_widget.cpp"
 )

foreach(CurrTargetType ${AllTargetTypes})
    add_library(urho3d-${CurrTargetType} ${SOURCES})
	set(CurrTarget ${ProjectName}-${CurrTargetType})
	add_dependencies(${CurrTarget} urho3d-${CurrTargetType})
	target_link_libraries(${CurrTarget} urho3d-${CurrTargetType})
    if (${CurrTargetType} STREQUAL "release")

        # Find Urho3D (release version)
        find_library(FOUND_URHO3D_LIB_RELEASE
            NAMES "Urho3D.lib" "Urho3D.a"
            PATHS ${URHO3D_DIR}/out/build/x64-Release/lib
            NO_DEFAULT_PATH
        )
        if (NOT FOUND_URHO3D_LIB_RELEASE)
            message(FATAL_ERROR "Could not find Urho3D release library (Urho3D.xxx) in folder ${URHO_LIBRARY_DIR_RELEASE}")
        endif()
        include_directories(${URHO3D_DIR}/out/build/x64-Release/include)
        target_link_libraries(${CurrTarget} ${FOUND_URHO3D_LIB_RELEASE})
        message(STATUS "Found Urho3D library: ${FOUND_URHO3D_LIB_RELEASE}")

        # Find SDL2
        find_library(FOUND_SDL2_LIB_RELEASE
            NAMES "SDL2.lib" "SDL2.a"
            PATHS ${SDL_DIR}/out/build/x64-Release
            NO_DEFAULT_PATH
        )
        if (NOT FOUND_SDL2_LIB_RELEASE)
            message(FATAL_ERROR "Could not find SDL2 release library (SDL2.xxx) in folder ${FOUND_SDL2_LIB_RELEASE}")
        endif()
        #include_directories(${SDL_DIR}/include)
        include_directories(${URHO3D_DIR}/out/build/x64-Release/include/Urho3D/ThirdParty)
        include_directories(${URHO3D_DIR}/out/build/x64-Release/include/Urho3D)
        include_directories(${URHO3D_DIR}/out/build/x64-Release/include)
        target_link_libraries(${CurrTarget} ${FOUND_SDL2_LIB_RELEASE})
        message(STATUS "Found SDL2 library: ${FOUND_SDL2_LIB_RELEASE}")

    else()

        # Find Urho3D (debug version)
        find_library(FOUND_URHO3D_LIB_DEBUG
            NAMES "Urho3D_d.lib" "Urho3D_d.a"
            PATHS ${URHO3D_DIR}/out/build/x64-Debug/lib
            NO_DEFAULT_PATH
        )        
        if (NOT FOUND_URHO3D_LIB_DEBUG)
            message(FATAL_ERROR "Could not find Urho3D debug library (Urho3D_d.xxx) in folder ${URHO_LIBRARY_DIR_DEBUG}")
        endif()
        include_directories(${URHO3D_DIR}/out/build/x64-Debug/include)
        target_link_libraries(${CurrTarget} ${FOUND_URHO3D_LIB_DEBUG})
        message(STATUS "Found Urho3D library: ${FOUND_URHO3D_LIB_DEBUG}")

        # Find SDL2
        find_library(FOUND_SDL2_LIB_DEBUG
            NAMES "SDL2d.lib" "SDL2d_d.a"
            PATHS ${SDL_DIR}/out/build/x64-Debug
            NO_DEFAULT_PATH
        )
        if (NOT FOUND_SDL2_LIB_DEBUG)
            message(FATAL_ERROR "Could not find SDL2 debug library (SDL2d.xxx) in folder ${FOUND_SDL2_LIB_DEBUG}")
        endif()
        #include_directories(${SDL_DIR}/include)
        include_directories(${URHO3D_DIR}/out/build/x64-Debug/include/Urho3D/ThirdParty)
        include_directories(${URHO3D_DIR}/out/build/x64-Debug/include/Urho3D)
        include_directories(${URHO3D_DIR}/out/build/x64-Debug/include)
        target_link_libraries(${CurrTarget} ${FOUND_SDL2_LIB_DEBUG})
        message(STATUS "Found SDL2 library: ${FOUND_SDL2_LIB_DEBUG}")

    endif()
endforeach()

print_includes()