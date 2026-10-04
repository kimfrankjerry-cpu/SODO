# Smart Pad Dispenser — Hardware Bill of Materials (BOM)

> **Phase**: 3D Design & Simulation
> **Currency**: KES (Kenyan Shillings) — update with local quotes

---

## 1. 3D Printed Parts

| Part | File | Est. Volume (cm³) | Est. Filament (g) | Est. Print Time | Notes |
|------|------|--------------------|-------------------|-----------------|----|
| Main body | `body_main.stl` | `[CALC]` | `[CALC]` | `[CALC]` | Largest part |
| Lid / top cover | `lid_top.stl` | `[CALC]` | `[CALC]` | `[CALC]` | |
| Front panel | `panel_front.stl` | `[CALC]` | `[CALC]` | `[CALC]` | Dispense slot |
| Back panel | `panel_back.stl` | `[CALC]` | `[CALC]` | `[CALC]` | |
| Motor mount | `motor_bracket.stl` | `[CALC]` | `[CALC]` | `[CALC]` | |
| Paddle wheel | `paddle_wheel.stl` | `[CALC]` | `[CALC]` | `[CALC]` | PETG recommended |
| Mechanism housing | `mechanism_housing.stl` | `[CALC]` | `[CALC]` | `[CALC]` | |
| Dispense chute | `chute_dispense.stl` | `[CALC]` | `[CALC]` | `[CALC]` | |
| IR beam mount (×2) | `mount_ir_beam.stl` | `[CALC]` | `[CALC]` | `[CALC]` | TX and RX |
| ToF sensor mount (VL6180X) | `mount_tof.stl` | `[CALC]` | `[CALC]` | `[CALC]` | Above the stack, looking down |
| ESP32 tray | `tray_esp32.stl` | `[CALC]` | `[CALC]` | `[CALC]` | |
| OLED display bezel | `bezel_oled.stl` | `[CALC]` | `[CALC]` | `[CALC]` | Front panel frame for 1.3" OLED |
| Keypad mount | `mount_keypad.stl` | `[CALC]` | `[CALC]` | `[CALC]` | 4×4 matrix keypad recess |
| 4G module tray | `tray_4g.stl` | `[CALC]` | `[CALC]` | `[CALC]` | A7672E + LTE antenna routing |
| Key lock collar | `collar_keylock.stl` | `[CALC]` | `[CALC]` | `[CALC]` | Admin lid key lock housing |
| Buzzer port plug | `plug_buzzer.stl` | `[CALC]` | `[CALC]` | `[CALC]` | Humidity-blocking grille |
| Wall mount bracket | `bracket_wall.stl` | `[CALC]` | `[CALC]` | `[CALC]` | Keyhole wall-mount plate |
| **TOTAL** | | `[CALC]` | `[CALC]` | `[CALC]` | |

### Filament Estimate

| Parameter | Value |
|-----------|-------|
| Filament spool size | 1 kg |
| Price per spool (PLA) | KES `[QUOTE]` |
| Price per spool (PETG) | KES `[QUOTE]` |
| Estimated PLA usage | `[CALC]` g |
| Estimated PETG usage | `[CALC]` g |
| Estimated filament cost | KES `[CALC]` |

---

## 2. Electronic Components

> Matches the rev B wiring diagram. Prices, shops and links are in [bom_electronics_pricing.md](bom_electronics_pricing.md) (checked 4 Oct 2026).
> Motor is a servo, so **no L298N/DRV8833 and no DC geared motor**.

### 2a. Core modules

| Component | Qty | Unit Price (KES) | Total (KES) | Supplier | Notes |
|-----------|-----|-----------------|-------------|----------|-------|
| ESP32 DevKit, **ESP-WROOM-32**, 30-pin | 1 | 1,400 | 1,400 | Pixel Electric | Not WROVER: GPIO16/17 are the modem UART. 38-pin also fits if back in stock |
| A7672E 4G LTE Cat-1 board + LTE antenna | 1 | 5,500 | 5,500 | ArduinoTech | Confirm 5 V input and 3.3 V logic at purchase. PWRKEY → GPIO23 |
| MG946R servo (MG996R size) | 1 | 950 | 950 | ArduinoTech | 4.8–7.2 V, up to 2.5 A at stall |
| 0.96" OLED 128×64 I2C, **SSD1306** | 1 | 550 | 550 | ArduinoTech | I2C 0x3C. A 1.3" module is usually SH1106 and needs a library change |
| 4×4 membrane keypad | 1 | 120 | 120 | K-Technics | |
| VL6180X time-of-flight sensor (MCU-6180 board) | 1 | 1,100 | 1,100 | Pixel Electric | I2C 0x29, shares the OLED bus. Reads 5–100 mm |
| E18-D80NK IR proximity switch (pad sensor) | 1 | 600 | 600 | K-Technics | 5 V supply, NPN output with 10 kΩ pull-up to 3V3 → GPIO19 |
| **Active** buzzer 5 V | 1 | 25 | 25 | K-Technics | Driven through Q1 |
| 5 mm LEDs (red, amber, green) | 3 | 5 | 15 | K-Technics | |

### 2b. Power

