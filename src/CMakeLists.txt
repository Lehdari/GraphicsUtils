add_subdirectory(gut_image)
add_subdirectory(gut_opengl)
add_subdirectory(gut_utils)
add_subdirectory(gut_test)

set(GUT_IMAGE_SOURCES ${GUT_IMAGE_SOURCES} PARENT_SCOPE)
set(GUT_OPENGL_SOURCES ${GUT_OPENGL_SOURCES} PARENT_SCOPE)
set(GUT_UTILS_SOURCES ${GUT_UTILS_SOURCES} PARENT_SCOPE)
set(GUT_TEST_SOURCES ${GUT_TEST_SOURCES} PARENT_SCOPE)
