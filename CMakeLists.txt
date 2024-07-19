cmake_minimum_required(VERSION 3.20)
project(prometheus VERSION 0.2)

set(CMAKE_CXX_STANDARD 20)

add_executable(prometheus src/main.cpp src/board/board.cpp src/board/bitboard.cpp src/hashing/poskey.cpp src/hashing/poskey.h src/move/move.cpp src/move/move.h src/move/movegen.cpp src/move/movegen.h src/util/assertions.cpp src/util/util.cpp src/search/search.cpp src/hashing/pvtable.cpp src/hashing/pvtable.h src/search/evaluation.cpp src/io/uci.cpp src/io/uci.h)

target_include_directories(prometheus PUBLIC)
