cmake_minimum_required(VERSION 3.30)
project(csav C)

set(CMAKE_C_STANDARD 11)

add_executable(csav main.c
        csav_pokemon.h
        csav_pokemon.c
        csav_text.c
        csav_text.h
        csav_moves.h
        csav_save.c
        csav_save.h
)
