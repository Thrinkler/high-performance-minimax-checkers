cmake_minimum_required(VERSION 3.20)
project(MinimaxCheckers)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(board)
include_directories(bots/minimax)
include_directories(bots/random)

add_executable(MinimaxCheckers
        board/board.cpp
        board/board.h
        board/gameMaster.cpp
        board/gameMaster.h
        bots/minimax/minimax.cpp
        bots/minimax/minimax.h
        bots/random/random.cpp
        bots/random/random.h
        main.cpp)
# Activar optimizaciones agresivas
if(CMAKE_BUILD_TYPE MATCHES "Release")
    add_compile_options(-O3 -march=native -ffast-math)
endif()