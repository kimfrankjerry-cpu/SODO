# Smart Pad Dispenser — 3D Design Specification

> **Status**: DRAFT — fill in `[MEASURE]` fields before modelling begins.

---

## 1. Reference Dimensions (Real-World Measurements)

### 1.1 Sanitary Pad (Single Unit)

| Dimension | Value | Notes |
|-----------|-------|-------|
| Length | `[MEASURE]` mm | Longest axis |
| Width | `[MEASURE]` mm | Shorter axis |
| Compressed thickness | `[MEASURE]` mm | When in packaging |
| Expanded thickness | `[MEASURE]` mm | After handling |
| Wing span (if any) | `[MEASURE]` mm | Folded-in dimension |

### 1.2 Pad Stack (10-pack)

| Dimension | Value | Notes |
|-----------|-------|-------|
| Stack height (10 pads) | `[MEASURE]` mm | Full load |
| Stack height (1 pad) | `[MEASURE]` mm | Minimum inventory |
| Stack footprint | `[MEASURE]` × `[MEASURE]` mm | L × W |

### 1.3 Electronics & Sensors

> ✅ Dimensions below sourced from published datasheets. Add ±0.5 mm clearance for clone boards.
> ⚠️ Physically verify your specific board with calipers before CAD — clone dimensions vary.

| Component | L (mm) | W (mm) | H (mm) | Mounting / Notes |
|-----------|--------|--------|--------|------------------|
| ESP32 dev board (38-pin WROOM-32) | 55 | 28 | 12 | Pin row pitch 22.86 mm; verify with calipers |
| VL6180X ToF board (MCU-6180) | `[MEASURE]` | `[MEASURE]` | `[MEASURE]` | Small I2C board; measure the one you buy |
| 1.3" OLED display (SSD1306/SH1106) | 35.6 | 33.0 | 3.5 | Active area 29.4 × 14.7 mm; I2C 4-pin |
| 4×4 matrix keypad | 69 | 77 | 1 | Membrane; adhesive or snap-fit retention |
| A7672E 4G board | `[MEASURE]` | `[MEASURE]` | `[MEASURE]` | Board size varies by vendor; leave room for SIM slot and LTE antenna lead |
| Motor — see Section 1.5 below | — | — | — | Decision required |
| XL4005 buck module (replaces L298N — servo needs no driver) | 51 | 26 | 14 | 2× M3 holes; heatsink on top; verify with calipers |
| E18-D80NK IR proximity switch | Ø18 × 45 | — | — | M18 threaded barrel with two nuts |

### 1.4 Local Printer Constraints

| Parameter | Value |
|-----------|-------|
| Printer model | `[CONFIRM]` |
| Bed size (X × Y) | `[CONFIRM]` mm |
| Max Z height | `[CONFIRM]` mm |
| Nozzle diameter | `[CONFIRM]` mm |
| Filament type | `[CONFIRM]` |
| Filament colour | `[CONFIRM]` |

### 1.5 Motor Selection & Torque Analysis

> **Why this matters**: The paddle wheel must overcome the friction and weight of the pad stack.
> Estimated load = 10 pads × ~7 g = 70 g. Paddle arm ≈ 20 mm. Required torque ≈ **70 × 20 = 1,400 g·cm = 1.4 kg·cm**.
> Add 2× safety factor → target **≥ 2.8 kg·cm** at operating speed.

| Motor | Stall Torque | Dimensions (mm) | Driver | Verdict |
|-------|-------------|-----------------|--------|--------|
| 28BYJ-48 stepper (5V) | 300–450 g·cm | Ø28 × 19 mm, 5 mm shaft | ULN2003 | ❌ Below target — too weak under full stack |
| **MG996R servo (recommended)** | **9.4 kg·cm** @ 4.8V | **40.7 × 19.7 × 42.9 mm** | Direct PWM from ESP32 | ✅ 3.4× margin over target |
| MG995 servo | 8.5 kg·cm | 40.7 × 19.7 × 42.9 mm | Direct PWM | ✅ Acceptable alternative |
| NEMA 17 stepper | 40+ kg·cm | 42 × 42 × 40 mm | A4988 | ⚠️ Overkill + bulky; use only if others fail |

**Selected Motor**: **MG996R servo** ✅
- Stall torque: **9.4 kg·cm** (4.8V) — 6.7× margin over required 1.4 kg·cm
- Dimensions: **40.7 × 19.7 × 42.9 mm**, shaft 5 mm
- Control: single PWM signal from ESP32 (no separate driver board needed)
- Indexed 90° rotation: set via PWM pulse width — no calibration loop needed
- Power: use external 5V 2A supply directly (do NOT power from ESP32 5V pin)
- Available from: RoboticsKe, Nerokas (est. KES 600–900)

> ⚠️ The servo draws up to **2.5 A stall current**. Power comes from a 12 V 2 A adapter through an XL4005 buck (≈4.3 A at 5 V),
> which covers the servo plus the 4G modem's peaks. Local stock: MG946R (same body size) at ArduinoTech.

---

## 2. Enclosure Specification

### 2.1 Overall Enclosure

