cmake_minimum_required(VERSION 3.10.0)

# Create new project
project(matches
    LANGUAGES C
    VERSION 1.0.0
    DESCRIPTION "Matches game application"
)

# Set C standard and flags
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Wno-unused-result")

# Create game executable
add_executable(${PROJECT_NAME}
    src/md5/md5-crypt.c
    src/md5/md5.c
    src/game.c
)

# Link Matches engine library
target_include_directories(
    ${PROJECT_NAME} PUBLIC

    ${CMAKE_CURRENT_SOURCE_DIR}/deps/matches_engine/install/matches_engine/include
)
target_link_libraries(
    ${PROJECT_NAME} PUBLIC

    ${CMAKE_CURRENT_SOURCE_DIR}/deps/matches_engine/install/matches_engine/lib/libmatches_engine.a
)
