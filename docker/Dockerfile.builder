FROM ubuntu:22.04

RUN apt-get update && apt-get install -y \
    build-essential \
    git \
    cmake \
    gstreamer1.0-plugins-base \
    gstreamer1.0-plugins-good \
    gstreamer1.0-plugins-bad \
    gstreamer1.0-plugins-ugly \
    gstreamer1.0-tools \
    libgstreamer1.0-dev \
    libgstreamer-plugins-base1.0-dev

COPY .. /app
WORKDIR /app

RUN rm -rf build
RUN ./install_build_dep.sh
RUN mkdir build && \
    cmake -S . -B build \
    -DCMAKE_BUILD_TYPE=Release

RUN cmake --build build --target OHDInterfaceLib OHDVideoLib OHDTelemetryLib OHDCommonLib

RUN cmake --build build --target open-audio-transmitter
RUN cmake --build build --target open-audio-receiver

WORKDIR /app/build
RUN strip open-audio-transmitter
RUN strip open-audio-receiver