| Component | Qty | Unit Price (KES) | Total (KES) | Supplier | Notes |
|-----------|-----|-----------------|-------------|----------|-------|
| 12 V 2 A adapter, barrel plug | 1 | 500 | 500 | K-Technics | No local 5 V adapter above 2 A |
| DC barrel jack, panel mount 5.5×2.1 mm | 1 | 20 | 20 | K-Technics | |
| XL4005 5 A buck module, set to 5.1 V | 1 | 300 | 300 | K-Technics | Makes the 5 V rail (~4.3 A available) |
| SS34 Schottky diode | 1 | 5 | 5 | K-Technics | Reverse polarity, 12 V side |
| 3 A fast-blow glass fuse 6×30 mm + inline holder | 2 + 1 | 5 / 120 | 130 | ArduinoTech / Pixel | 12 V side, one spare fuse |
| Rocker switch | 1 | 50 | 50 | K-Technics | |
| 1000 µF 16 V electrolytic | 2 | 10 | 20 | ArduinoTech | C1 at modem VIN, C2 at servo V+ |
| 100 nF ceramic capacitors | 4 | 3 | 12 | K-Technics | Decoupling |

### 2c. Support passives & interconnect

| Component | Qty | Unit Price (KES) | Total (KES) | Supplier | Notes |
|-----------|-----|-----------------|-------------|----------|-------|
| 10 kΩ resistor | 5 | 2 | 10 | K-Technics | 4× keypad row pull-ups (GPIO34/35/36/39), 1× IR output pull-up |
| 220 Ω resistor | 3 | 2 | 6 | K-Technics | LEDs |
| S8050 / 2N2222 NPN + 1 kΩ base resistor | 1 + 1 | 10 / 2 | 12 | ArduinoTech / K-Technics | Buzzer low-side driver |
| 1N4007 diode | 1 | 5 | 5 | ArduinoTech | Buzzer flyback |
| Prototype PCB 7×9 cm, female headers ×2 | 1 set | — | 140 | ArduinoTech / K-Technics | |
| 2-pin screw terminals ×4, JST 3-pin cables ×4 | 1 set | — | 260 | K-Technics / ArduinoTech | Off-board modules |
| Heat-shrink kit; 22 AWG + 26 AWG wire | 1 set | 550 + `[QUOTE]` | `[CALC]` | K-Technics / Luthuli Ave | |
| **Electronics Subtotal** | | | **≈ KES 12,280** + wire | | |

> Not in the product BOM: micro-USB cable and jumper wires (tools), Safaricom SIM, cam lock, wall screws (see 2d).

---

## 2d. Non-electronic Items

| Component | Qty | Unit Price (KES) | Total (KES) | Supplier | Notes |
|-----------|-----|-----------------|-------------|----------|-------|
| Safaricom SIM card | 1 | `[QUOTE]` | `[CALC]` | Safaricom shop | Data plan for M-PESA callbacks |
| Key lock cylinder (cam lock, 16 mm) | 1 | `[QUOTE]` | `[CALC]` | Hardware shop | Admin access to pad stack |
| Wall mounting screws + rawl plugs | 1 set | `[QUOTE]` | `[CALC]` | Hardware shop | Concrete/tile anchors |

---

## 3. Fasteners & Assembly Hardware

| Item | Qty | Unit Price (KES) | Total (KES) | Notes |
|------|-----|-----------------|-------------|-------|
| M3 × 10 mm screws | 20 | `[QUOTE]` | `[CALC]` | General assembly |
| M3 × 20 mm screws | 10 | `[QUOTE]` | `[CALC]` | Motor mount |
| M3 hex nuts | 20 | `[QUOTE]` | `[CALC]` | |
| M3 heat-set inserts | 10 | `[QUOTE]` | `[CALC]` | Optional — stronger threads |
| Small hinges (for lid) | 2 | `[QUOTE]` | `[CALC]` | If lid is hinged not snap |
| **Fasteners Subtotal** | | | KES `[CALC]` | |

---

## 4. Printing Services (If Not Printing In-House)

| Service | Qty | Price (KES) | Notes |
|---------|-----|-------------|-------|
| Local print shop — small parts | `[CALC]` g | `[QUOTE]` | Mounts, brackets |
| Local print shop — large parts | `[CALC]` g | `[QUOTE]` | Body, panels |
| **Print Service Subtotal** | | KES `[CALC]` | |

---

## 5. Backend & Connectivity Costs (Recurring)

| Item | Cost | Frequency | Notes |
|------|------|-----------|-------|
| Cloud server hosting (Railway / Render) | KES 0–500 | Monthly | Free tier sufficient for low traffic |
| Safaricom data plan (A7672E, 4G) | KES `[QUOTE]` | Monthly | ~50 MB/month for callbacks |
| Safaricom Daraja API | KES 0 | One-time setup | Free; requires business registration |
| Domain name (for HTTPS callback URL) | KES `[QUOTE]` | Yearly | Or use free subdomain from host |
| **Monthly Running Cost** | KES `[CALC]` | Monthly | |

---

## 6. Total Budget Summary

| Category | Estimated Cost (KES) |
|----------|---------------------|
| 3D Filament | `[CALC]` |
| Electronic Components | `[CALC]` |
| Non-electronic items | `[CALC]` |
| Fasteners | `[CALC]` |
| Print Services (if applicable) | `[CALC]` |
| Backend Setup (one-time) | `[CALC]` |
| **GRAND TOTAL (one-time)** | KES `[CALC]` |
| Contingency (+15%) | KES `[CALC]` |
| **BUDGET WITH CONTINGENCY** | KES `[CALC]` |
| Monthly running cost | KES `[CALC]` |

---

## 6. Local Suppliers Reference (Kenya)

| Supplier | Location | What They Stock |
|----------|----------|-----------------|
| Nerokas | Nairobi CBD / Online | ESP32, sensors, modules |
| Ktronics | Nairobi | Electronic components |
| RoboticsKe | Nairobi | Motors, sensors, dev boards |
| Ultimaker/local print shops | Various | 3D printing services |
| Hardware shops (Kirinyaga Rd) | Nairobi CBD | Fasteners, cables |

> **Note**: Always get quotes before finalising budget. Prices fluctuate.
