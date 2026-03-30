cmake_minimum_required(VERSION 3.16)

if(NOT DEFINED PICO_SDK_PATH OR "${PICO_SDK_PATH}" STREQUAL "")
  if(DEFINED ENV{PICO_SDK_PATH} AND NOT "$ENV{PICO_SDK_PATH}" STREQUAL "")
    set(PICO_SDK_PATH "$ENV{PICO_SDK_PATH}")
  endif()
endif()

if(NOT EXISTS "${PICO_SDK_PATH}/external/pico_sdk_import.cmake")
  message(FATAL_ERROR "pico-sdk not found. Set PICO_SDK_PATH (e.g. /home/edometro/pico-sdk).")
endif()

include(${PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(sbdbt_pio_usb C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(PICO_BOARD "seeed_xiao_rp2350" CACHE STRING "Target board")

pico_sdk_init()

include(FetchContent)
FetchContent_Declare(
  pico_pio_usb
  GIT_REPOSITORY https://github.com/sekigon-gonnoc/Pico-PIO-USB.git
  GIT_TAG main
)
FetchContent_MakeAvailable(pico_pio_usb)

if(NOT DEFINED PICO_TINYUSB_PATH)
  message(FATAL_ERROR "PICO_TINYUSB_PATH is not defined by pico-sdk.")
endif()

set(PIO_USB_DP_PIN 26 CACHE STRING "PIO-USB D+ GPIO pin (D- is D+1 for DPDM)")
set(SBDBT_UART_TX_PIN 0 CACHE STRING "SBDBT UART TX GPIO pin")
set(SBDBT_UART_RX_PIN 1 CACHE STRING "SBDBT UART RX GPIO pin")
set(SBDBT_UART_BAUD 115200 CACHE STRING "SBDBT UART baudrate")
set(USB_ACTIVITY_LED_PIN 25 CACHE STRING "USB-A activity LED GPIO pin")
set(USER_LED_ACTIVE_LOW 0 CACHE STRING "1 for active-low LED, 0 for active-high")

add_executable(sbdbt_pio_usb
  src/main.cpp
  src/ds4_parser.cpp
  src/sbdbt_packet.cpp
  ${PICO_TINYUSB_PATH}/src/portable/raspberrypi/pio_usb/hcd_pio_usb.c
)

target_include_directories(sbdbt_pio_usb PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/include
  ${CMAKE_CURRENT_LIST_DIR}/src
)

target_compile_definitions(sbdbt_pio_usb PRIVATE
  PIO_USB_USE_TINYUSB=1
  CFG_TUH_RPI_PIO_USB=1
  PICO_DEFAULT_UART=1
  PICO_DEFAULT_UART_TX_PIN=4
  PICO_DEFAULT_UART_RX_PIN=5
  PIO_USB_DP_PIN=${PIO_USB_DP_PIN}
  SBDBT_UART_TX_PIN=${SBDBT_UART_TX_PIN}
  SBDBT_UART_RX_PIN=${SBDBT_UART_RX_PIN}
  SBDBT_UART_BAUD=${SBDBT_UART_BAUD}
  USB_ACTIVITY_LED_PIN=${USB_ACTIVITY_LED_PIN}
  USER_LED_ACTIVE_LOW=${USER_LED_ACTIVE_LOW}
)

target_compile_options(sbdbt_pio_usb PRIVATE -Wall -Wextra)

target_link_libraries(sbdbt_pio_usb PRIVATE
  pico_stdlib
  hardware_uart
  hardware_gpio
  hardware_clocks
  tinyusb_host
  tinyusb_board
  pico_pio_usb
)

pico_enable_stdio_usb(sbdbt_pio_usb 0)
pico_enable_stdio_uart(sbdbt_pio_usb 1)
pico_add_extra_outputs(sbdbt_pio_usb)
