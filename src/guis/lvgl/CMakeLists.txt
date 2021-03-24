cmake_minimum_required(VERSION 3.14)

# Converted to CMake from https://github.com/lvgl/lv_sim_visual_studio
# (Built-in CMake build does not work well)
# Had to update many file paths/names
# Does not include all current source files, only those included in the original example
# (Should update this glob at some point!)

set(CMAKE_C_STANDARD 11)#C11
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Honestly have no clue what this does exactly
SET(CMAKE_C_ARCHIVE_CREATE "<CMAKE_AR> -qcD <TARGET> <LINK_FLAGS> <OBJECTS>")
SET(CMAKE_C_ARCHIVE_FINISH "<CMAKE_RANLIB> -D <TARGET>")

file(GLOB_RECURSE LVGL_SOURCES "${PROJECT_SOURCE_DIR}/lib/lvgl/src/*.c")
file(GLOB_RECURSE LVGL_DRIVER_SOURCES "${PROJECT_SOURCE_DIR}/lib/lv_drivers/src/*.c")
file(GLOB_RECURSE LVGL_EXAMPLE_SOURCES "${PROJECT_SOURCE_DIR}/lib/lv_examples/src/*.c")

write_file("examplesources.txt" "${LVGL_EXAMPLE_SOURCES}")

# some example sources do not current compile under MSVC, need to be selective

