add_subdirectory(gut_image)
add_subdirectory(gut_opengl)
add_subdirectory(gut_utils)
add_subdirectory(gut_test)

set(GUT_IMAGE_HEADERS ${GUT_IMAGE_HEADERS} PARENT_SCOPE)
set(GUT_OPENGL_HEADERS ${GUT_OPENGL_HEADERS} PARENT_SCOPE)
set(GUT_UTILS_HEADERS ${GUT_UTILS_HEADERS} PARENT_SCOPE)
set(GUT_TEST_HEADERS ${GUT_TEST_HEADERS} PARENT_SCOPE)
