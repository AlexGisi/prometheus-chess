cmake_minimum_required(VERSION 3.20)
project(athene VERSION 0.1)

set(CMAKE_CXX_STANDARD 20)

add_executable(athene src/main.cpp src/Board.cpp src/BitBoard.cpp src/PosKey.cpp src/PosKey.h src/Move.cpp src/Move.h src/MoveGen.cpp src/MoveGen.h src/assertions.cpp src/MoveList.cpp src/MoveList.h src/util.cpp src/search.cpp src/PVTable.cpp src/PVTable.h src/evaluation.cpp src/uci.cpp src/uci.h)

target_include_directories(athene PUBLIC)