cmake_minimum_required(VERSION 3.14)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

option(PRINT_INCLUDE_TARGET_DIRS "Debug info for package targets/includes" OFF)
option(INCLUDE_QT3D "Include Qt3D" ON)
option(INCLUDE_SDL "Include SDL" ON)
option(INCLUDE_MAGNUM "Include Magnum engine" OFF)
option(INCLUDE_WICKED "Include WickedEngine" OFF) # Currently disabled, WickedEngine does not compile
option(INCLUDE_OGRE "Include OGRE (classic)" ON)
option(INCLUDE_OGRE_NEXT "Include OGRE-next" ON)
option(INCLUDE_TINY_RENDERER "Include tinyrenderer" ON)

option(INCLUDE_LVGL "Include LVGL gui library" ON)

include(${UTILOGENY_DIR}/cmake/cppmake.cmake)
include(${UTILOGENY_DIR}/cmake/target_add_qt.cmake)
include(${UTILOGENY_DIR}/cmake/target_add_sdl.cmake)
include(${UTILOGENY_DIR}/cmake/target_add_magnum.cmake)
include(${UTILOGENY_DIR}/cmake/target_add_wicked.cmake)
include(${UTILOGENY_DIR}/cmake/target_add_lvgl.cmake)

add_executable(AbstractorizerGUI
	main.cpp
	mainwindow.h
	mainwindow.cpp
)

#mainwindow.ui

set_target_properties(AbstractorizerGUI PROPERTIES
            CXX_STANDARD ${OVERRIDE_CXX_STANDARD}
            CXX_EXTENSIONS OFF
            )

#target_precompile_headers(AbstractorizerGUI PRIVATE qtprecomp.h)

target_config_compiler(AbstractorizerGUI)
target_link_sdl2(AbstractorizerGUI)

# Base Qt libraries required
set(QT_LIBRARIES_REQUIRED Core Gui Widgets Network Gamepad Concurrent PrintSupport WinExtras)

add_subdirectory(backends/generic)

if(INCLUDE_QT3D)
	add_compile_definitions(USE_QT3D)
	set(QT_LIBRARIES_REQUIRED 3DCore 3DExtras 3DRender 3DLogic 3DAnimation 3DInput ${QT_LIBRARIES_REQUIRED})
	add_subdirectory(backends/qt3d)
	# Qt link handled below
	target_link_libraries(AbstractorizerGUI PUBLIC AbstractorizerQt3D) #AbstractorizerMagnum
endif()

if(INCLUDE_SDL)
	add_compile_definitions(USE_SDL)
	add_subdirectory(backends/sdl)
	target_link_libraries(AbstractorizerGUI PUBLIC AbstractorizerSDL)
endif()

if(INCLUDE_MAGNUM)
	add_compile_definitions(USE_MAGNUM)
	set(MAGNUM_TARGET_GL ON)
	set(CORRADE_LIBRARIES_REQUIRED Containers Utility)	
	#https://en.wikipedia.org/wiki/EGL_(API)	
	set(MAGNUM_LIBRARIES_REQUIRED Magnum GL Sdl2Application Shaders MeshTools Primitives)
	add_subdirectory(backends/magnum)
	target_link_magnum(TARGET AbstractorizerGUI CORRADE_COMPONENTS "${CORRADE_LIBRARIES_REQUIRED}" MAGNUM_COMPONENTS "${MAGNUM_LIBRARIES_REQUIRED}")
	target_link_libraries(AbstractorizerGUI PUBLIC AbstractorizerMagnum)
endif()

if(INCLUDE_WICKED)
	add_compile_definitions(USE_WICKED)
	#message(STATUS "Including Wicked")
	add_subdirectory(backends/wicked)
	#target_link_wicked(AbstractorizerGUI)
	target_link_libraries(AbstractorizerGUI PUBLIC AbstractorizerWicked)
	target_copy_wicked_shaders(AbstractorizerGUI)
endif()

if(INCLUDE_OGRE)
	add_compile_definitions(USE_OGRE)
	add_subdirectory(backends/ogre)
	target_link_libraries(AbstractorizerGUI PUBLIC AbstractorizerOgre)
endif()

if(INCLUDE_OGRE_NEXT)
	add_compile_definitions(USE_OGRE_NEXT)
	add_subdirectory(backends/ogre-next)
	target_link_libraries(AbstractorizerGUI PUBLIC AbstractorizerOgreNext)
endif()

if(INCLUDE_TINY_RENDERER)
	add_compile_definitions(USE_TINY_RENDERER)
	add_subdirectory(backends/tinyrenderer)
	target_link_libraries(AbstractorizerGUI PUBLIC AbstractorizerTinyRenderer)
endif()

if(INCLUDE_LVGL)
	add_compile_definitions(USE_LVGL)
	add_subdirectory(guis/lvgl)
	#target_link_libraries(AbstractorizerGUI PUBLIC AbstractorizerLVGL)
endif()

target_link_qt(TARGET AbstractorizerGUI COMPONENTS ${QT_LIBRARIES_REQUIRED})
target_link_libraries(AbstractorizerGUI PUBLIC AbstractorizerGeneric) # UtilogenyCore

# Copy WickedEngine shaders to build directory
macro(deploy_resources target)	
	add_custom_command(TARGET "${target}" COMMAND ${CMAKE_COMMAND} -E copy_directory "${PROJECT_SOURCE_DIR}/resources" "${CMAKE_CURRENT_BINARY_DIR}/resources")
endmacro()

deploy_resources(AbstractorizerGUI)