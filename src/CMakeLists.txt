# (c) 2021 Eric Fedosejevs and contributors. Licensed under MIT license (see LICENSE for details)

cmake_minimum_required(VERSION 3.14)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(FIND_HINTS_COPPERSPICE "C:/CopperSpice/cmake/CopperSpice")
option(PRINT_INCLUDE_TARGET_DIRS "Debug info for package targets/includes" OFF)
option(INCLUDE_QT "Include Qt" ON)
option(INCLUDE_COPPERSPICE "Include CopperSpice" OFF)
option(INCLUDE_QT3D "Include Qt3D" ON)
option(INCLUDE_SDL "Include SDL" ON)
option(INCLUDE_MAGNUM "Include Magnum engine" OFF)
option(INCLUDE_WICKED "Include WickedEngine" OFF) # Currently disabled, WickedEngine does not compile
option(INCLUDE_OGRE "Include OGRE (classic)" ON)
option(INCLUDE_OGRE_NEXT "Include OGRE-next" ON)
option(INCLUDE_TINY_RENDERER "Include tinyrenderer" ON)
option(INCLUDE_LVGL "Include LVGL gui library" ON)

# Check option validity and setup qt/copperspice
if (INCLUDE_QT AND INCLUDE_COPPERSPICE)
	message(FATAL_ERROR "Cannot simultaneously include both Qt and Copperspice due to conflicting namespaces")
elseif(NOT INCLUDE_QT AND NOT INCLUDE_COPPERSPICE)
	message(FATAL_ERROR "Must include either Qt and Copperspice as window manager")
elseif(NOT INCLUDE_QT AND INCLUDE_QT3D)
	message(FATAL_ERROR "Cannot include Qt3D without also including Qt")
elseif(INCLUDE_QT)
	add_compile_definitions(USE_QT)
	set(QT_LIBRARIES_REQUIRED Core Gui Widgets Network Gamepad Concurrent PrintSupport WinExtras)
	include(${UTILOGENY_DIR}/cmake/target_add_qt.cmake)
elseif(INCLUDE_COPPERSPICE)
	add_compile_definitions(USE_COPPERSPICE)
	set(COPPERSPICE_LIBRARIES_REQUIRED CsCore CsGui)
	include(${UTILOGENY_DIR}/cmake/target_add_copperspice.cmake)
endif()

include(${UTILOGENY_DIR}/cmake/cppmake.cmake)
include(${UTILOGENY_DIR}/cmake/target_add_sdl.cmake)
include(${UTILOGENY_DIR}/cmake/target_add_magnum.cmake)
include(${UTILOGENY_DIR}/cmake/target_add_wicked.cmake)
include(${UTILOGENY_DIR}/cmake/target_add_lvgl.cmake)

add_executable(AbstractorizerGUI
	main.cpp
	mainwindow.h
	mainwindow.cpp
 "platforms.h")

set_target_properties(AbstractorizerGUI PROPERTIES
            CXX_STANDARD ${OVERRIDE_CXX_STANDARD}
            CXX_EXTENSIONS OFF
            )

#target_precompile_headers(AbstractorizerGUI PRIVATE qtprecomp.h)

macro(target_include_qt_or_cs target)
	if(INCLUDE_QT)
		include(${UTILOGENY_DIR}/cmake/target_add_qt.cmake)
		target_include_qt(TARGET ${target} COMPONENTS ${QT_LIBRARIES_REQUIRED})
	elseif(INCLUDE_COPPERSPICE)
		include(${UTILOGENY_DIR}/cmake/target_add_copperspice.cmake)
		target_include_copperspice(TARGET ${target} COMPONENTS ${COPPERSPICE_LIBRARIES_REQUIRED})
	else()
		message(FATAL_ERROR "Option validation should have prevented getting here")
	endif()
endmacro()

macro(target_link_qt_or_cs target)
	if(INCLUDE_QT)
		include(${UTILOGENY_DIR}/cmake/target_add_qt.cmake)
		target_link_qt(TARGET ${target} COMPONENTS ${QT_LIBRARIES_REQUIRED})
	elseif(INCLUDE_COPPERSPICE)
		include(${UTILOGENY_DIR}/cmake/target_add_copperspice.cmake)
		target_link_copperspice(TARGET ${target} COMPONENTS ${COPPERSPICE_LIBRARIES_REQUIRED})
	else()
		message(FATAL_ERROR "Option validation should have prevented getting here")
	endif()
endmacro()

target_config_compiler(AbstractorizerGUI)
target_link_sdl2(AbstractorizerGUI)

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
	# waiting on resolution/workaround for MSVC compiler bug # 1380599
	#add_subdirectory(guis/lvgl)
	#target_link_libraries(AbstractorizerGUI PUBLIC AbstractorizerLVGL)
endif()

if(NOT TARGET UtilogenyCore)
	add_subdirectory(${UTILOGENY_DIR} ${CMAKE_BINARY_DIR}/lib/Utilogeny)
endif()

target_link_qt_or_cs(AbstractorizerGUI)
target_link_libraries(AbstractorizerGUI PUBLIC AbstractorizerGeneric UtilogenyCore)

# Copy WickedEngine shaders to build directory
macro(deploy_resources target)	
	add_custom_command(TARGET "${target}" COMMAND ${CMAKE_COMMAND} -E copy_directory "${PROJECT_SOURCE_DIR}/resources" "${CMAKE_CURRENT_BINARY_DIR}/resources")
endmacro()

deploy_resources(AbstractorizerGUI)