cmake_minimum_required(VERSION 3.14)

include(${UTILOGENY_DIR}/cmake/cppmake.cmake)
include(${UTILOGENY_DIR}/cmake/target_add_qt.cmake)
include(${UTILOGENY_DIR}/cmake/target_add_sdl.cmake)
include(${UTILOGENY_DIR}/cmake/target_add_magnum.cmake)
include(${UTILOGENY_DIR}/cmake/target_add_urho3d.cmake)
include(${UTILOGENY_DIR}/cmake/target_add_wicked.cmake)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

option(INCLUDE_QT3D "Include Qt3D" ON)
if(INCLUDE_QT3D)
	add_compile_definitions(USE_QT3D)
endif()

option(INCLUDE_MAGNUM "Include Magnum engine" ON)
if(INCLUDE_MAGNUM)
	add_compile_definitions(USE_MAGNUM)
endif()

# The Urho3D CMake files are such a mess that I gave up
# Also it doesn't currently build from head without Android SDK
option(INCLUDE_URHO3D "Include Urho3D engine" OFF)
if(INCLUDE_URHO3D)
	add_compile_definitions(USE_URHO3D)
endif()

option(INCLUDE_WICKED "Include WickedEngine engine" ON)
if(INCLUDE_WICKED)
	add_compile_definitions(USE_WICKED)
endif()


# incoming variables (that user should set)

add_executable(AbstractorizerGUI
	main.cpp
	mainwindow.h
	mainwindow.cpp
	mainwindow.ui
)

set_target_properties(AbstractorizerGUI PROPERTIES
            CXX_STANDARD ${OVERRIDE_CXX_STANDARD}
            CXX_EXTENSIONS OFF
            )

#target_precompile_headers(AbstractorizerGUI PRIVATE qtprecomp.h)

target_config_compiler(AbstractorizerGUI)

target_link_sdl2(AbstractorizerGUI)

# Base Qt libraries required
set(QT_LIBRARIES_REQUIRED Core Gui Widgets Network Gamepad Concurrent PrintSupport WinExtras)

add_subdirectory(generic)

if(INCLUDE_QT3D)
	set(QT_LIBRARIES_REQUIRED 3DCore 3DExtras 3DRender 3DLogic 3DAnimation 3DInput ${QT_LIBRARIES_REQUIRED})
	add_subdirectory(qt3d)
	# Qt link handled below
	target_link_libraries(AbstractorizerGUI AbstractorizerQt3D) #AbstractorizerMagnum
endif()

if(INCLUDE_MAGNUM)
	set(MAGNUM_TARGET_GL ON)
	set(CORRADE_LIBRARIES_REQUIRED Containers Utility)	
	#https://en.wikipedia.org/wiki/EGL_(API)	
	set(MAGNUM_LIBRARIES_REQUIRED Magnum GL Sdl2Application Shaders)
	add_subdirectory(magnum)
	target_link_magnum(TARGET AbstractorizerGUI CORRADE_COMPONENTS "${CORRADE_LIBRARIES_REQUIRED}" MAGNUM_COMPONENTS "${MAGNUM_LIBRARIES_REQUIRED}")
	target_link_libraries(AbstractorizerGUI AbstractorizerMagnum)
endif()

if(INCLUDE_URHO3D)
	add_subdirectory(urho3d)
	target_link_urho3d(AbstractorizerGUI)
	target_link_libraries(AbstractorizerGUI AbstractorizerUrho3D)
endif()

if(INCLUDE_WICKED)
	add_subdirectory(wicked)
	target_link_wicked(AbstractorizerGUI)
	target_link_libraries(AbstractorizerGUI AbstractorizerWicked)
endif()

target_link_qt(TARGET AbstractorizerGUI COMPONENTS ${QT_LIBRARIES_REQUIRED})
target_link_libraries(AbstractorizerGUI AbstractorizerGeneric) # UtilogenyCore #AbstractorizerMagnum

