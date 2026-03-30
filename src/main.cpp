#include <stdio.h>
#include <string.h>

#include "bsp/board_api.h"
#include "hardware/clocks.h"
#include "hardware/gpio.h"
#include "hardware/uart.h"
#include "pico/stdlib.h"
#include "pio_usb.h"
#include "tusb.h"

#include "board_config.h"
#include "controller_state.h"
#include "ds4_parser.h"
#include "sbdbt_packet.h"

namespace {

ControllerState g_state{};
uint8_t g_hid_addr = 0;
uint8_t g_hid_instance = 0;
absolute_time_t g_last_report = {};

void log_line(const char* msg) { printf("%s\n", msg); }

void setup_uart() {
  uart_init(uart0, board_config::kSbdbtBaud);
  gpio_set_function(board_config::kSbdbtUartTxPin, GPIO_FUNC_UART);
  gpio_set_function(board_config::kSbdbtUartRxPin, GPIO_FUNC_UART);
}

void send_sbdbt() {
  uint8_t packet[8];
  build_sbdbt_packet(&g_state, packet);
  uart_write_blocking(uart0, packet, sizeof(packet));
}

void setup_heartbeat() {
  gpio_init(board_config::kHeartbeatLedPin);
  gpio_set_dir(board_config::kHeartbeatLedPin, GPIO_OUT);
  gpio_put(board_config::kHeartbeatLedPin, 0);
}

void blink_heartbeat() {
  static uint32_t ticks = 0;
  ++ticks;
  if ((ticks % 25000u) == 0u) {
    gpio_xor_mask(1u << board_config::kHeartbeatLedPin);
  }
}

}  // namespace

extern "C" {

void tuh_mount_cb(uint8_t dev_addr) { printf("[USB] device mounted addr=%u\n", dev_addr); }

void tuh_umount_cb(uint8_t dev_addr) {
  printf("[USB] device unmounted addr=%u\n", dev_addr);
  if (dev_addr == g_hid_addr) {
    g_hid_addr = 0;
    g_hid_instance = 0;
    memset(&g_state, 0, sizeof(g_state));
  }
}

void tuh_hid_mount_cb(uint8_t dev_addr, uint8_t instance, uint8_t const* desc_report,
                      uint16_t desc_len) {
  (void)desc_report;
  (void)desc_len;

  const uint8_t proto = tuh_hid_interface_protocol(dev_addr, instance);
  printf("[HID] mount addr=%u inst=%u proto=%u\n", dev_addr, instance, proto);

  g_hid_addr = dev_addr;
  g_hid_instance = instance;
  g_state.connected = true;

  if (!tuh_hid_receive_report(dev_addr, instance)) {
    printf("[HID] receive request failed at mount\n");
  }
}

void tuh_hid_umount_cb(uint8_t dev_addr, uint8_t instance) {
  printf("[HID] umount addr=%u inst=%u\n", dev_addr, instance);
  if (dev_addr == g_hid_addr && instance == g_hid_instance) {
    g_hid_addr = 0;
    g_hid_instance = 0;
    memset(&g_state, 0, sizeof(g_state));
  }
}

void tuh_hid_report_received_cb(uint8_t dev_addr, uint8_t instance, uint8_t const* report,
                                uint16_t len) {
  if (parse_ds4_report(report, len, &g_state)) {
    g_last_report = get_absolute_time();
    send_sbdbt();
  }

  if (!tuh_hid_receive_report(dev_addr, instance)) {
    printf("[HID] receive request failed addr=%u inst=%u\n", dev_addr, instance);
  }
}

}  // extern "C"

int main() {
  set_sys_clock_khz(board_config::kSysClockKHz, true);
  stdio_init_all();
  setup_default_uart();
  sleep_ms(200);

  setup_uart();
  setup_heartbeat();
  log_line("SBDBT Pico-PIO-USB bring-up start");

  pio_usb_configuration_t pio_cfg = PIO_USB_DEFAULT_CONFIG;
  pio_cfg.pin_dp = board_config::kUsbDpPin;
  pio_cfg.pinout = PIO_USB_PINOUT_DPDM;
  tuh_configure(1, TUH_CFGID_RPI_PIO_USB_CONFIGURATION, &pio_cfg);

  if (!tuh_init(1)) {
    log_line("[USB] tuh_init failed");
    while (true) {
      blink_heartbeat();
      sleep_ms(1);
    }
  }

  log_line("[USB] host init done; waiting for HID device");

  while (true) {
    tuh_task();
    blink_heartbeat();

    if (g_state.connected && absolute_time_diff_us(g_last_report, get_absolute_time()) > 3000000) {
      // Keep alive packet every 3s even without new report.
      send_sbdbt();
      g_last_report = get_absolute_time();
    }
  }
}
