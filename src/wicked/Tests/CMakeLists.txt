# if (NOT WIN32)
	# find_package(Threads REQUIRED)
# endif ()

set (SOURCE_FILES
	stdafx.cpp
	Tests.cpp
	Tests.h
	stdafx.h
	wicked_window.cpp 
	wicked_window.h
)

if (WIN32)
	list (APPEND SOURCE_FILES
		main_Windows.cpp
		main_Windows.h
		Tests.rc
	)

	add_library(AbstractorizerWicked ${SOURCE_FILES})
#	add_library(WickedTests WIN32 ${SOURCE_FILES})

	target_link_libraries(AbstractorizerWicked PUBLIC 
		WickedEngine_Windows
	)
else()
	list (APPEND SOURCE_FILES
		main_SDL2.cpp
	)

	add_library(AbstractorizerWicked ${SOURCE_FILES})

	target_link_libraries(AbstractorizerWicked PUBLIC 
		WickedEngine
		Threads::Threads
	)
endif ()

if (MSVC)
	set_property(TARGET AbstractorizerWicked PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
endif ()
