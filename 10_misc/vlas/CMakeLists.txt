cmake_minimum_required(VERSION 3.10)

project(mean_using_vlas LANGUAGES C)

# Set C standard
set(CMAKE_C_STANDARD 99)

add_executable(
    bad_mean
    src/bad_mean.c
)

add_executable(
    cool_mean
    src/cool_mean.c
)

# pro GCC překladač (u jiných překladačů budou jiné přepínače)
if (CMAKE_C_COMPILER_ID STREQUAL "GNU")
    # necháme si vygenerovat i assembly
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -save-temps")
endif()
