cmake_minimum_required(VERSION 3.31)
project(SkipList)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pg")

add_executable(SkipList main.cpp
        SkipList.cpp
        SkipList.h)
