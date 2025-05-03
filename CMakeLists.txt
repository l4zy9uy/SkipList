cmake_minimum_required(VERSION 3.31)
project(SkipList)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pg")

find_package(Catch2 3 REQUIRED)

enable_testing()

# These tests can use the Catch2-provided main
add_executable(tests SkipList.h test.cpp)
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)

add_test(
  NAME skiplist_tests
  COMMAND tests
)