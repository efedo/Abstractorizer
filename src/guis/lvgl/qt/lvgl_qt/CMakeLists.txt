cmake_minimum_required(VERSION 3.10)

add_library(lvgl_qt
  LvglRenderer.cpp
  LvglRenderer.hpp
)

target_include_directories(lvgl_qt
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
)

target_link_libraries(lvgl_qt PUBLIC lvgl)


if(NOT TARGET Qt5::Widgets)
	message(WARNING "No Qt5::Widgets target here????")
endif()

target_link_qt(TARGET lvgl_qt COMPONENTS ${QT_LIBRARIES_REQUIRED})
