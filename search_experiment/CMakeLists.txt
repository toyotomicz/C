cmake_minimum_required(VERSION 3.5)

project(search_experiment LANGUAGES C)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra")

add_executable(
    ${PROJECT_NAME}

    src/search_experiment.c
    src/searchers/searchers.c
)

target_link_libraries(search_experiment m)  # because of the `pow` function
