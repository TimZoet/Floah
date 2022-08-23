set(NAME draft)
set(TYPE application)
set(INCLUDE_DIR "include/draft")
set(SRC_DIR "src")

set(HEADERS
    ${INCLUDE_DIR}/application.h
    ${INCLUDE_DIR}/data_sources.h
    ${INCLUDE_DIR}/generators.h
    ${INCLUDE_DIR}/window.h

    ${INCLUDE_DIR}/materials/display_material.h
    ${INCLUDE_DIR}/materials/text_material.h
    ${INCLUDE_DIR}/materials/widget_material.h
)

set(SOURCES
    ${SRC_DIR}/application.cpp
    ${SRC_DIR}/data_sources.cpp
    ${SRC_DIR}/generators.cpp
    ${SRC_DIR}/main.cpp
    ${SRC_DIR}/window.cpp

    ${SRC_DIR}/materials/display_material.cpp
    ${SRC_DIR}/materials/text_material.cpp
    ${SRC_DIR}/materials/widget_material.cpp
)

set(DEPS_PUBLIC
    floah-widget
)

make_target(
    TYPE ${TYPE}
    NAME ${NAME}
    EXAMPLE
    HEADERS "${HEADERS}"
    SOURCES "${SOURCES}"
    DEPS_PUBLIC "${DEPS_PUBLIC}"
)

target_compile_definitions(${NAME}
    PRIVATE
        RESOURCES_DIR="${PROJECT_SOURCE_DIR}/examples/draft/resources"
)
