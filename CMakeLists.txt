cmake_minimum_required(VERSION 3.14)
project(GraphicsUtils)

set(CMAKE_CXX_STANDARD 17)


add_subdirectory(ext)
add_subdirectory(include)
add_subdirectory(src)


add_library(OpenGLUtils STATIC ${OPENGLUTILS_SOURCES})
target_include_directories(OpenGLUtils
    PUBLIC
        include)

add_executable(GraphicsUtilsTest ${TEST_SOURCES})
target_include_directories(GraphicsUtilsTest
    PUBLIC
        include)