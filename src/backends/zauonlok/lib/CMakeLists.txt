cmake_minimum_required(VERSION 3.10)

set(HEADERS
    renderer/core/api.h
    renderer/core/camera.h
    renderer/core/darray.h
    renderer/core/draw2d.h
    renderer/core/graphics.h
    renderer/core/image.h
    renderer/core/macro.h
    renderer/core/maths.h
    renderer/core/mesh.h
    renderer/core/platform.h
    renderer/core/private.h
    renderer/core/scene.h
    renderer/core/skeleton.h
    renderer/core/texture.h
    renderer/scenes/blinn_scenes.h
    renderer/scenes/pbr_scenes.h
    renderer/scenes/scene_helper.h
    renderer/shaders/blinn_shader.h
    renderer/shaders/cache_helper.h
    renderer/shaders/pbr_shader.h
    renderer/shaders/skybox_shader.h
    renderer/tests/test_blinn.h
    renderer/tests/test_helper.h
    renderer/tests/test_pbr.h
)
set(SOURCES
    renderer/core/camera.c
    renderer/core/darray.c
    renderer/core/draw2d.c
    renderer/core/graphics.c
    renderer/core/image.c
    renderer/core/maths.c
    renderer/core/mesh.c
    renderer/core/private.c
    renderer/core/scene.c
    renderer/core/skeleton.c
    renderer/core/texture.c
    renderer/scenes/blinn_scenes.c
    renderer/scenes/pbr_scenes.c
    renderer/scenes/scene_helper.c
    renderer/shaders/blinn_shader.c
    renderer/shaders/cache_helper.c
    renderer/shaders/pbr_shader.c
    renderer/shaders/skybox_shader.c
    renderer/tests/test_blinn.c
    renderer/tests/test_helper.c
    renderer/tests/test_pbr.c
    renderer/main.c
)

if(WIN32)
    set(SOURCES ${SOURCES} renderer/platforms/win32.c)
elseif(APPLE)
    set(SOURCES ${SOURCES} renderer/platforms/macos.m)
else()
    set(SOURCES ${SOURCES} renderer/platforms/linux.c)
endif()

# ==============================================================================
# Target definition
# ==============================================================================

set(TARGET ZauonlokViewer)

add_library(${TARGET} ${HEADERS} ${SOURCES})

# ==============================================================================
# Target properties
# ==============================================================================

set_target_properties(${TARGET} PROPERTIES C_STANDARD 90)
set_target_properties(${TARGET} PROPERTIES C_EXTENSIONS OFF)
set_target_properties(${TARGET} PROPERTIES INTERPROCEDURAL_OPTIMIZATION ON)

# ==============================================================================
# Compile options
# ==============================================================================

if(MSVC)
    target_compile_options(${TARGET} PRIVATE /W4 /D_CRT_SECURE_NO_WARNINGS)
    target_compile_options(${TARGET} PRIVATE /fp:fast)
else()
    target_compile_options(${TARGET} PRIVATE -Wall -Wextra -pedantic)
    target_compile_options(${TARGET} PRIVATE -ffast-math)
endif()

if(UNIX AND NOT APPLE)
    target_compile_options(${TARGET} PRIVATE -D_POSIX_C_SOURCE=200809L)
endif()

# ==============================================================================
# Link libraries
# ==============================================================================

if(WIN32)
    # nothing to do for now
elseif(APPLE)
    target_link_libraries(${TARGET} PRIVATE "-framework Cocoa")
else()
    target_link_libraries(${TARGET} PRIVATE m X11)
endif()

# ==============================================================================
# Asset files
# ==============================================================================

set(ASSETS_SRC "${CMAKE_CURRENT_SOURCE_DIR}/assets")
set(ASSETS_BUILD_DST "${CMAKE_BINARY_DIR}/src/assets")
set(ASSETS_INSTALL_DST "${CMAKE_INSTALL_PREFIX}/bin/assets")
set(ASSETS_CMD copy_directory)
set(ASSETS_MSG "Copy assets")

add_custom_command(
    TARGET ${TARGET} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E ${ASSETS_CMD}
    "${ASSETS_SRC}" "${ASSETS_BUILD_DST}"
    COMMENT "${ASSETS_MSG}: ${ASSETS_SRC} to ${ASSETS_BUILD_DST}"
)

install(CODE "
	message(\"${ASSETS_MSG}: ${ASSETS_SRC} to ${ASSETS_INSTALL_DST}\")		
	execute_process(
    COMMAND ${CMAKE_COMMAND} -E ${ASSETS_CMD}
    \"${ASSETS_SRC}\" \"${ASSETS_INSTALL_DST}\"
	)
")

# ==============================================================================
# IDE support
# ==============================================================================

#set_directory_properties(PROPERTIES VS_STARTUP_PROJECT ${TARGET})
#source_group(TREE "${CMAKE_SOURCE_DIR}/lib/zauonlok_renderer" FILES ${HEADERS} ${SOURCES})
