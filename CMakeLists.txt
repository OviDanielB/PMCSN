cmake_minimum_required(VERSION 3.6)
project(PMCSN)

set(CMAKE_C_STANDARD 90)

set(CMAKE_C_COMPILER "/usr/bin/gcc")
set(CMAKE_C_FLAGS "-Wall -Wextra")

set(SOURCE_FILES  utils.h utils.c rng.h rng.c  rng_lib/rngs.h rng_lib/rngs.c
        rng_lib/rvgs.h rng_lib/rvgs.c rng_lib/rvms.h rng_lib/rvms.c tests/independence_test.c empirical_test_randomness.c empirical_test_randomness.h)

add_executable(PMCSN ${SOURCE_FILES})