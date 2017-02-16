cmake_minimum_required(VERSION 3.6)
project(PMCSN)

set(CMAKE_C_STANDARD 90)

set(CMAKE_C_COMPILER "/usr/bin/gcc")
set(CMAKE_C_FLAGS "-Wall -Wextra")

set(SOURCE_FILES main.c utils.h utils.c rng.h rng.c)
add_executable(PMCSN ${SOURCE_FILES})