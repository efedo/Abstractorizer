cmake_minimum_required(VERSION 3.14)

#include(${UTILOGENY_DIR}/cmake/target_add_tbb.cmake)
include(${UTILOGENY_DIR}/cmake/target_add_qt.cmake)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

# incoming variables (that user should set)
set(MAGNUM_DIR "C:/Programming/lib/magnum" CACHE STRING "Magnum directory")
set(URHO3D_DIR "C:/Programming/lib/Urho3D" CACHE STRING "Urho3D directory")

# Setup Qt uic, moc, and rcc
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

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

set(QT_LIBRARIES_REQUIRED Core Gui Widgets 3DCore 3DExtras 3DRender 3DLogic 3DAnimation 3DInput Network Gamepad Concurrent PrintSupport WinExtras) #CANNOT FIND Qt5Charts
target_link_qt(AbstractorizerGUI)

#set(MAGNUM_LIBRARIES_REQUIRED)
#add_subdirectory(qt3d)
#add_subdirectory(magnum)

target_link_libraries(UtilogenyCore)
#target_link_libraries(AbstractorizerQt3D AbstractorizerMagnum)


