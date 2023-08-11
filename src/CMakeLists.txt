target_sources(
    ${CMAKE_PROJECT_NAME}
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/Logger.hpp
    ${CMAKE_CURRENT_LIST_DIR}/ProgBar_Simple.hpp
    ${CMAKE_CURRENT_LIST_DIR}/ProgBar_Fancy.hpp
)

target_include_directories(
    ${CMAKE_PROJECT_NAME}
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
)