cmake_minimum_required(VERSION 2.6)
project(tddd16-lab2)

find_package(FLEX)

set(CMAKE_CXX_FLAGS "-g -Wall -std=c++11 -stdlib=libc++")
add_subdirectory(googletest)
enable_testing()
include_directories(${CMAKE_SOURCE_DIR}/include ${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

flex_target(scanner
  lib/scanner.l
  ${CMAKE_BINARY_DIR}/scanner.l.cc
)

add_executable(scanner_test
  ${FLEX_scanner_OUTPUTS}
  test/scanner_test.cc
)
target_link_libraries(scanner_test ${CORELIBS} gtest gtest_main)

add_executable(s
  ${FLEX_scanner_OUTPUTS}
  lib/main.cc
)

