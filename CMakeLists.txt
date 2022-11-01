cmake_minimum_required(VERSION 3.21)
project(ConBPTree C)

set(CMAKE_C_STANDARD 11)
set(SRC
        lib/bplustree.c
        lib/bplustree.h
        )
add_executable(ConBPTree ${SRC} main.c)
