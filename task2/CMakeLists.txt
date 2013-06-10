cmake_minimum_required(VERSION 2.6.4)

project(task2)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/../CMakeModules/")

if(NOT PHEET_PATH)
    set(PHEET_PATH "${CMAKE_SOURCE_DIR}/../../pheet")
endif()

include(FindPkgConfig)
pkg_search_module(HWLOC REQUIRED hwloc)

find_path(PHEET_INCLUDE_DIR pheet/pheet.h PATHS ${PHEET_PATH})
if(PHEET_INCLUDE_DIR)
    message(STATUS "Pheet found in ${PHEET_INCLUDE_DIR}")
else()
    message(FATAL_ERROR "Pheet not found")
endif()

find_package(Check REQUIRED)
find_package(Threads REQUIRED)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    add_definitions("-Wall -Wextra -pedantic -std=gnu++11 -O3")
endif()

enable_testing()

add_subdirectory(src)
add_subdirectory(test)
