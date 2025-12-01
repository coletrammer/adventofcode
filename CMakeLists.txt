cmake_minimum_required(VERSION 3.21)

include(meta/cmake/prelude.cmake)

project(
  AdventOfCode
  VERSION 0.1.0
  DESCRIPTION "Advent of code solutions"
  HOMEPAGE_URL "https://github.com/coletrammer/adventofcode"
  LANGUAGES CXX)

include(meta/cmake/variables.cmake)

# ---- Dependencies ----

# Drop verify interface header sets for our dependencies.
set(want_header_set_verification ${CMAKE_VERIFY_INTERFACE_HEADER_SETS})
set(CMAKE_VERIFY_INTERFACE_HEADER_SETS OFF)

set(aoc_dius_DIRECTORY
    "${CMAKE_CURRENT_SOURCE_DIR}/dius"
    CACHE STRING "Directory containing the dius library")

if(EXISTS "${aoc_dius_DIRECTORY}")
  message(STATUS "Found dius dependency at ${aoc_dius_DIRECTORY}")
  add_subdirectory("${aoc_dius_DIRECTORY}")
else()
  message(STATUS "Looking for dius library via find_package()")
  find_package(dius REQUIRED)
endif()

# Restore verify interface header sets.
set(CMAKE_VERIFY_INTERFACE_HEADER_SETS ${want_header_set_verification})

# ---- aoc app object lib ----

file(
  GLOB_RECURSE sources CONFIGURE_DEPENDS
  RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}"
  "src/*.cpp")
list(FILTER sources EXCLUDE REGEX src/runner/main.cpp)
file(
  GLOB_RECURSE headers CONFIGURE_DEPENDS
  RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}"
  "src/*.h")

add_library(aoc_app_lib OBJECT ${sources})
add_library(aoc::aoc_app_lib ALIAS aoc_app_lib)
set_property(TARGET aoc_app_lib PROPERTY EXPORT_NAME aoc_app_lib)

target_include_directories(
  aoc_app_lib ${warning_guard}
  PUBLIC "\$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src>")
target_sources(
  aoc_app_lib
  INTERFACE FILE_SET
            HEADERS
            TYPE
            HEADERS
            BASE_DIRS
            src
            FILES
            ${headers})
target_precompile_headers(aoc_app_lib PRIVATE [["runner/aliases.h"]]
                          [["runner/aoc_problem_registry.h"]])
target_link_libraries(aoc_app_lib PUBLIC dius::dius)

# ---- aoc app ----

add_executable(aoc_app src/runner/main.cpp)
set_property(TARGET aoc_app PROPERTY OUTPUT_NAME aoc)
target_link_libraries(aoc_app PRIVATE aoc::aoc_app_lib)

# ---- Install rules ----

if(NOT CMAKE_SKIP_INSTALL_RULES)
  include(meta/cmake/install-rules.cmake)
endif()

# ---- Developer mode ----

if(NOT aoc_DEVELOPER_MODE)
  return()
elseif(NOT PROJECT_IS_TOP_LEVEL)
  message(AUTHOR_WARNING "Developer mode is intended for developers of aoc")
endif()

include(meta/cmake/dev-mode.cmake)
