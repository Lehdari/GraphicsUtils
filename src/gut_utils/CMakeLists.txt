file(GLOB SUB_SOURCES "*.cpp")

set(GUT_UTILS_SOURCES
    ${GUT_UTILS_SOURCES}
    ${SUB_SOURCES}
    PARENT_SCOPE)
