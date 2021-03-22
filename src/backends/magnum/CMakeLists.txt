cmake_minimum_required(VERSION 3.14)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(AbstractorizerMagnum_SOURCES 
	magnum_widget.cpp
    magnum_widget.h
	magnum_widget_internal.h
	magnum_widget_internal.cpp
)

if(INCLUDE_SDL)
	set(AbstractorizerMagnum_SOURCES ${AbstractorizerMagnum_SOURCES}
		
		
	)
endif()

add_library(AbstractorizerMagnum
	${AbstractorizerMagnum_SOURCES}
)

if(INCLUDE_SDL)
	target_link_sdl2(AbstractorizerMagnum)
endif()

target_include_qt(TARGET AbstractorizerMagnum COMPONENTS ${QT_LIBRARIES_REQUIRED})
target_include_magnum(TARGET AbstractorizerMagnum CORRADE_COMPONENTS "${CORRADE_LIBRARIES_REQUIRED}" MAGNUM_COMPONENTS "${MAGNUM_LIBRARIES_REQUIRED}")