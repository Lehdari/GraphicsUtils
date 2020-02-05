# glad OpenGL extension loader library
add_subdirectory(glad)


# imgui user interface library
add_library(
        imgui
    SHARED
        imgui/imgui.cpp
        imgui/imgui_draw.cpp
        imgui/imgui_widgets.cpp
        imgui/examples/imgui_impl_opengl3.cpp
        imgui/examples/imgui_impl_sdl.cpp
)

target_link_libraries(
    imgui
PUBLIC
    glad
PRIVATE
    ${OPENGL_LIBRARIES}
    ${SDL2_LIBRARY}
)

target_include_directories(
        imgui
    PUBLIC
        imgui
        imgui/examples
        ${SDL2_INCLUDE_DIR}
)

target_compile_definitions(
        imgui
    PUBLIC
        IMGUI_IMPL_OPENGL_LOADER_GLAD
)

if(WIN32)
    set_target_properties(
        imgui
    PROPERTIES
        WINDOWS_EXPORT_ALL_SYMBOLS ON
    )
endif()
