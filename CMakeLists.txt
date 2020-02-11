cmake_minimum_required(VERSION 3.12)
project(GraphicsUtils)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# Set library output directories to binary directory
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})


# Find SDL 2
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/modules)

if(WIN32)
    set(SDL2_PATH $ENV{SDL2_DIR})
    if(NOT SDL2_PATH)
        message(FATAL_ERROR "Please set SDL2_DIR environment variable to point to SDL 2 installation directory.")
    endif()
endif()

find_package(SDL2 REQUIRED)


# Find OpenGL
set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)


# Find Eigen
if (UNIX)
    find_package(Eigen3 REQUIRED)
endif()

if(NOT EIGEN3_INCLUDE_DIR)
    set(EIGEN3_INCLUDE_DIR "$ENV{EIGEN3_INCLUDE_DIR}")
    if(NOT EIGEN3_INCLUDE_DIR)
        message(FATAL_ERROR "Please set EIGEN3_INCLUDE_DIR environment variable to point to Eigen 3 installation directory.")
    endif()
endif()


# Add absolute path to resource directory
add_definitions(-DRES_PATH="${CMAKE_CURRENT_LIST_DIR}/res/")


# Add subdirectories
add_subdirectory(include)
add_subdirectory(src)
add_subdirectory(ext)


# Configure Utils library
add_library(Utils STATIC ${UTILS_HEADERS} ${UTILS_SOURCES})

target_include_directories(Utils
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        ${EIGEN3_INCLUDE_DIR}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Utils
)


# Configure OpenGLUtils library
add_library(OpenGLUtils STATIC ${OPENGLUTILS_HEADERS} ${OPENGLUTILS_SOURCES})

target_include_directories(OpenGLUtils
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        ${SDL2_INCLUDE_DIR}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include/OpenGLUtils
        ${CMAKE_CURRENT_SOURCE_DIR}/ext/stb)

target_link_libraries(OpenGLUtils
    PUBLIC
        Utils
        imgui
        ${SDL2_LIBRARY}
)

# Cancel SDL stdio redirect when using windows
if(WIN32)
    target_link_options(OpenGLUtils
        PUBLIC
            -mconsole
    )
endif()


# Configure GraphicsUtilsTest executable
add_executable(GraphicsUtilsTest ${TEST_HEADERS} ${TEST_SOURCES})

target_include_directories(GraphicsUtilsTest
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        ${EIGEN3_INCLUDE_DIR})

target_link_libraries(GraphicsUtilsTest
    PUBLIC
        OpenGLUtils)
