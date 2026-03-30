# SBDPICO (Pico SDK + Pico-PIO-USB)

XIAO RP2040 を想定した、`Pico-PIO-USB` ベースの SBDBT 互換ファームウェアです。  
USB HID コントローラ（まずは DS4 有線想定）の入力を受け、SBDBT 形式の 8byte パケットを UART で出力します。

## 構成

- `src/main.cpp`  
  TinyUSB host / Pico-PIO-USB 初期化、HID コールバック、UART 送信ループ
- `src/ds4_parser.cpp`  
  DS4 レポートの解析
- `src/sbdbt_packet.cpp`  
  SBDBT 8byte パケット生成
- `src/tusb_config.h`  
  TinyUSB host 設定
- `include/board_config.h`  
  ピン/ボーレート等のボード設定

## 前提

- pico-sdk: `$HOME/pico-sdk`（または `PICO_SDK_PATH` 環境変数）
- CMake / ARM GCC toolchain
- GitHub から `Pico-PIO-USB` を取得できるネットワーク

## ビルド

```bash
cmake -S . -B build -DPICO_SDK_PATH=$HOME/pico-sdk
cmake --build build -j
```

生成物:

- `build/sbdbt_pio_usb.uf2`
- `build/sbdbt_pio_usb.elf`
- `build/sbdbt_pio_usb.bin`
- `build/sbdbt_pio_usb.hex`

## デフォルト設定

- SYSCLK: `120MHz`
- SBDBT UART: `uart0`, `TX=GPIO0`, `RX=GPIO1`, `115200bps`
- Pico-PIO-USB D+: `GPIO26`（D- は `GPIO27`）
- USB-A 活動LED: `GPIO17`（受信中点滅、未接続/無通信時OFF）

必要なら CMake キャッシュで変更できます:

```bash
cmake -S . -B build \
  -DPICO_SDK_PATH=~$HOME/pico-sdk \
  -DPIO_USB_DP_PIN=26 \
  -DSBDBT_UART_TX_PIN=0 \
  -DSBDBT_UART_RX_PIN=1 \
  -DSBDBT_UART_BAUD=115200 \
  -DUSB_ACTIVITY_LED_PIN=17
```

## 配線（XIAO RP2040）

- USB-C: 書き込み/電源供給（本ファームの `stdio` は USB-C CDC を使いません）
- SBDBT UART:
  - `GPIO0 (XIAO D6)` -> 受信側 `RX`
  - `GPIO1 (XIAO D7)` -> 受信側 `TX`（必要時）
  - `GND` 共通
- USB-A（ホスト）データ:
  - `GPIO26 (D0 Analog)` -> USB D+
  - `GPIO27 (D1 Analog)` -> USB D-
  - `VBUS 5V` と `GND` はUSB-A側に適切に供給/共通化
- 状態表示LED:
  - `GPIO17 (USER_LED_R)` を活動表示に使用
  - HIDレポート受信中: 点滅
  - 未接続または通信停止時: 消灯
  - `GPIO25 (USER_LED_B)` は明示的に消灯固定

## 注意

- XIAO RP2040 では USB ホスト動作に配線品質・電源余裕が影響します。
- 列挙不安定時は、USB ログ（`printf`）を見ながら電源/配線とソフト設定を切り分けてください。
