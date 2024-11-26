#!/bin/bash

### Sestavení knihovny
# Připraveníme si adresář pro sestavení knihovny
BUILD_PREFIX="build"
rm -rf "${BUILD_PREFIX}"
mkdir "${BUILD_PREFIX}"

# Ze zdrojových souborů vytvoříme objektové soubory, které následně zabalíme do knihovny
gcc -o "${BUILD_PREFIX}/matches.o" -c "src/matches.c"
ar -rc "${BUILD_PREFIX}/libmatches_engine.a" "build/matches.o"

### Instalace knihovny
INSTALL_PREFIX="build/deploy"

# Instalace zkompilované statické knihovny
lib_prefix="${INSTALL_PREFIX}/lib"
mkdir -p "${lib_prefix}"
cp "${BUILD_PREFIX}/libmatches_engine.a" ${lib_prefix}/libmatches_engine.a

# Instalace hlavičkových souborů
include_prefix="${INSTALL_PREFIX}/include/matches_engine"
mkdir -p "${include_prefix}"
cp src/matches.h "${include_prefix}/matches.h"

# V adresáři `INSTALL_PREFIX` tedy máme připravenou zkompilovou knihovnu a hlavičkové soubory
