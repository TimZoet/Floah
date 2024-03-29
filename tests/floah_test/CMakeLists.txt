find_package(bettertest REQUIRED)

set(NAME floah_test)
set(TYPE test)
set(INCLUDE_DIR "include/floah_test")
set(SRC_DIR "src")

set(HEADERS
    ${INCLUDE_DIR}/elements/grid.h
    ${INCLUDE_DIR}/elements/horizontal_flow.h

    ${INCLUDE_DIR}/input/sorting.h
)
 
set(SOURCES
    ${SRC_DIR}/main.cpp

    ${SRC_DIR}/elements/grid.cpp
    ${SRC_DIR}/elements/horizontal_flow.cpp

    ${SRC_DIR}/input/sorting.cpp
)

set(DEPS_PRIVATE
    bettertest::bettertest
    floah-layout
    floah-put
)

make_target(
    TYPE ${TYPE}
    NAME ${NAME}
    OUTDIR "tests"
    HEADERS "${HEADERS}"
    SOURCES "${SOURCES}"
    DEPS_PRIVATE "${DEPS_PRIVATE}"
)