cmake_minimum_required(VERSION 3.10)

project(tail_recursion_optimization_example LANGUAGES C)

# pro GCC překladač (u jiných překladačů budou jiné přepínače)
if (CMAKE_C_COMPILER_ID STREQUAL "GNU")
    # klasické warningy + si necháme vygenerovat i assembly souborů
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -save-temps")
endif()


add_executable(const_factorial
    src/const_factorial.c
)

add_executable(dynamic_factorial
    src/dynamic_factorial.c
)
