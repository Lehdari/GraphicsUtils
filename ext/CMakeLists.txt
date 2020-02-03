if(WIN32)
    # only static linkage is supported for Windows for gl3w and imgui libraries
    add_library(
            gl3w
        STATIC
            imgui/examples/libs/gl3w/GL/gl3w.c
    )
else()
    add_library(
            gl3w
        SHARED
            imgui/examples/libs/gl3w/GL/gl3w.c
    )
endif()

target_include_directories(
        gl3w
    PUBLIC
        imgui/examples/libs/gl3w
)

target_link_libraries(
        gl3w
    PUBLIC
        ${CMAKE_DL_LIBS}
        ${OPENGL_LIBRARIES}
        ${SDL2_LIBRARY} # This contains some required OSX libs
)


if(WIN32)
    # only static linkage is supported for Windows for gl3w and imgui libraries
    add_library(
            imgui
        STATIC
            imgui/imgui.cpp
            imgui/imgui_draw.cpp
            imgui/imgui_widgets.cpp
            imgui/examples/imgui_impl_opengl3.cpp
            imgui/examples/imgui_impl_sdl.cpp
    )
else()
    add_library(
            imgui
        SHARED
            imgui/imgui.cpp
            imgui/imgui_draw.cpp
            imgui/imgui_widgets.cpp
            imgui/examples/imgui_impl_opengl3.cpp
            imgui/examples/imgui_impl_sdl.cpp
    )
endif()

target_include_directories(
        imgui
    PUBLIC
        imgui
        imgui/examples
        ${SDL2_INCLUDE_DIR}
)

target_link_libraries(
        imgui
    PUBLIC
        gl3w
    PRIVATE
        ${OPENGL_LIBRARIES}
        ${SDL2_LIBRARY}
)

# force gl3w extension loader
target_compile_definitions(
        imgui
    PRIVATE
        IMGUI_IMPL_OPENGL_LOADER_GL3W
)
