# Smart Pad Dispenser — Print Settings Guide

> Fill in `[CONFIRM]` fields after measuring/testing on your specific printer.

---

## Global Defaults (Apply Unless Overridden Per-Part)

| Setting | Value | Notes |
|---------|-------|-------|
| Slicer | PrusaSlicer / Cura | Your choice |
| Filament | PLA (structural), PETG (mechanism) | |
| Nozzle temperature | PLA: 210 °C / PETG: 235 °C | `[CONFIRM]` for your filament brand |
| Bed temperature | PLA: 60 °C / PETG: 70 °C | `[CONFIRM]` |
| Print speed | 50 mm/s | Reduce to 30 mm/s for first layer |
| First layer speed | 25 mm/s | Adhesion critical |
| Layer height | 0.2 mm | General quality |
| Perimeters / walls | 3 | = 1.2 mm wall at 0.4 mm nozzle |
| Top/bottom layers | 4 | Solid surface |
| Infill pattern | Gyroid or Honeycomb | Isotropic strength |
| Supports | As noted per part | Avoid where possible |
| Cooling | 100% fan after layer 2 | Reduce to 50% for PETG |

---

## Per-Part Settings

### `body_main.stl` — Enclosure Main Body

| Setting | Value |
|---------|-------|
| Filament | PLA |
| Infill % | 25% |
| Supports | Yes — for electronics bay overhangs |
| Orientation | Print upright (Z = height) |
| Layer height | 0.2 mm |
| Perimeters | 4 (extra rigidity) |
| Est. time | `[SLICER OUTPUT]` |
| Est. filament | `[SLICER OUTPUT]` g |
| Notes | Largest print — ensure bed is level, use brim |

---

### `lid_top.stl` — Top Loading Lid

| Setting | Value |
|---------|-------|
| Filament | PLA |
| Infill % | 20% |
| Supports | None |
| Orientation | Print flat (face down) |
| Layer height | 0.2 mm |
| Perimeters | 3 |
| Est. time | `[SLICER OUTPUT]` |
| Est. filament | `[SLICER OUTPUT]` g |
| Notes | Ensure hinge/snap features face up for clean surface |

---

### `panel_front.stl` — Front Face with Dispense Slot

| Setting | Value |
|---------|-------|
| Filament | PLA |
| Infill % | 30% |
| Supports | Yes — dispense slot overhang if angled |
| Orientation | Face outward (Z = depth) |
| Layer height | 0.15 mm | Fine detail for slot edges |
| Perimeters | 4 |
| Est. time | `[SLICER OUTPUT]` |
| Est. filament | `[SLICER OUTPUT]` g |
| Notes | Smooth external surface — consider ironing top layer |

---

### `panel_back.stl` — Back Panel

| Setting | Value |
|---------|-------|
| Filament | PLA |
| Infill % | 20% |
| Supports | None |
| Orientation | Print flat |
| Layer height | 0.2 mm |
| Est. time | `[SLICER OUTPUT]` |
| Est. filament | `[SLICER OUTPUT]` g |

---

### `motor_bracket.stl` — Motor Mount

| Setting | Value |
|---------|-------|
| Filament | PETG (vibration resistance) |
| Infill % | 40% |
| Supports | Yes — motor pocket overhang |
| Orientation | `[DECIDE based on stress direction]` |
| Layer height | 0.2 mm |
| Perimeters | 4 |
| Est. time | `[SLICER OUTPUT]` |
| Est. filament | `[SLICER OUTPUT]` g |
| Notes | Print with load-bearing axis perpendicular to layer lines |

---

### `paddle_wheel.stl` — Dispensing Paddle Wheel

| Setting | Value |
|---------|-------|
| Filament | **PETG** (must flex slightly without cracking) |
| Infill % | 50% |
| Supports | Minimal — design paddles to be self-supporting |
| Orientation | Shaft axis vertical (Z-axis) |
| Layer height | 0.15 mm |
| Perimeters | 4 |
| Est. time | `[SLICER OUTPUT]` |
| Est. filament | `[SLICER OUTPUT]` g |
| Notes | Most critical moving part — inspect for delamination |

---

### `mechanism_housing.stl` — Mechanism Enclosure

| Setting | Value |
|---------|-------|
| Filament | PLA |
| Infill % | 25% |
| Supports | Yes |
| Layer height | 0.2 mm |
| Perimeters | 3 |
| Est. time | `[SLICER OUTPUT]` |
| Est. filament | `[SLICER OUTPUT]` g |

---

### `chute_dispense.stl` — Pad Exit Chute

| Setting | Value |
|---------|-------|
| Filament | PLA |
| Infill % | 20% |
| Supports | None (design chute walls ≤ 45° overhang) |
| Orientation | Open side down |
| Layer height | 0.2 mm |
| Perimeters | 3 |
| Est. time | `[SLICER OUTPUT]` |
| Est. filament | `[SLICER OUTPUT]` g |
| Notes | Inner surface finish matters — sand smooth if pad snags |

---

### `mount_ir_beam.stl` — IR Break-Beam Mounts (×2)

| Setting | Value |
|---------|-------|
| Filament | PLA |
| Infill % | 40% |
| Supports | None |
| Layer height | 0.15 mm |
| Perimeters | 4 |
| Est. time | `[SLICER OUTPUT]` |
| Est. filament | `[SLICER OUTPUT]` g |
| Notes | Print both halves at once; check snap-fit clearance on first print |

---

### `mount_ultrasonic.stl` — HC-SR04 Mount

| Setting | Value |
|---------|-------|
| Filament | PLA |
| Infill % | 30% |
| Supports | None |
| Layer height | 0.2 mm |
| Perimeters | 3 |
| Est. time | `[SLICER OUTPUT]` |
| Est. filament | `[SLICER OUTPUT]` g |

---

### `tray_esp32.stl` — ESP32 Electronics Tray

| Setting | Value |
|---------|-------|
| Filament | PLA |
| Infill % | 20% |
| Supports | None |
| Orientation | Tray open side up |
| Layer height | 0.2 mm |
| Perimeters | 3 |
| Est. time | `[SLICER OUTPUT]` |
| Est. filament | `[SLICER OUTPUT]` g |
| Notes | Test fit with actual ESP32 board before printing body |

---

## Post-Processing Notes

| Process | When | Tool |
|---------|------|------|
| Remove supports | After print | Needle-nose pliers |
| Sand dispense chute inner walls | After `chute_dispense.stl` | 220-grit then 400-grit |
| Test snap-fit before assembly | All snap parts | Hand-fit dry |
| Heat-set inserts | Before assembly | Soldering iron at 200 °C |
| Thread tapping (M3) | If no inserts used | M3 tap |
