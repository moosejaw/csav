cmake_minimum_required(VERSION 3.30)
project(csav C)

set(CMAKE_C_STANDARD 11)

add_executable(csav main.c
        pokemon.h
        pokemon.c
        util.c
        util.h
        moves.h
)
