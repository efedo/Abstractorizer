cmake_minimum_required(VERSION 3.14)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

link_directories(${CMAKE_LIBRARY_OUTPUT_DIRECTORY})

set(SOURCES 
	main.cpp
	mainwindow.h
	mainwindow.cpp
	mainwindow.ui	
)

set(MAGNUM_LIBRARIES_REQUIRED)

foreach(CurrTargetType ${AllTargetTypes})
	set(CurrTarget ${ProjectName}-${CurrTargetType})
	add_executable(${CurrTarget} ${SOURCES})

	# Theoretically automoc should take care of this, but just in case....
	set_property(TARGET ${CurrTarget} PROPERTY AUTOMOC TRUE)
	set_property(TARGET ${CurrTarget} PROPERTY AUTOUIC TRUE)
	set_property(TARGET ${CurrTarget} PROPERTY AUTORCC TRUE)

	add_dependencies(${CurrTarget} qt3d-${CurrTargetType})
	target_link_libraries(${CurrTarget} 
	qt3d-${CurrTargetType}
	magnum-${CurrTargetType}
	Qt5::Core Qt5::Gui Qt5::Widgets Qt5::3DCore Qt5::3DExtras Qt5::3DRender Qt5::3DLogic Qt5::3DAnimation Qt5::3DInput)

	# Install required DLLs
	if (WIN32) # If Windows, might as well use WinDeployQt
		get_target_property(_qmake_executable Qt5::qmake IMPORTED_LOCATION)
		get_filename_component(_qt_bin_dir "${_qmake_executable}" DIRECTORY)
		find_program(WINDEPLOYQT_EXECUTABLE windeployqt HINTS "${_qt_bin_dir}")
		if (NOT WINDEPLOYQT_EXECUTABLE)
			message(FATAL_ERROR "WinDeployQt not found")
		endif()
		add_custom_command(TARGET ${CurrTarget} POST_BUILD
			COMMAND "${CMAKE_COMMAND}" -E
				env PATH="${_qt_bin_dir}" "${WINDEPLOYQT_EXECUTABLE}"
					--verbose 0
					--no-compiler-runtime
					\"$<TARGET_FILE:${CurrTarget}>\"
			COMMENT "Deploying Qt..."
		)
		set(CMAKE_INSTALL_UCRT_LIBRARIES TRUE) # installs UCRT libraries when installing system libraries (below)
		message(STATUS "Windows post-build installation successfully set up for target ${CurrTarget} via WinDeployQt.exe")
	else()
		foreach(QT_LIBRARIES_REQUIRED ${QT_LIBRARIES_REQUIRED})
			add_custom_command(TARGET ${CurrTarget} POST_BUILD
				COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Qt5::${QT_LIBRARIES_REQUIRED}> $<TARGET_FILE_DIR:${CurrTarget}>
			)
		endforeach()
		message(STATUS "Non-Windows post-build installation set up for target ${CurrTarget} (should work for most platforms)")
	endif()
endforeach()

add_subdirectory(qt3d)
add_subdirectory(magnum)
# add_subdirectory(urho3d)

# Install compiler runtime
include(InstallRequiredSystemLibraries) # Installs MSVC/other compiler runtime libraries



