cmake_minimum_required(VERSION 3.15)

link_directories(${CMAKE_LIBRARY_OUTPUT_DIRECTORY})

set(SOURCES 
	main.cpp
	mainwindow.h
	mainwindow.cpp
	mainwindow.ui
)

add_executable(abstractorizer ${SOURCES})

add_dependencies(abstractorizer qt3d)

target_link_libraries(abstractorizer PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

add_subdirectory(qt3d)
