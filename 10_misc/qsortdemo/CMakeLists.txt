cmake_minimum_required(VERSION 3.10)

project(qsortdemo LANGUAGES C ASM)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -save-temps")

# Tento cíl bude možné přeložit pouze pokud bude v souboru `src/qsortdemo.c` definována funkce `swap`.
add_executable(
    qsortdemo
    src/qsortdemo.c
)

# Pro přeložení tohoto cíle je třeba správně upravit soubor `src/qsortdemo.c`
# -> odstranit definici funkce `swap` a ponechat pouze její deklaraci.
add_executable(
    qsortdemo_asm_gcc_x86_64
    src/qsortdemo.c
    src/swap_att_gcc_x86_64.s
)
