cmake_minimum_required(VERSION 3.14)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

if (NOT WIN32)
	find_package(Threads REQUIRED)
endif ()

add_subdirectory(Tests)

# add_library(AbstractorizerWicked
	# wicked_widget.cpp
    # wicked_widget.h
	#wicked_window.cpp 
	#wicked_window.h)

# target_include_qt(TARGET AbstractorizerWicked COMPONENTS ${QT_LIBRARIES_REQUIRED})
# target_include_wicked(AbstractorizerWicked)