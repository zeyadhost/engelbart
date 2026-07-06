cmake_minimum_required(VERSION 3.20)

include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(engelbart_pico_tinyusb_smoke C CXX ASM)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

pico_sdk_init()

add_executable(engelbart_pico_tinyusb_smoke
    main.cpp
)

target_include_directories(engelbart_pico_tinyusb_smoke PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/../../include
    ${CMAKE_CURRENT_LIST_DIR}
)

target_link_libraries(engelbart_pico_tinyusb_smoke
    pico_stdlib
    tinyusb_device
    tinyusb_board
)