add_library(lvgl STATIC
	${LVGL_SOURCES}
	${LVGL_DRIVER_SOURCES}
	"${PROJECT_SOURCE_DIR}/lib/lv_examples/src/lv_demo_benchmark/lv_demo_benchmark.c"
	"${PROJECT_SOURCE_DIR}/lib/lv_examples/src/lv_demo_keypad_encoder/lv_demo_keypad_encoder.c"
	# "${PROJECT_SOURCE_DIR}/lib/lv_examples/src/lv_demo_music/assets/img_lv_demo_music_btn_list_pause.c"
	# "${PROJECT_SOURCE_DIR}/lib/lv_examples/src/lv_demo_music/assets/img_lv_demo_music_btn_list_play.c"
	# "${PROJECT_SOURCE_DIR}/lib/lv_examples/src/lv_demo_music/assets/img_lv_demo_music_btn_loop.c"
	# "${PROJECT_SOURCE_DIR}/lib/lv_examples/src/lv_demo_music/assets/img_lv_demo_music_btn_next.c"
	# "${PROJECT_SOURCE_DIR}/lib/lv_examples/src/lv_demo_music/assets/img_lv_demo_music_btn_pause.c"
	# "${PROJECT_SOURCE_DIR}/lib/lv_examples/src/lv_demo_music/assets/img_lv_demo_music_btn_play.c"
	# "${PROJECT_SOURCE_DIR}/lib/lv_examples/src/lv_demo_music/assets/img_lv_demo_music_btn_prev.c"
	# "${PROJECT_SOURCE_DIR}/lib/lv_examples/src/lv_demo_music/assets/img_lv_demo_music_btn_rnd.c"
	# "${PROJECT_SOURCE_DIR}/lib/lv_examples/src/lv_demo_music/assets/img_lv_demo_music_corner_left.c"
	# "${PROJECT_SOURCE_DIR}/lib/lv_examples/src/lv_demo_music/assets/img_lv_demo_music_corner_right.c"
	# "${PROJECT_SOURCE_DIR}/lib/lv_examples/src/lv_demo_music/assets/img_lv_demo_music_cover_1.c"
	# "${PROJECT_SOURCE_DIR}/lib/lv_examples/src/lv_demo_music/assets/img_lv_demo_music_cover_2.c"
	# "${PROJECT_SOURCE_DIR}/lib/lv_examples/src/lv_demo_music/assets/img_lv_demo_music_cover_3.c"
	# "${PROJECT_SOURCE_DIR}/lib/lv_examples/src/lv_demo_music/assets/img_lv_demo_music_icon_1.c"
	# "${PROJECT_SOURCE_DIR}/lib/lv_examples/src/lv_demo_music/assets/img_lv_demo_music_icon_2.c"
	# "${PROJECT_SOURCE_DIR}/lib/lv_examples/src/lv_demo_music/assets/img_lv_demo_music_icon_3.c"
	# "${PROJECT_SOURCE_DIR}/lib/lv_examples/src/lv_demo_music/assets/img_lv_demo_music_icon_4.c"
	# "${PROJECT_SOURCE_DIR}/lib/lv_examples/src/lv_demo_music/assets/img_lv_demo_music_list_border.c"
	# "${PROJECT_SOURCE_DIR}/lib/lv_examples/src/lv_demo_music/assets/img_lv_demo_music_logo.c"
	# "${PROJECT_SOURCE_DIR}/lib/lv_examples/src/lv_demo_music/assets/img_lv_demo_music_slider_knob.c"
	# "${PROJECT_SOURCE_DIR}/lib/lv_examples/src/lv_demo_music/assets/img_lv_demo_music_wave_bottom.c"
	# "${PROJECT_SOURCE_DIR}/lib/lv_examples/src/lv_demo_music/assets/img_lv_demo_music_wave_top.c"
	# "${PROJECT_SOURCE_DIR}/lib/lv_examples/src/lv_demo_music/assets/lv_demo_music_font_16_bold.c"
	# "${PROJECT_SOURCE_DIR}/lib/lv_examples/src/lv_demo_music/lv_demo_music.c"
	# "${PROJECT_SOURCE_DIR}/lib/lv_examples/src/lv_demo_music/lv_demo_music_list.c"
	# "${PROJECT_SOURCE_DIR}/lib/lv_examples/src/lv_demo_music/lv_demo_music_main.c"
	# "${PROJECT_SOURCE_DIR}/lib/lv_examples/src/lv_demo_printer/images/lv_demo_printer_icon_bright.c"
	# "${PROJECT_SOURCE_DIR}/lib/lv_examples/src/lv_demo_printer/images/lv_demo_printer_icon_eco.c"
	# "${PROJECT_SOURCE_DIR}/lib/lv_examples/src/lv_demo_printer/images/lv_demo_printer_icon_hue.c"
	# "${PROJECT_SOURCE_DIR}/lib/lv_examples/src/lv_demo_printer/images/lv_demo_printer_icon_pc.c"
	# "${PROJECT_SOURCE_DIR}/lib/lv_examples/src/lv_demo_printer/images/lv_demo_printer_icon_tel.c"
	# "${PROJECT_SOURCE_DIR}/lib/lv_examples/src/lv_demo_printer/images/lv_demo_printer_icon_wifi.c"
	# "${PROJECT_SOURCE_DIR}/lib/lv_examples/src/lv_demo_printer/images/lv_demo_printer_img_btn_bg_1.c"
	# "${PROJECT_SOURCE_DIR}/lib/lv_examples/src/lv_demo_printer/images/lv_demo_printer_img_btn_bg_2.c"
	# "${PROJECT_SOURCE_DIR}/lib/lv_examples/src/lv_demo_printer/images/lv_demo_printer_img_btn_bg_3.c"
	# "${PROJECT_SOURCE_DIR}/lib/lv_examples/src/lv_demo_printer/images/lv_demo_printer_img_btn_bg_4.c"
	# "${PROJECT_SOURCE_DIR}/lib/lv_examples/src/lv_demo_printer/images/lv_demo_printer_img_cloud.c"
	# "${PROJECT_SOURCE_DIR}/lib/lv_examples/src/lv_demo_printer/images/lv_demo_printer_img_copy.c"
	# "${PROJECT_SOURCE_DIR}/lib/lv_examples/src/lv_demo_printer/images/lv_demo_printer_img_internet.c"
	# "${PROJECT_SOURCE_DIR}/lib/lv_examples/src/lv_demo_printer/images/lv_demo_printer_img_mobile.c"
	# "${PROJECT_SOURCE_DIR}/lib/lv_examples/src/lv_demo_printer/images/lv_demo_printer_img_no_internet.c"
	# "${PROJECT_SOURCE_DIR}/lib/lv_examples/src/lv_demo_printer/images/lv_demo_printer_img_phone.c"
	# "${PROJECT_SOURCE_DIR}/lib/lv_examples/src/lv_demo_printer/images/lv_demo_printer_img_print.c"
	# "${PROJECT_SOURCE_DIR}/lib/lv_examples/src/lv_demo_printer/images/lv_demo_printer_img_printer2.c"
	# "${PROJECT_SOURCE_DIR}/lib/lv_examples/src/lv_demo_printer/images/lv_demo_printer_img_ready.c"
	# "${PROJECT_SOURCE_DIR}/lib/lv_examples/src/lv_demo_printer/images/lv_demo_printer_img_scan.c"
	# "${PROJECT_SOURCE_DIR}/lib/lv_examples/src/lv_demo_printer/images/lv_demo_printer_img_scan_example.c"
	# "${PROJECT_SOURCE_DIR}/lib/lv_examples/src/lv_demo_printer/images/lv_demo_printer_img_setup.c"
	# "${PROJECT_SOURCE_DIR}/lib/lv_examples/src/lv_demo_printer/images/lv_demo_printer_img_usb.c"
	# "${PROJECT_SOURCE_DIR}/lib/lv_examples/src/lv_demo_printer/images/lv_demo_printer_img_wave.c"
	# "${PROJECT_SOURCE_DIR}/lib/lv_examples/src/lv_demo_printer/lv_demo_printer.c"
	# "${PROJECT_SOURCE_DIR}/lib/lv_examples/src/lv_demo_printer/lv_demo_printer_theme.c"
	"${PROJECT_SOURCE_DIR}/lib/lv_examples/src/lv_demo_stress/assets/lv_font_montserrat_12_compr_az.c"
	"${PROJECT_SOURCE_DIR}/lib/lv_examples/src/lv_demo_stress/assets/lv_font_montserrat_16_compr_az.c"
	"${PROJECT_SOURCE_DIR}/lib/lv_examples/src/lv_demo_stress/assets/lv_font_montserrat_28_compr_az.c"
	"${PROJECT_SOURCE_DIR}/lib/lv_examples/src/lv_demo_stress/lv_demo_stress.c"
	"${PROJECT_SOURCE_DIR}/lib/lv_examples/src/lv_demo_widgets/assets/img_clothes.c"
	"${PROJECT_SOURCE_DIR}/lib/lv_examples/src/lv_demo_widgets/assets/img_demo_widgets_avatar.c"
	"${PROJECT_SOURCE_DIR}/lib/lv_examples/src/lv_demo_widgets/assets/img_lvgl_logo.c"
	"${PROJECT_SOURCE_DIR}/lib/lv_examples/src/lv_demo_widgets/lv_demo_widgets.c"
)

