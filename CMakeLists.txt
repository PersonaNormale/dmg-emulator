cmake_minimum_required(VERSION 3.23)

project(gb LANGUAGES CXX)

# GCC-only
if(NOT CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  message(FATAL_ERROR
    "This project currently supports GCC only (CMAKE_CXX_COMPILER_ID=${CMAKE_CXX_COMPILER_ID})."
  )
endif()

option(GB_ENABLE_COVERAGE "Enable coverage instrumentation" OFF)

# Core library 
add_library(gb_core
  src/gb/cpu/registers.cpp
)
target_compile_features(gb_core PUBLIC cxx_std_20)
target_include_directories(gb_core
  PUBLIC
    "${PROJECT_SOURCE_DIR}/include"
)

# Testing
include(CTest)

if(BUILD_TESTING)
  include(FetchContent)

  FetchContent_Declare(Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.12.0
  )
  FetchContent_MakeAvailable(Catch2)

  list(APPEND CMAKE_MODULE_PATH "${Catch2_SOURCE_DIR}/extras")
  include(Catch)

  add_executable(gb_tests
    tests/gb/cpu/registers_test.cpp
    tests/gb/cpu/operand_types_test.cpp
  )
  target_compile_features(gb_tests PRIVATE cxx_std_20)
  target_link_libraries(gb_tests
    PRIVATE
      gb_core
      Catch2::Catch2WithMain
  )

  catch_discover_tests(gb_tests)
endif()

# Coverage
if(GB_ENABLE_COVERAGE)
  if(NOT BUILD_TESTING)
    message(FATAL_ERROR "GB_ENABLE_COVERAGE requires BUILD_TESTING=ON.")
  endif()
  if(NOT TARGET gb_tests)
    message(FATAL_ERROR "GB_ENABLE_COVERAGE requires the 'gb_tests' target.")
  endif()

  foreach(tgt IN ITEMS gb_core gb_tests)
    target_compile_options(${tgt} PRIVATE -O0 -g --coverage)
    target_link_options(${tgt} PRIVATE --coverage)
  endforeach()

  find_program(LCOV_EXECUTABLE lcov)
  find_program(GENHTML_EXECUTABLE genhtml)

  if(LCOV_EXECUTABLE AND GENHTML_EXECUTABLE)
    add_custom_target(coverage
      COMMAND "${LCOV_EXECUTABLE}" --directory . --zerocounters
      COMMAND "${CMAKE_CTEST_COMMAND}" --output-on-failure
      COMMAND "${LCOV_EXECUTABLE}" --capture --directory . --output-file coverage.info
      COMMAND "${LCOV_EXECUTABLE}" --remove coverage.info "*/_deps/*" "*/tests/*" "/usr/*"
              --output-file coverage.filtered.info
      COMMAND "${GENHTML_EXECUTABLE}" coverage.filtered.info --output-directory coverage
      WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
      DEPENDS gb_tests
    )
  else()
    message(WARNING "Coverage enabled but lcov/genhtml not found; target 'coverage' will not be created.")
  endif()
endif()
