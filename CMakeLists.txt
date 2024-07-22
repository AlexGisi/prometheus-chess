cmake_minimum_required(VERSION 3.20)
project(prometheus VERSION 0.2)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

SET(CMAKE_CXX_FLAGS "-pedantic")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wextra")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wconversion")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wshadow")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wcast-qual")

set(SOURCES
        src/main.cpp
        src/board/board.cpp
        src/board/bitboard.cpp
        src/hashing/poskey.cpp
        src/hashing/poskey.h
        src/move/move.cpp
        src/move/move.h
        src/move/movegen.cpp
        src/move/movegen.h
        src/util/assertions.cpp
        src/util/util.cpp
        src/search/search.cpp
        src/hashing/pvtable.cpp
        src/hashing/pvtable.h
        src/search/evaluation.cpp
        src/io/uci.cpp
        src/io/uci.h
)

add_executable(prometheus ${SOURCES})

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
    target_compile_options(prometheus PRIVATE
            -pedantic
            -Wall
            -Wextra
            -Wconversion
            -Wshadow
            -Wcast-qual
            -flto
            )
    message(STATUS "Using GCC or Clang compiler")

elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(prometheus PRIVATE
            /W4
            )
    message(STATUS "Using MSVC compiler")

else()
    message(WARNING "Unsupported compiler ${CMAKE_CXX_COMPILER_ID}")
endif()

target_include_directories(prometheus PUBLIC)
