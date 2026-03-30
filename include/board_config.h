#pragma once

#include <stdint.h>

namespace board_config {

constexpr uint32_t kSysClockKHz = 120000;

// XIAO RP2040: use D6(GPIO0)=TX, D7(GPIO1)=RX for SBDBT UART.
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
#define PIO_USB_DP_PIN 16
#endif
constexpr uint8_t kUsbDpPin = PIO_USB_DP_PIN;

// Diagnostic heartbeat pin (on-board LED on many RP2040 boards is GPIO25).
constexpr uint8_t kHeartbeatLedPin = 25;

}  // namespace board_config
