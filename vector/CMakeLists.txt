cmake_minimum_required(VERSION 3.5)

project(loader LANGUAGES C)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra")

add_executable(loader
    src/vector.c
    src/person.c
    src/loader_examples.c
    src/main.c
)
