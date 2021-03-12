cmake_minimum_required(VERSION 3.14)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_library(AbstractorizerWicked
	wicked_widget.cpp
    wicked_widget.h
	wicked_widget_internal.h
	wicked_widget_internal.cpp
)

target_include_qt(TARGET AbstractorizerWicked COMPONENTS ${QT_LIBRARIES_REQUIRED})
target_include_wicked(AbstractorizerWicked)