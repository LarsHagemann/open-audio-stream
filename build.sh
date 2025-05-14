#!/usr/bin/env bash

cmake -S . -B build \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
#cmake --build build --target install
cmake --build build