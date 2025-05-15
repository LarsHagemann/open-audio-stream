cmake_minimum_required(VERSION 3.20)
project(audio-stream LANGUAGES CXX VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# fetch content
include(FetchContent)

FetchContent_Declare(
    OpenHD
    GIT_REPOSITORY https://github.com/OpenHD/OpenHD.git
    GIT_TAG        6360d3f719121ac9f840bbeb25f12225e44626f7
)
FetchContent_MakeAvailable(OpenHD)
set(OPENHD_SOURCE_DIR "${openhd_SOURCE_DIR}")
set(OPENHD_BINARY_DIR "${openhd_BINARY_DIR}")

function(dump_cmake_variables)
    get_cmake_property(_variableNames VARIABLES)
    list (SORT _variableNames)
    foreach (_variableName ${_variableNames})
        if (ARGV0)
            unset(MATCHED)
            string(REGEX MATCH ${ARGV0} MATCHED ${_variableName})
            if (NOT MATCHED)
                continue()
            endif()
        endif()
        message(STATUS "${_variableName}=${${_variableName}}")
    endforeach()
endfunction()

# dump_cmake_variables()
message(STATUS "OpenHD source dir: ${OPENHD_SOURCE_DIR}")

add_subdirectory(${OPENHD_SOURCE_DIR}/OpenHD/)

add_executable(open-audio-transmitter src/main-transmitter.cpp)
target_link_libraries(open-audio-transmitter
    PRIVATE
        OHDInterfaceLib 
        OHDVideoLib 
        OHDTelemetryLib 
        OHDCommonLib
)
target_include_directories(open-audio-transmitter
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${OPENHD_SOURCE_DIR}/OpenHD/ohd_interface/inc
        ${OPENHD_SOURCE_DIR}/OpenHD/ohd_video/inc
        ${OPENHD_SOURCE_DIR}/OpenHD/ohd_telemetry/inc
        ${OPENHD_SOURCE_DIR}/OpenHD/ohd_common/inc
)

add_executable(open-audio-receiver src/main-receiver.cpp)
target_link_libraries(open-audio-receiver
    PRIVATE
        OHDInterfaceLib 
        OHDVideoLib 
        OHDTelemetryLib 
        OHDCommonLib
)
target_include_directories(open-audio-receiver
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${OPENHD_SOURCE_DIR}/OpenHD/ohd_interface/inc
        ${OPENHD_SOURCE_DIR}/OpenHD/ohd_video/inc
        ${OPENHD_SOURCE_DIR}/OpenHD/ohd_telemetry/inc
        ${OPENHD_SOURCE_DIR}/OpenHD/ohd_common/inc
)
