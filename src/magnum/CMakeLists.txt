cmake_minimum_required(VERSION 3.14)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

link_directories(${CMAKE_LIBRARY_OUTPUT_DIRECTORY})

set(MAGNUM_INSTALL_DEBUG "${MAGNUM_DIR}/out/install/x64-Debug")
set(MAGNUM_INSTALL_RELEASE "${MAGNUM_DIR}/out/install/x64-Release")
set(CMAKE_MODULE_PATH "${MAGNUM_DIR}/corrade/modules" ${CMAKE_MODULE_PATH})
set(CMAKE_MODULE_PATH "${MAGNUM_DIR}/magnum/modules" ${CMAKE_MODULE_PATH})

foreach(CurrTargetType ${AllTargetTypes})
	set(CurrTarget ${ProjectName}-${CurrTargetType})
    if (${CurrTargetType} STREQUAL "release")
        set(CMAKE_PREFIX_PATH ${MAGNUM_INSTALL_RELEASE} ${CMAKE_PREFIX_PATH})
        include_directories( ${MAGNUM_INSTALL_RELEASE}/include )
        message(STATUS "Adding release include directory: '${MAGNUM_INSTALL_RELEASE}/include'")
    else()
        set(CMAKE_PREFIX_PATH ${MAGNUM_INSTALL_DEBUG} ${CMAKE_PREFIX_PATH})
        include_directories( ${MAGNUM_INSTALL_DEBUG}/include )
        message(STATUS "Adding debug include directory: '${MAGNUM_INSTALL_DEBUG}/include'")
    endif()
endforeach()

set(SOURCES 
    "magnum_widget.cpp"
     "magnum_widget.h" "magnum_widget_internal.h" "magnum_widget_internal.cpp")

find_package(Qt5Widgets REQUIRED)
find_package(Magnum REQUIRED GL Shaders)

if(MAGNUM_TARGET_GLES AND NOT MAGNUM_TARGET_DESKTOP_GLES)
    find_package(Magnum REQUIRED EglContext)
elseif(CORRADE_TARGET_WINDOWS)
    find_package(Magnum REQUIRED WglContext)
elseif(CORRADE_TARGET_APPLE)
    find_package(Magnum REQUIRED CglContext)
elseif(CORRADE_TARGET_UNIX)
    find_package(Magnum REQUIRED GlxContext)
else()
    message(FATAL_ERROR "Magnum context creation is not supported on this platform")
endif()

set_directory_properties(PROPERTIES CORRADE_USE_PEDANTIC_FLAGS ON)

foreach(CurrTargetType ${AllTargetTypes})

	set(CurrTarget ${ProjectName}-${CurrTargetType})

    add_library(magnum-${CurrTargetType} ${SOURCES})

    target_link_libraries(magnum-${CurrTargetType} PRIVATE
        Magnum::GLContext
        Magnum::GL
        Magnum::Magnum
        Magnum::Shaders
        Qt5::Widgets)

    message(STATUS "Copying: ")
    set(MAGNUM_DLLS_REQUIRED Magnum MagnumGL MagnumShaders CorradeUtility)
    foreach(CurrDLL ${MAGNUM_DLLS_REQUIRED})
        if (${CurrTargetType} STREQUAL "debug")
            set(CurrDLL ${CurrDLL}-d)
            set(SearchPath ${MAGNUM_INSTALL_DEBUG}/bin)
        else()
            set(SearchPath ${MAGNUM_INSTALL_RELEASE}/bin)
        endif()

        find_file(FOUND_DLL-${CurrDLL}
            NAMES ${CurrDLL}.dll ${CurrDLL}.so ${CurrDLL}.dylib
            PATHS ${SearchPath}
            NO_DEFAULT_PATH
        )

        if (NOT FOUND_DLL-${CurrDLL})
            message(FATAL_ERROR "Could not find ${CurrDLL} (dynamic library) in folder ${SearchPath}")
        endif()

        message(STATUS "  ${FOUND_DLL-${CurrDLL}} to: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")      
        add_custom_command(TARGET magnum-${CurrTargetType}
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${FOUND_DLL-${CurrDLL}} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
        )
   endforeach()

endforeach()

# Make the executable a default target to build & run in Visual Studio
set_property(DIRECTORY ${PROJECT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT MyApplication)
