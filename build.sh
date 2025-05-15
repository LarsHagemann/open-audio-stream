#!/usr/bin/env bash

# cmake -S . -B build \
#     -DCMAKE_BUILD_TYPE=Release \
#     -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
# #cmake --build build --target install
# cmake --build build

docker build -f docker/Dockerfile.builder -t audio-stream-builder .
docker build -f docker/Dockerfile.transmitter -t audio-stream-transmitter .
docker build -f docker/Dockerfile.receiver -t audio-stream-receiver .
