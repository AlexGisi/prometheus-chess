cmake_minimum_required(VERSION 3.20)
project(blue VERSION 0.1)

set(CMAKE_CXX_STANDARD 14)

add_executable(blue src/main.cpp src/Board.cpp src/BitBoard.cpp src/PosKey.cpp src/PosKey.h)

target_include_directories(blue PUBLIC)