# add GLAD OpenGL extension loader library

add_library(glad SHARED src/glad.c)

target_include_directories(glad
	PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

# link relevant OpenGL libraries
target_link_libraries(
        glad
    PUBLIC
        ${CMAKE_DL_LIBS}
        ${OPENGL_LIBRARIES}
        ${SDL2_LIBRARY} # This contains some required OSX libs
)

target_compile_definitions(glad
    PUBLIC
        GLAD_GLAPI_EXPORT
    PRIVATE
        GLAD_GLAPI_EXPORT_BUILD
)