cmake_minimum_required(VERSION 3.5)

project(spirals LANGUAGES C)
set(CMAKE_C_FLAGS "-Wall -Wextra -pedantic")

add_executable(spirals 
    src/spirals.c 
    
    # Podadresář s náší miniaturní knihovnou.    
    src/matrix/matrix.c
)

target_link_libraries(spirals m)
