cmake_minimum_required(VERSION 3.5)
project(cpp_example LANGUAGES CXX)

# set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# set compiler flags
if (MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -W4 -EHsc -WX")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")
endif()

# add executable to our project
add_executable(${PROJECT_NAME}
    src/main.cpp
    src/units/wiffle.cpp
    src/examples/examples.cpp
)
