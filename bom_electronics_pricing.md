# Smart Pad Dispenser — Electronics Pricing (Nairobi)

> **Prices checked**: 4 Oct 2026, from each shop's website. Shelf prices can differ, so call ahead.
> **Scope**: electronics only (one prototype unit). Mechanical parts, the SIM card and fasteners stay in [bom_hardware.md](bom_hardware.md).
> **Stock**: "in stock" / "OUT" is what the shop's site showed. Pixel Electric doesn't show stock online.

---

## Shops

| Shop | Location | Phone | Notes |
|------|----------|-------|-------|
| **K-Technics** | Munyu Road Business Center, 2nd Floor, Rm C6, Nairobi CBD | +254 712 799123 | [ktechnics.com](https://www.ktechnics.com) |
| **ArduinoTech Kenya** | Travis Business Center, 3rd Floor, Rm A3, Mfangano Street, Nairobi CBD (same building as Quickmart Mfangano) | +254 724 011572 | [arduinotech.co.ke](https://arduinotech.co.ke) |
| Pixel Electric | Pixel Engineering Building, Nakuru–Kabarak Road, **Nakuru**. Ships countrywide | +254 722 333997 | [pixelelectric.com](https://www.pixelelectric.com). Use only for items the CBD shops don't have |
| Nerokas | Mama Simba Lane, Garissa Rd, **Thika**. Ships countrywide | +254 706 246248 | [store.nerokas.co.ke](https://store.nerokas.co.ke). No cash, M-Pesa/card only |

---

## Design changes this pricing forces

Shop stock and later design decisions changed four things in the design I drew earlier:

1. **Power input: 12 V adapter + buck converter instead of a 5 V 4 A adapter.** None of the four shops sells a 5 V barrel adapter above 2 A. A 12 V 2 A adapter into an XL4005 buck set to 5.1 V gives up to ~4.3 A at 5 V, and keeps mains wiring outside the enclosure. The fuse and polarity diode move to the 12 V side.
2. **Connectivity: A7672E 4G LTE Cat-1 instead of the SIM800L.** The SIM800L is 2G-only and struggles with the TLS 1.2 encryption modern servers require. The A7672E covers Safaricom's LTE bands (B3/B20), falls back to 2G, and comes with an LTE antenna. Its listing doesn't state the board's supply voltage or logic level, so confirm both at the counter. The diagram assumes 5 V in and 3.3 V logic. If it only takes 3.4–4.2 V, add a buck converter set to 3.8 V (LM2596, K-Technics, KES 200). Wire its PWRKEY pin to GPIO23. The firmware needs the TinyGSM `A7672X` modem type instead of `SIM800`.
3. **Stock sensor: VL6180X time-of-flight instead of the HC-SR04.** It is accurate to the millimetre at short range and its narrow beam doesn't echo off the chamber walls. It shares the OLED's I2C bus (address 0x29). Of the two pins it frees, GPIO23 goes to the modem PWRKEY and GPIO25 is spare. It only reads reliably up to ~100 mm, so mount it so an empty chamber is still within range.
4. **IR break-beam: no ready-made break-beam pair is sold locally.** There are two options in the table below. The recommended E18-D80NK reads the same way the firmware expects. The bare IR pair is cheaper, but it needs a bench test and a one-line firmware change.

Also note:
- **ESP32 board.** Neither CBD shop has a standard ESP32-WROOM-32 DevKit in stock. (Their ESP32-S3 boards use a different pinout, so they won't work without rewiring.) The 30-pin Pixel board exposes every GPIO in `config.h`, but it is smaller than the 38-pin board, so re-check `tray_esp32.stl`.
- **Servo.** The MG996R is out of stock at K-Technics. The MG946R at ArduinoTech has the same body size and more torque.

---

## 1. Main modules

| # | Part | Qty | Unit (KES) | Total (KES) | Shop | Stock | Link / notes |
|---|------|-----|-----------:|------------:|------|-------|--------------|
| 1 | ESP32 DevKit, ESP-WROOM-32, 30-pin, CP2102 | 1 | 1,400 | 1,400 | Pixel Electric | not shown | [link](https://www.pixelelectric.com/iot-development-boards/esp32s-devkit-wifi-ble-module-30pin/). Alt: K-Technics 38-pin, 1,500, **OUT** ([link](https://www.ktechnics.com/products/esp32-wifi-bluetooth-development-board-type-c-38pin)) |
| 2 | A7672E 4G LTE Cat-1 board **+ LTE antenna** | 1 | 5,500 | 5,500 | ArduinoTech | in stock | [link](https://arduinotech.co.ke/product/a7672e-4g-lte-module-lte-cat-1-gsm-gprs-development-board-for-esp32-arduino-and-iot). Confirm 5 V input and 3.3 V logic. Alt: SIM7600E breakout, Nerokas, 7,500 |
| 3 | MG946R metal-gear servo (MG996R size) | 1 | 950 | 950 | ArduinoTech | in stock | [link](https://arduinotech.co.ke/product/mg946r-180-metal-gear-high-torque-servo-motor-digital-servo-for-arduino-rc-and-robotics). Alt: MG996R at Nerokas 1,500 / K-Technics 1,100 **OUT** |
| 4 | 0.96" OLED 128×64, I2C, SSD1306 | 1 | 550 | 550 | ArduinoTech | in stock | [link](https://arduinotech.co.ke/product/0-96-inch-oled-display-128-64-i2c-ssd1306-screen-for-arduino-esp32-and-raspberry-pi). Alt: K-Technics 600 |
| 5 | 4×4 membrane keypad | 1 | 120 | 120 | K-Technics | in stock | [link](https://www.ktechnics.com/products/4x4-matrix-membrane-keypad). Alt: ArduinoTech 150 |
| 6 | VL6180X time-of-flight sensor (MCU-6180 board, 2.8 V regulator + I2C level shifter) | 1 | 1,100 | 1,100 | Pixel Electric | not shown | [link](https://www.pixelelectric.com/pir-ir-and-optical-sensor/mcu-6180-vl6180-proximity-sensor-ambient-light-sensor-gesture/). Not stocked by the CBD shops. VL53L0X (ArduinoTech, 750) is in stock but can't read under ~30 mm |
| 7a | **IR pad sensor (recommended)**: E18-D80NK adjustable IR proximity switch | 1 | 600 | 600 | K-Technics | in stock | [link](https://www.ktechnics.com/products/e18-d80nk-proximity-sensor-switch). See note A |
| 7b | IR pad sensor (cost-down): 5 mm 940 nm IR LED + receiver pair | 1 pair | 20 | (20) | K-Technics | in stock | [link](https://www.ktechnics.com/products/infrared-receiverir-led-transmitter-pair-diode-5mm). See note B. Not in total |
| 8 | Active buzzer 5 V | 1 | 25 | 25 | K-Technics | in stock | [link](https://www.ktechnics.com/products/active-buzzer-5v) |
| 9 | 5 mm LED, red / yellow (amber) / green | 3 | 5 | 15 | K-Technics | in stock | [link](https://www.ktechnics.com/products/diffused-5mm-led-multi-color-assorted-options) |
| | **Subtotal** | | | **10,260** | | | |

**Note A, E18-D80NK.** Power it from the 5 V rail (it doesn't need GPIO18) and pull its output up to 3V3 with 10 kΩ into GPIO19. Its output goes LOW when it sees a pad and HIGH when the slot is empty, which matches the firmware's `HIGH = pad taken`. Before connecting it to the ESP32, measure the output with nothing in front of it. Some units have an internal pull-up to 5 V. If yours reads ~5 V, add a 10k/20k divider.

**Note B, bare IR pair.** Use a 150 Ω resistor in series with the IR LED on GPIO18 (about 13 mA). The receiver is a photodiode, so its signal is weak. Bench-test it across the real chute width before committing, and expect to need a 100 kΩ+ pull-up or a transistor stage. This circuit reads **LOW when the beam is clear**, the opposite of the firmware, so `AWAITING_PAD_REMOVAL` would need its comparison inverted.

---

## 2. Power

| # | Part | Qty | Unit (KES) | Total (KES) | Shop | Stock | Link / notes |
|---|------|-----|-----------:|------------:|------|-------|--------------|
| 10 | 12 V 2 A adapter, 5.5×2.1 mm barrel | 1 | 500 | 500 | K-Technics | in stock | [link](https://www.ktechnics.com/products/12v-2a-ac-to-dc-power-supply-adapter-5-5mm-x-2-1mm-barrel-jack). Alt: Nerokas 800 |
| 11 | DC barrel jack, panel mount, 5.5×2.1 mm | 1 | 20 | 20 | K-Technics | in stock | [link](https://www.ktechnics.com/products/dc-power-supply-female-barrel-jack-socket-5-5mm-x-2-1mm-panel-mount) |
| 12 | XL4005 5 A adjustable buck, set to 5.1 V | 1 | 300 | 300 | K-Technics | in stock | [link](https://www.ktechnics.com/products/5a-xl4005-dc-dc-adjustable-step-down-power-supply-module-buck-converter). Alt: Pixel 12V→5V 5A fixed module, 500 |
| 13 | SS34 Schottky 3 A 40 V (reverse polarity, 12 V side) | 1 | 5 | 5 | K-Technics | in stock | [link](https://www.ktechnics.com/products/smd-schottky-diode-ss34-1n5822-3a-40v-high-efficiency-rectifier). SMD. Through-hole alt: 1N5822 |
| 14 | Glass fuse 6×30 mm, **3 A** fast-blow (12 V side) | 2 | 5 | 10 | ArduinoTech | in stock | [link](https://arduinotech.co.ke/product/glass-tube-fuse-6-30mm-250v-fast-blow-cartridge-fuse-for-electronic-and-electrical-protection). One spare |
| 15 | Inline fuse holder 6×30 mm, 18 AWG | 1 | 120 | 120 | Pixel Electric | not shown | [link](https://www.pixelelectric.com/fuse-carriers/6-30mm-fuse-casing-pipe-with-18awg/). Also sold at Luthuli Ave stalls |
| 16 | Rocker switch, 2-pin | 1 | 50 | 50 | K-Technics | in stock | [link](https://www.ktechnics.com/products/black-rocker-on-off-switch-2-pin) |
| 17 | Electrolytic cap 1000 µF, ≥10 V (modem VIN, servo V+) | 2 | 10 | 20 | ArduinoTech | in stock | [link](https://arduinotech.co.ke/product/electrolytic-capacitors-for-arduino-power-supplies-amplifiers-and-diy-electronics-projects). Ask for 16 V |
| 18 | Ceramic cap 100 nF | 4 | 3 | 12 | K-Technics | in stock | [link](https://www.ktechnics.com/products/ceramic-disc-capacitor-50v-through-hole-multiple-values-available) |
| | **Subtotal** | | | **1,037** | | | |

---

## 3. Support passives

| # | Part | Qty | Unit (KES) | Total (KES) | Shop | Stock | Link / notes |
|---|------|-----|-----------:|------------:|------|-------|--------------|
| 19 | Resistor 10 kΩ ¼ W (keypad rows ×4, IR output ×1) | 5 | 2 | 10 | K-Technics | in stock | [link](https://www.ktechnics.com/products/1-4w-resistor-through-hole-multiple-values-available) |
| 20 | Resistor 1 kΩ ¼ W (buzzer transistor base) | 1 | 2 | 2 | K-Technics | in stock | same listing |
| 21 | Resistor 220 Ω ¼ W (LEDs) | 3 | 2 | 6 | K-Technics | in stock | same listing |
| 22 | NPN transistor S8050 or 2N2222 (buzzer driver) | 1 | 10 | 10 | ArduinoTech | in stock | [link](https://arduinotech.co.ke/product/bjt-bipolar-power-transistors-npn-and-pnp-through-hole-transistors-for-amplifier-and-switching-circuits). Name the part at the counter. Pixel S8050: 5 |
| 23 | Diode 1N4007 (buzzer flyback) | 1 | 5 | 5 | ArduinoTech | in stock | [link](https://arduinotech.co.ke/product/rectifier-diode-through-hole-silicon-diode-for-power-supply-and-ac-to-dc-conversion). 1N4148 at K-Technics is OUT |
| | **Subtotal** | | | **33** | | | |

No longer needed: LM2596, the 1k/2k divider and the 2200 µF cap (all for the SIM800L), and the HC-SR04.

---

## 4. Board and wiring

| # | Part | Qty | Unit (KES) | Total (KES) | Shop | Stock | Link / notes |
|---|------|-----|-----------:|------------:|------|-------|--------------|
| 24 | Prototype PCB 7×9 cm | 1 | 100 | 100 | ArduinoTech | in stock | [link](https://arduinotech.co.ke/product/7-9cm-single-side-prototype-pcb-bakelite-copper-board-for-arduino-and-diy-circuits). Double-sided: K-Technics 160 |
| 25 | Female header 1×40, 2.54 mm (ESP32 + modem sockets) | 2 | 20 | 40 | K-Technics | in stock | [link](https://www.ktechnics.com/products/header-pins-female-connector-40-pins-2-54mm) |
| 26 | Screw terminal 2-pin, 5.08 mm (power in/out) | 4 | 15 | 60 | K-Technics | in stock | [link](https://www.ktechnics.com/products/2-pin-screw-terminal-block-connector-5-08mm-pitch-301-2p) |
| 27 | JST 3-pin cable, male + female (sensors, servo extension) | 4 | 50 | 200 | ArduinoTech | in stock | [link](https://arduinotech.co.ke/product/jst-3-pin-cable-with-male-female-connector-pre-wired-3-pin-jst-cable-for-arduino-and-diy-electronics) |
| 28 | Heat-shrink kit (328 pcs) | 1 | 550 | 550 | K-Technics | in stock | [link](https://www.ktechnics.com/products/heat-shrink-tube-mixed-color-kit-328pcs) |
| 29 | Hook-up wire, 22 AWG (power) + 26 AWG (signal), ~2 m each | 1 set | `[QUOTE]` | `[QUOTE]` | Luthuli Ave stalls | — | Not listed online at either CBD shop |
| | **Subtotal** | | | **950** + wire | | | |

---

## Total

| Section | KES |
|---------|----:|
| 1. Main modules | 10,260 |
| 2. Power | 1,037 |
| 3. Support passives | 33 |
| 4. Board and wiring | 950 |
| **Electronics total (excl. wire)** | **≈ 12,280** |
| With bare IR pair instead of E18-D80NK | ≈ 11,700 |
| +15% contingency | ≈ 14,120 |

**Shopping plan:** one CBD trip to K-Technics (Munyu Rd) and ArduinoTech (Mfangano St) covers everything except the ESP32 board, the VL6180X and the fuse holder. Order those three from Pixel Electric for delivery, or pick up the fuse holder on Luthuli Ave.

**Cost driver:** the 4G modem is now 45% of the electronics. It replaces a part that likely wouldn't have worked with the payment server, so it's money well spent, but it's the first place to look when costing a production run.

---

## Not in this BOM
- Micro-USB cable for programming. This is a tool, not a product part.
- Safaricom SIM card, cam lock, screws: see [bom_hardware.md](bom_hardware.md) section 2d.
