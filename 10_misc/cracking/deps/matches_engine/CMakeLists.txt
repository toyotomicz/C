cmake_minimum_required(VERSION 3.10.0)

project(matches_engine
    LANGUAGES C
    VERSION 1.0.0
    DESCRIPTION "Matches game engine library"
)

# Set C standard and flags
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Wno-unused-result")

# Create library
add_library(
    ${PROJECT_NAME} STATIC

    src/matches.c
)

# Install library and its header files
set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/install/matches_engine)
install(TARGETS ${PROJECT_NAME} DESTINATION lib)
install(FILES src/matches.h DESTINATION include/${PROJECT_NAME})
