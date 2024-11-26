cmake_minimum_required(VERSION 3.10)

project(stack_size LANGUAGES C)

# Pojďme si zjistit velikost našich zásobníků.
add_executable(
    stack_size

    src/stack_size.c
)

# pro GCC překladač (u jiných překladačů budou jiné přepínače)
if (CMAKE_C_COMPILER_ID STREQUAL "GNU")
    # můžeme vypnout warning upozoňující na nekonečnou rekurzi -- to ovšem nikdy dělat nechceme!
    # necháme si vygenerovat i assembly
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Wno-infinite-recursion -save-temps")
endif()
