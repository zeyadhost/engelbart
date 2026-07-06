cmake_minimum_required(VERSION 3.20)

include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(engelbart_pico_tinyusb_smoke C CXX ASM)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

pico_sdk_init()

add_executable(engelbart_pico_tinyusb_smoke
    main.cpp
    ${PICO_SDK_PATH}/src/rp2_common/pico_stdio_usb/reset_interface.c
)

target_include_directories(engelbart_pico_tinyusb_smoke PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/../../include
    ${CMAKE_CURRENT_LIST_DIR}
)

target_compile_definitions(engelbart_pico_tinyusb_smoke PRIVATE
    PICO_STDIO_USB_ENABLE_RESET_VIA_VENDOR_INTERFACE=1
    PICO_STDIO_USB_ENABLE_RESET_VIA_BAUD_RATE=0
    PICO_STDIO_USB_RESET_INTERFACE_SUPPORT_RESET_TO_BOOTSEL=1
    PICO_STDIO_USB_RESET_INTERFACE_SUPPORT_RESET_TO_FLASH_BOOT=1
    PICO_STDIO_USB_RESET_INTERFACE_SUPPORT_MS_OS_20_DESCRIPTOR=0
    PICO_STDIO_USB_RESET_BOOTSEL_INTERFACE_DISABLE_MASK=0u
    PICO_STDIO_USB_RESET_RESET_TO_FLASH_DELAY_MS=100
)

target_link_libraries(engelbart_pico_tinyusb_smoke
    pico_stdlib
    pico_stdio_usb_headers
    pico_usb_reset_interface
    hardware_watchdog
    tinyusb_device
    tinyusb_board
)

pico_set_program_name(engelbart_pico_tinyusb_smoke "engelbart_pico_tinyusb_smoke")
pico_set_program_version(engelbart_pico_tinyusb_smoke "dev")

pico_enable_stdio_uart(engelbart_pico_tinyusb_smoke 0)
pico_add_extra_outputs(engelbart_pico_tinyusb_smoke)
