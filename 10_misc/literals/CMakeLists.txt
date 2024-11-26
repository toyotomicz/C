cmake_minimum_required(VERSION 3.10)

project(stack_size LANGUAGES C)

add_executable(
    const_literals
    src/const_literals.c
)

add_executable(
    string_comparison
    src/string_comparison.c
)

# pro GCC překladač (u jiných překladačů budou jiné přepínače)
if (CMAKE_C_COMPILER_ID STREQUAL "GNU")
    # necháme si vygenerovat i assembly
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -save-temps")
endif()