| Dimension | Value | Derivation |
|-----------|-------|------------|
| Outer Width | `[CALC]` mm | Pad width + 2 × wall + clearance |
| Outer Depth | `[CALC]` mm | Pad length + mechanism depth + electronics bay |
| Outer Height | `[CALC]` mm | Stack height (10) + mechanism height + base |
| Wall thickness | 2.4 mm | 6 × 0.4 mm nozzle |
| Base thickness | 3.0 mm | Structural floor |
| Lid thickness | 2.0 mm | Top-loading cover |

### 2.2 Stack Chamber

| Dimension | Value | Clearance |
|-----------|-------|-----------|
| Internal width | pad width + `[X]` mm | Pad must slide freely, not rattle |
| Internal depth | pad length + `[X]` mm | Sliding clearance |
| Height | full stack + `[X]` mm | Top-load room |
| Taper angle | 1–2° | Guides pads downward |

### 2.3 Electronics Bay

| Component | Min Bay Size | Notes |
|-----------|-------------|-------|
| ESP32 tray | `[CALC]` mm | 5 mm clearance each side |
| Motor driver | `[CALC]` mm | Ventilation gap on top |
| Power supply | `[CALC]` mm | Accessible connector side |
| Wire harness | 20 mm channel | Minimum routing channel width |

### 2.4 Dispense Slot (Front Face)

| Dimension | Value | Notes |
|-----------|-------|-------|
| Slot width | pad width + 10 mm | Pad exits without catching |
| Slot height | pad thickness × 1.5 | Single pad only, prevents double-dispense |
| Slot depth (chute length) | `[CALC]` mm | Must cover IR beam path |
| Chamfer on slot edges | 3 mm × 45° | Smooth exit, no snag |

---

## 3. Dispensing Mechanism Specification

> Mechanism type: **Paddle Wheel** (subject to [`mechanism_decision.md`](./mechanism_decision.md))

### 3.1 Paddle Wheel

| Parameter | Value | Notes |
|-----------|-------|-------|
| Wheel diameter | `[CALC]` mm | Must reach one pad depth into stack |
| Paddle count | 4 | 90° apart for single-pad indexing |
| Paddle width | pad width − 5 mm | Narrower than pad to avoid catching wings |
| Paddle depth | compressed thickness + 2 mm | Scoops exactly one pad |
| Shaft diameter | motor shaft `[MEASURE]` mm | Press-fit or keyed |
| Hub-to-wall clearance | 0.5 mm | Running clearance |
| Material recommendation | PETG | More flexible than PLA under load |

### 3.2 Mechanism Housing

| Parameter | Value | Notes |
|-----------|-------|-------|
| Clearance around wheel | 1.5 mm radial | Allows free rotation |
| Entry aperture height | compressed pad thickness + 3 mm | Lets one pad enter per stroke |
| Exit aperture | matches dispense chute | Aligned with front slot |

### 3.3 Motor Mount Bracket

| Parameter | Value | Notes |
|-----------|-------|-------|
| Motor body fit | motor OD + 0.3 mm | Snug but removable |
| Alignment tolerance | ± 0.5 mm | Shaft must align with wheel bore |
| Mounting screws | M3 × `[CALC]` mm | Into enclosure body |
| Retention method | 2 × M3 screws | Locking not snap-fit (motor vibrates) |

---

## 4. Sensor Mount Specifications

### 4.1 VL6180X Time-of-Flight Mount

| Parameter | Value | Notes |
|-----------|-------|-------|
| Sensor pocket depth | board thickness + 1 mm | Retains board; window must stay clear |
| Sensor pocket clearance | +0.4 mm each side | Print tolerance |
| Pointing angle | 90° (straight down) | Perpendicular to pad stack top |
| Distance to stack | ≥ 5 mm when full, ≤ 100 mm when empty | VL6180X reliable range is 5–100 mm |
| Cable exit direction | Rear | Exits to electronics bay |

### 4.2 IR Pad Sensor Mount (E18-D80NK)

| Parameter | Value | Notes |
|-----------|-------|-------|
| Mounting hole | Ø18.5 mm | M18 barrel, clamped by its two nuts |
| Aim | across the dispense slot, at the pad | Reflective sensor, so no receiver on the far side |
| Far wall | matte, ≥ 80 mm away or angled | Stops the sensor seeing the wall as a pad |
| Range setting | set with its rear screw | Adjust so an empty slot reads "no object" |

---

## 5. Tolerances & Clearances Summary

| Fit Type | Clearance | Application |
|----------|-----------|-------------|
| Snap-fit (latching) | 0.25 mm per side | Sensor mounts, lid clips |
| Sliding fit | 0.3 mm per side | Pad channel walls |
| Press-fit | −0.1 mm per side | Motor shaft hub |
| Running clearance | 0.5 mm | Paddle wheel vs housing |
| Screw holes (M3 self-tap) | 2.7 mm Ø | All screw holes |
| Screw holes (M3 clearance) | 3.4 mm Ø | Through-holes |

---

## 6. Version History

| Version | Date | Changes |
|---------|------|---------|
| v0.1 | `[DATE]` | Initial draft — measurements TBD |
