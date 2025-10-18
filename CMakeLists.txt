cmake_minimum_required(VERSION 3.28)

if(NOT KVASIR_ROOT)
    if(DEFINED ENV{KVASIR_ROOT})
        set(KVASIR_ROOT $ENV{KVASIR_ROOT})
    else()
        message(
            FATAL_ERROR
                "KVASIR_ROOT is not set. Please set it as a CMake variable (-DKVASIR_ROOT=<path>) or environment variable (export KVASIR_ROOT=<path>)"
        )
    endif()
endif()

include(${KVASIR_ROOT}/cmake/kvasir.cmake)

project(
    "rp2350_template"
    VERSION 0.1.0
    DESCRIPTION "RP2350 firmware template using Kvasir SDK")

include(${KVASIR_ROOT}/cmake/util.cmake)

pioasm_generate(ws2812Pio INPUT_FILE ${CHIP_ROOT}/src/chip/rp_common/pio/ws2812.pio)

kvasir_executable_variants("" SOURCES src/main.cpp LIBRARIES ws2812Pio OPTIMIZATION size)
