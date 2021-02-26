cmake_minimum_required(VERSION 3.14)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_library(AbstractorizerMagnum
	magnum_widget.cpp
    magnum_widget.h
	magnum_widget_internal.h
	magnum_widget_internal.cpp
)

target_include_qt(TARGET AbstractorizerMagnum COMPONENTS ${QT_LIBRARIES_REQUIRED})
target_include_magnum(TARGET AbstractorizerMagnum CORRADE_COMPONENTS "${CORRADE_LIBRARIES_REQUIRED}" MAGNUM_COMPONENTS "${MAGNUM_LIBRARIES_REQUIRED}")