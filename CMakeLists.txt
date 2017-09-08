cmake_minimum_required(VERSION 3.6)
project(PMCSN)

set(CMAKE_C_STANDARD 99)

set(CMAKE_C_COMPILER "/usr/bin/gcc")
set(CMAKE_C_FLAGS "-Wall -Wextra")

set(SOURCE_FILES utils/utils.h utils/utils.c utils/rng_utils.h utils/rng_utils.c rng_lib/rngs.h rng_lib/rngs.c
        rng_lib/rvgs.h rng_lib/rvgs.c rng_lib/rvms.h rng_lib/rvms.c
        randomness_test/empirical_test_randomness.c randomness_test/empirical_test_randomness.h
        utils/constants.h event_time_generator.c event_time_generator.h main.c
        task_queue.c task_queue.h)

add_executable(PMCSN ${SOURCE_FILES})

target_link_libraries(PMCSN m)