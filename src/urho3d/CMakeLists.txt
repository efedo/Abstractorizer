cmake_minimum_required(VERSION 3.14)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

link_directories(${CMAKE_LIBRARY_OUTPUT_DIRECTORY})

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

# Set CMake modules search path
set (CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/CMake/Modules)
message(STATUS "Urho module path: ${CMAKE_MODULE_PATH}")

set(URHO3D_HOME ${URHO3D_DIR}/out/build/x64-Release)

# Include UrhoCommon.cmake module after setting project name
include (UrhoCommon)

foreach(CurrTargetType ${AllTargetTypes})
	set(CurrTarget ${ProjectName}-${CurrTargetType})

    # Define target name
    set (${CurrTarget} MyExecutableName)

    # Define source files
    define_source_files ()

    # Setup target with resource copying
    setup_main_executable ()

endforeach()


set(SOURCES 
    "../generic/engine_window.h"
    "../generic/imgui_manager.h"
    "urho3d_window.h"
    "urho3d_window.cpp"
    "urho3d_widget.h" 
    "urho3d_widget.cpp"
 )

foreach(CurrTargetType ${AllTargetTypes})
    add_library(urho3d-${CurrTargetType} ${SOURCES})
endforeach()

foreach(CurrTargetType ${AllTargetTypes})
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