cmake_minimum_required(VERSION 3.5)

project(steganor LANGUAGES C)
set(CMAKE_C_FLAGS "-Wall -Wextra -pedantic")

add_executable(steganodecoder
    src/decoder.c
    src/imgio.c
)