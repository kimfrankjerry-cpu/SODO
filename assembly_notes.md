# Smart Pad Dispenser — Assembly Notes

> **Status**: STUB — to be completed after Phase 5 sign-off.
> Do NOT begin physical assembly until all Phase 5 sign-off criteria are met.

---

## Pre-Assembly Checklist

| Check | Status |
|-------|--------|
| All STLs printed and inspected for delamination | `[ ]` |
| Supports removed from all parts | `[ ]` |
| Snap-fit clearances verified by hand before main assembly | `[ ]` |
| Heat-set inserts installed (if used) | `[ ]` |
| Dispense chute inner walls sanded smooth (220 → 400 grit) | `[ ]` |
| All parts labelled and identified | `[ ]` |

---

## Assembly Order

> Fill in step-by-step instructions here after completing prototype prints.

### Step 1 — Electronics Bay Sub-Assembly
- [ ] Install ESP32 tray (`tray_esp32.stl`) into main body
- [ ] Seat motor driver board
- [ ] Route wire harness through channel

### Step 2 — Mechanism Sub-Assembly
- [ ] Mount motor to bracket (`motor_bracket.stl`)
- [ ] Press paddle wheel (`paddle_wheel.stl`) onto motor shaft
- [ ] Seat mechanism housing (`mechanism_housing.stl`) over paddle wheel
- [ ] Attach dispense chute (`chute_dispense.stl`)

### Step 3 — Sensor Installation
- [ ] Snap IR break-beam TX and RX into mounts (`mount_ir_beam.stl`)
- [ ] Seat HC-SR04 into ultrasonic mount (`mount_ultrasonic.stl`)
- [ ] Connect all sensor wires to ESP32

### Step 4 — Enclosure Closure
- [ ] Attach front panel (`panel_front.stl`) — align dispense slot with chute exit
- [ ] Attach back panel (`panel_back.stl`)
- [ ] Test top lid (`lid_top.stl`) opens and closes without tools

### Step 5 — Dry-Run Test
- [ ] Load pads (Brand 1 from Phase 0)
- [ ] Manually rotate paddle wheel — confirm single-pad separation
- [ ] Load pads (Brand 2 from Phase 0)
- [ ] Confirm dispense with power applied

---

## Torque / Force Notes

> Record actual stack weight and motor torque measurements here during assembly.

| Parameter | Measured Value | Notes |
|-----------|---------------|-------|
| 10-pad stack weight | `[MEASURE]` g | Full load |
| Motor rated torque | `[MEASURE]` kg·cm | From datasheet |
| Paddle arm length | `[MEASURE]` mm | Centre of shaft to paddle tip |
| Required torque estimate | `[CALC]` kg·cm | Stack weight × arm length |
| Margin | `[CALC]` % | (Rated − Required) / Rated |

---

## Known Issues

> Log any assembly problems here for changelog_3d.md reference.

| Issue | Part | Status |
|-------|------|--------|
| _None yet_ | — | — |

---

## Version History

| Version | Date | Notes |
|---------|------|-------|
| v0.1 | `[DATE]` | Stub created — no assembly steps yet |
