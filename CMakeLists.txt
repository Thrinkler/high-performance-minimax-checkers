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
if(MSVC)
    target_compile_options(MinimaxCheckers PRIVATE /O2 /Oi /Ot /GL /arch:AVX2)
    set_target_properties(MinimaxCheckers PROPERTIES LINK_FLAGS "/LTCG")
    else()
    if(CMAKE_BUILD_TYPE MATCHES "Release")
        target_compile_options(MinimaxCheckers PRIVATE -O3 -march=native -ffast-math)
    endif()
endif()