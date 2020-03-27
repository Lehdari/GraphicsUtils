cmake_minimum_required(VERSION 3.12)
project(GraphicsUtils)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# Build options
option(GUT_BUILD_SHARED_LIBRARIES
    "Build shared libraries instead of static ones."
    ON
)


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
else()
    set(EIGEN3_INCLUDE_DIR "$ENV{EIGEN3_INCLUDE_DIR}")
endif()

if(NOT EIGEN3_INCLUDE_DIR)
    message(FATAL_ERROR "Please set EIGEN3_INCLUDE_DIR environment variable to point to Eigen 3 installation directory.")
endif()


# Add absolute path to resource directory
add_definitions(-DRES_PATH="${CMAKE_CURRENT_LIST_DIR}/res/")


# Add subdirectories
add_subdirectory(include)
add_subdirectory(src)
add_subdirectory(ext)


# Configure gut_utils library
if (${GUT_BUILD_SHARED_LIBRARIES})
    add_library(gut_utils SHARED ${GUT_UTILS_HEADERS} ${GUT_UTILS_SOURCES})

    # Export symbols on Windows shared library
    set_target_properties(gut_utils PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS ON)
else()
    add_library(gut_utils STATIC ${GUT_UTILS_HEADERS} ${GUT_UTILS_SOURCES})
endif()

target_include_directories(gut_utils
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        ${EIGEN3_INCLUDE_DIR}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include/gut_utils
)


# Configure gut_opengl library
if (${GUT_BUILD_SHARED_LIBRARIES})
    add_library(gut_opengl SHARED ${GUT_OPENGL_HEADERS} ${GUT_OPENGL_SOURCES})

    # Export symbols on Windows shared library
    set_target_properties(gut_opengl PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS ON)
else()
    add_library(gut_opengl STATIC ${GUT_OPENGL_HEADERS} ${GUT_OPENGL_SOURCES})
endif()

target_include_directories(gut_opengl
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        ${SDL2_INCLUDE_DIR}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include/gut_opengl
        ${CMAKE_CURRENT_SOURCE_DIR}/ext/stb
)

target_link_libraries(gut_opengl
    PUBLIC
        gut_utils
        imgui
        ${SDL2_LIBRARY}
)

# Cancel SDL stdio redirect when using windows
if(WIN32)
    target_link_options(gut_opengl
        PUBLIC
            -mconsole
    )
endif()


# Configure gut_test executable
add_executable(gut_test ${GUT_TEST_HEADERS} ${GUT_TEST_SOURCES})

target_include_directories(gut_test
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        ${EIGEN3_INCLUDE_DIR}
)

target_link_libraries(gut_test
    PUBLIC
        gut_opengl
)
