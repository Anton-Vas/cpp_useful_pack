target_sources(
    ${CMAKE_PROJECT_NAME}
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/Logger.hpp
    ${CMAKE_CURRENT_LIST_DIR}/ProgBar.hpp
    ${CMAKE_CURRENT_LIST_DIR}/ProgSpin.hpp
)

target_include_directories(
    ${CMAKE_PROJECT_NAME}
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
)