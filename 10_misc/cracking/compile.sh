#!/bin/bash

### Sestavení aplikace s užitím knihovny `deps/matches_engine`
# Připraveníme si adresář pro sestavení knihovny
BUILD_PREFIX="build"
rm -rf "${BUILD_PREFIX}"
mkdir "${BUILD_PREFIX}"

gcc \
    src/game.c src/md5/md5.c src/md5/md5-crypt.c \
    -o "${BUILD_PREFIX}/matches" \
    -Ideps/matches_engine/build/deploy/include \
    -Ldeps/matches_engine/build/deploy/lib -lmatches_engine \
    -Wall -Wextra
