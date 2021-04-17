cmake_minimum_required(VERSION 3.14)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_library(AbstractorizerWicked
	stdafx.cpp
	Tests.cpp
	Tests.h
	stdafx.h
	wicked_widget.h
	wicked_widget_internal.cpp
    wicked_widget_internal.h
	#wicked_window.cpp 
	#wicked_window.h
	#main_SDL2.cpp
	#main_SDL2.h
	#main_Windows.cpp
	#main_Windows.h
)

# if (WIN32)
	# target_link_libraries(AbstractorizerWicked PUBLIC 
		# WickedEngine_Windows
	# )
# else()
	# find_package(Threads REQUIRED)
	# target_link_libraries(AbstractorizerWicked PUBLIC 
		# WickedEngine
		# Threads::Threads
	# )
# endif ()

target_link_wicked(AbstractorizerWicked)
target_include_qt_or_cs(AbstractorizerWicked)
target_link_sdl2(AbstractorizerWicked)