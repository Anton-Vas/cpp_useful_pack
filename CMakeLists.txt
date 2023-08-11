cmake_minimum_required(VERSION 3.0.0)
project(cpp_useful_pack VERSION 0.1.0 LANGUAGES C CXX)

add_executable(cpp_useful_pack main.cpp)

set(ROOT /home/dev/Desktop/cpp_useful_pack)
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

# ~~~~~~ CPack ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|
include(CPack)

# ~~~~~~ CTest ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|
# include(CTest)
# enable_testing()

# ~~~~~~ C++  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|
set(CMAKE_CXX_STANDARD_REQUIRED 17)                         # set to C++17

# ~~~~~~ Folders ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|
add_subdirectory(src)
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|


