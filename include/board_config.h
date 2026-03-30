#pragma once

#include <stdint.h>

namespace board_config {

constexpr uint32_t kSysClockKHz = 120000;

// XIAO RP2350: use D6(GPIO0)=TX, D7(GPIO1)=RX for SBDBT UART.
#ifndef SBDBT_UART_TX_PIN
#define SBDBT_UART_TX_PIN 0
#endif
#ifndef SBDBT_UART_RX_PIN
#define SBDBT_UART_RX_PIN 1
#endif
#ifndef SBDBT_UART_BAUD
#define SBDBT_UART_BAUD 115200
#endif
constexpr uint8_t kSbdbtUartTxPin = SBDBT_UART_TX_PIN;
constexpr uint8_t kSbdbtUartRxPin = SBDBT_UART_RX_PIN;
constexpr uint32_t kSbdbtBaud = SBDBT_UART_BAUD;

// Pico-PIO-USB host pins: DM is expected at DP + 1.
#ifndef PIO_USB_DP_PIN
#define PIO_USB_DP_PIN 26
#endif
constexpr uint8_t kUsbDpPin = PIO_USB_DP_PIN;

// USB-A activity LED (XIAO RP2350 USER_LED).
#ifndef USB_ACTIVITY_LED_PIN
#define USB_ACTIVITY_LED_PIN 25
#endif
constexpr uint8_t kUsbActivityLedPin = USB_ACTIVITY_LED_PIN;

// 1 if LED is active-low, 0 if active-high.
#ifndef USER_LED_ACTIVE_LOW
#define USER_LED_ACTIVE_LOW 0
#endif
constexpr bool kUserLedActiveLow = USER_LED_ACTIVE_LOW != 0;

}  // namespace board_config