set_target_properties(lvgl PROPERTIES LINKER_LANGUAGE C)

add_subdirectory(qt)
# add_subdirectory(sdl)

# add_executable(AbstractorizerLVGL
	# # "simulator.c"
	# # "simulatorold.c"
	# # "LVGL.Simulator.manifest"
	# "LVGL.Simulator.rc"
	# "LVGL.ico"
	# #"Mile.Project.Properties.h"
	# "lv_conf.h"
	# "lv_drv_conf.h"
	# "lv_ex_conf.h"
	# "resource.h"
	# #main.c"
	# #mouse_cursor_icon.c"
	# # "simulator.h"
	# )

# Bypass hard-coded config directories
add_compile_definitions(LV_DEMO_CONF_PATH=${CMAKE_CURRENT_SOURCE_DIR}/lv_ex_conf.h)
#add_compile_definitions(LV_DRV_NO_CONF)

include_directories("${PROJECT_SOURCE_DIR}/lib/")
include_directories("${PROJECT_SOURCE_DIR}/lib/lvgl")

#target_link_libraries(AbstractorizerLVGL PUBLIC lvgl)

#set_target_properties(AbstractorizerLVGL PROPERTIES LINKER_LANGUAGE CXX)
	 
#target_include_qt(TARGET AbstractorizerLVGL COMPONENTS ${QT_LIBRARIES_REQUIRED})
#target_link_lvgl(AbstractorizerLVGL)
#target_link_sdl2(AbstractorizerLVGL)