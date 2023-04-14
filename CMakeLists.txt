cmake_minimum_required(VERSION 3.20)
project(7yMedio C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(7yMedio
        inputs.c
        inputs.h
        cards.c
        cards.h
        main.c)
