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
| Ultrasonic mount | `mount_ultrasonic.stl` | `[CALC]` | `[CALC]` | `[CALC]` | |
| ESP32 tray | `tray_esp32.stl` | `[CALC]` | `[CALC]` | `[CALC]` | |
| OLED display bezel | `bezel_oled.stl` | `[CALC]` | `[CALC]` | `[CALC]` | Front panel frame for 1.3" OLED |
| Keypad mount | `mount_keypad.stl` | `[CALC]` | `[CALC]` | `[CALC]` | 4×4 matrix keypad recess |
| GSM module tray | `tray_gsm.stl` | `[CALC]` | `[CALC]` | `[CALC]` | SIM800L + antenna routing |
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

| Component | Qty | Unit Price (KES) | Total (KES) | Supplier | Notes |
|-----------|-----|-----------------|-------------|----------|-------|
| ESP32 dev board (38-pin) | 1 | `[QUOTE]` | `[CALC]` | `[LOCAL]` | e.g. ESP32-WROOM-32 |
| HC-SR04 ultrasonic sensor | 1 | `[QUOTE]` | `[CALC]` | `[LOCAL]` | |
| IR break-beam pair (3–5V) | 1 pair | `[QUOTE]` | `[CALC]` | `[LOCAL]` | TX + RX |
| DC geared motor (5–12V) | 1 | `[QUOTE]` | `[CALC]` | `[LOCAL]` | OR stepper motor |
| Motor driver (L298N / DRV8833) | 1 | `[QUOTE]` | `[CALC]` | `[LOCAL]` | Match motor choice |
| 5V regulated power supply | 1 | `[QUOTE]` | `[CALC]` | `[LOCAL]` | Min 2A |
| LED indicators (Red/Green) | 2 | `[QUOTE]` | `[CALC]` | `[LOCAL]` | Status lights |
| 220 Ω resistors (for LEDs) | 2 | `[QUOTE]` | `[CALC]` | `[LOCAL]` | |
| Jumper wires (M–F 20 cm) | 1 pack | `[QUOTE]` | `[CALC]` | `[LOCAL]` | |
| Micro USB cable | 1 | `[QUOTE]` | `[CALC]` | `[LOCAL]` | Programming + power |
| **Electronics Subtotal** | | | KES `[CALC]` | | |

---

## 2b. M-PESA & UI Hardware

| Component | Qty | Unit Price (KES) | Total (KES) | Supplier | Notes |
|-----------|-----|-----------------|-------------|----------|-------|
| 1.3" OLED display (SSD1306, I2C) | 1 | `[QUOTE]` | `[CALC]` | Nerokas / Ktronics | 128×64, 3.3V |
| 4×4 matrix keypad | 1 | `[QUOTE]` | `[CALC]` | Nerokas | Phone number entry for STK Push |
| SIM800L GSM module | 1 | `[QUOTE]` | `[CALC]` | RoboticsKe | WiFi fallback / primary connectivity |
| GSM antenna (external, SMA) | 1 | `[QUOTE]` | `[CALC]` | RoboticsKe | For SIM800L |
| Safaricom SIM card | 1 | `[QUOTE]` | `[CALC]` | Safaricom shop | Data plan for M-PESA callbacks |
| Key lock cylinder (cam lock, 16 mm) | 1 | `[QUOTE]` | `[CALC]` | Hardware shop | Admin access to pad stack |
| Passive buzzer (3.3V) | 1 | `[QUOTE]` | `[CALC]` | Nerokas | Audio feedback |
| 5V 2A mains adapter (micro USB / barrel) | 1 | `[QUOTE]` | `[CALC]` | Local shop | Wall-powered; GSM peak draw ~2A |
| Wall mounting screws + rawl plugs | 1 set | `[QUOTE]` | `[CALC]` | Hardware shop | Concrete/tile anchors |
| **M-PESA & UI Subtotal** | | | KES `[CALC]` | | |

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
| Safaricom data plan (SIM800L) | KES `[QUOTE]` | Monthly | ~50 MB/month for callbacks |
| Safaricom Daraja API | KES 0 | One-time setup | Free; requires business registration |
| Domain name (for HTTPS callback URL) | KES `[QUOTE]` | Yearly | Or use free subdomain from host |
| **Monthly Running Cost** | KES `[CALC]` | Monthly | |

---

## 6. Total Budget Summary

| Category | Estimated Cost (KES) |
|----------|---------------------|
| 3D Filament | `[CALC]` |
| Electronic Components | `[CALC]` |
| M-PESA & UI Hardware | `[CALC]` |
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
