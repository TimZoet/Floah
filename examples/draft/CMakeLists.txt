set(NAME draft)
set(TYPE application)
set(INCLUDE_DIR "include/draft")
set(SRC_DIR "src")

set(HEADERS
    ${INCLUDE_DIR}/application.h
    ${INCLUDE_DIR}/generators.h
    ${INCLUDE_DIR}/window.h

    ${INCLUDE_DIR}/data_sources/bool_data_source.h
    ${INCLUDE_DIR}/data_sources/index_data_source.h
    ${INCLUDE_DIR}/data_sources/string_list_data_source.h

    ${INCLUDE_DIR}/materials/display_material.h
    ${INCLUDE_DIR}/materials/display_material_instance.h
    ${INCLUDE_DIR}/materials/panel_transform_node.h
    ${INCLUDE_DIR}/materials/text_material.h
    ${INCLUDE_DIR}/materials/text_material_instance.h
    ${INCLUDE_DIR}/materials/widget_material.h
    ${INCLUDE_DIR}/materials/widget_material_instance.h
    ${INCLUDE_DIR}/materials/widget_transform_node.h

    ${INCLUDE_DIR}/models/global_model.h
    ${INCLUDE_DIR}/models/indexed_string_list_model.h

    ${INCLUDE_DIR}/panels/simple_panel.h
)

set(SOURCES
    ${SRC_DIR}/application.cpp
    ${SRC_DIR}/generators.cpp
    ${SRC_DIR}/main.cpp
    ${SRC_DIR}/window.cpp

    ${SRC_DIR}/data_sources/bool_data_source.cpp
    ${SRC_DIR}/data_sources/index_data_source.cpp
    ${SRC_DIR}/data_sources/string_list_data_source.cpp

    ${SRC_DIR}/materials/display_material.cpp
    ${SRC_DIR}/materials/display_material_instance.cpp
    ${SRC_DIR}/materials/panel_transform_node.cpp
    ${SRC_DIR}/materials/text_material.cpp
    ${SRC_DIR}/materials/text_material_instance.cpp
    ${SRC_DIR}/materials/widget_material.cpp
    ${SRC_DIR}/materials/widget_material_instance.cpp
    ${SRC_DIR}/materials/widget_transform_node.cpp

    ${SRC_DIR}/models/global_model.cpp
    ${SRC_DIR}/models/indexed_string_list_model.cpp

    ${SRC_DIR}/panels/simple_panel.cpp
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
