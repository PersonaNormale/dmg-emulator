cmake_minimum_required(VERSION 3.23)
project(gb LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)
FetchContent_Declare(Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG v3.12.0
)
FetchContent_MakeAvailable(Catch2)

list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
include(CTest)
include(Catch)

add_executable(gb_tests tests/gb/cpu/registers_test.cpp)
target_include_directories(gb_tests PRIVATE include)
target_link_libraries(gb_tests PRIVATE Catch2::Catch2WithMain)
catch_discover_tests(gb_tests)
