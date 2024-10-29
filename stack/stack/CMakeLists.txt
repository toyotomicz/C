cmake_minimum_required(VERSION 3.5)

project(spirals LANGUAGES C)
set(CMAKE_C_FLAGS "-Wall -Wextra -pedantic -ansi")

add_executable(parser
    src/parser.c
    src/operators.c

    # Podadresář s implementací zásobníku.
    src/stack/stack.c
)
