# Smart Pad Dispenser — 3D Design & Simulation Development Plan

## Project Context

An ESP32-based **pay-to-dispense** sanitary pad vending machine for washroom deployment.
Users pay via **M-PESA STK Push** — the device sends a PIN prompt to their phone; on confirmation,
one pad is dispensed. The device is wall-mounted in a public washroom and must be tamper-resistant,
humidity-resistant, and connected to both GSM and WiFi for payment processing.

Before electronics and firmware, the **physical housing and mechanical dispensing mechanism**
must be designed, simulated, and confirmed printable on local FDM printers.

---

## Phase 0 — Pre-Design Research (Current Phase)

**Goal**: Nail down dimensions and constraints before modelling anything.

| Task | Owner | Target Date | Status |
|------|-------|-------------|--------|
| Measure a standard sanitary pad (length × width × compressed thickness) | You | `[DATE]` | `[ ]` |
| Measure a **second pad brand** for worst-case dimensions | You | `[DATE]` | `[ ]` |
| Measure a typical pad stack height (e.g., 10-pack) | You | `[DATE]` | `[ ]` |
| Confirm motor type available **AND its rated torque (kg·cm)** | You | `[DATE]` | `[ ]` |
| Confirm local printer bed size and nozzle diameter | You | `[DATE]` | `[ ]` |
| Confirm filament type (PLA / PETG / ABS) | You | `[DATE]` | `[ ]` |
| Confirm IR break-beam module physical dimensions | You | `[DATE]` | `[ ]` |
| Confirm HC-SR04 module physical dimensions | You | `[DATE]` | `[ ]` |
| Confirm ESP32 dev-board physical dimensions | You | `[DATE]` | `[ ]` |
| **Select CAD tool** (Fusion 360 / FreeCAD / OpenSCAD) and install it | You | `[DATE]` | `[ ]` |
| **Initialise Git repository** and document its root path | You | `[DATE]` | `[ ]` |
| Confirm price per pad (KES) | You | 2026-09-15 | `[x]` **KES 20** (configurable in `firmware_spec.md`) |
| Confirm M-PESA Paybill or Till Number (register with Safaricom if needed) | You | `[DATE]` | `[ ]` |
| Confirm Safaricom Daraja API credentials (Consumer Key + Secret) | You | `[DATE]` | `[ ]` |
| Confirm washroom wall type (tiles / concrete / drywall) for mount hardware | You | `[DATE]` | `[ ]` |
| Confirm 4×4 keypad module physical dimensions | You | `[DATE]` | `[ ]` |
| Confirm 1.3" OLED display physical dimensions | You | `[DATE]` | `[ ]` |
| Confirm SIM800L / GSM module physical dimensions | You | `[DATE]` | `[ ]` |
| Confirm SIM card + data plan available | You | `[DATE]` | `[ ]` |

### Phase 0 Exit Criteria

> 🚫 **DO NOT advance to Phase 1 until ALL rows above are `[x]`.**
> This is a hard gate — partial measurements produce an invalid CAD model.

| Exit Check | Status |
|------------|--------|
| All measurement rows complete | `[ ]` |
| Both pad brands measured | `[ ]` |
| CAD tool selected and installed | `[ ]` |
| Git repository initialised | `[ ]` |
| Phase 0 sign-off by owner | `[ ]` |

**Sign-off**: ___________________ Date: ___________

> **IMPORTANT**: Do NOT begin modelling until Phase 0 measurements are complete.
> Every dimension in the CAD model traces back to a real measurement.

---

## Phase 1 — Mechanical Concept & Layout

**Goal**: Decide the dispensing mechanism type and overall enclosure layout.

### 1.1 Dispensing Mechanism Options

| Option | Pros | Cons |
|--------|------|------|
| **Rotating drum / paddle wheel** | Simple single-motor, low jam risk | Pad must be soft/flexible |
| **Push-slide tray** | Good for stiff pads | Needs linear actuator or rack-and-pinion |
| **Gravity-fed with gate** | Fewest moving parts | Orientation-sensitive, can jam |

> ⚠️ **No verdict here.** Mechanism selection is made in [`mechanism_decision.md`](./mechanism_decision.md) after Phase 0 measurements are complete. Do not pre-select a mechanism before pad dimensions and motor torque are confirmed.

### 1.2 Internal Layout Diagram (Text)

```
┌────────────────────────────────────┐
│  ┌──────────┐   Stack Chamber      │
│  │  Pad     │  (top-loading lid)   │
│  │  Stack   │                      │
│  └────┬─────┘                      │
│       │ gravity feed               │
│  ┌────▼─────────┐                  │
│  │  Motor +     │  ← Dispensing    │
│  │  Mechanism   │    Module        │
│  └────┬─────────┘                  │
│       │ single pad exits here      │
│  ┌────▼─────────┐                  │
│  │  IR Break-   │  ← Detects pad  │
│  │  Beam Slot   │    removal       │
│  └──────────────┘                  │
│                                    │
│  ┌──────────────┐                  │
│  │ Electronics  │  ESP32, Motor    │
│  │ Bay          │  Driver, Wiring  │
│  └──────────────┘                  │
│                                    │
│  ┌──────────────┐                  │
│  │ HC-SR04 Bay  │  ← Faces into   │
│  │ (top-facing) │    stack chamber │
│  └──────────────┘                  │
└────────────────────────────────────┘
```

### 1.3 Deliverables

- [ ] Concept sketch (hand-drawn or digital, photographed and saved to `/docs/sketches/`)
- [ ] **Torque estimate completed**: calculate required torque = (pad stack weight × paddle arm length) and confirm it is ≤ motor's rated torque. Document result in `mechanism_decision.md` under the Motor Torque criterion.
- [ ] Mechanism decision documented in [`mechanism_decision.md`](./mechanism_decision.md) with status updated to `PROVISIONAL`

---

## Phase 2 — CAD Modelling

**Goal**: Model all printable parts in your CAD tool of choice.

### Recommended CAD Tools (Free)

| Tool | Best For | Notes |
|------|----------|-------|
| **Fusion 360** | Parametric, professional | Free for hobbyists/startups |
| **FreeCAD** | 100% open-source | Steeper learning curve |
| **Tinkercad** | Quick blockout | Limited for complex mechanisms |
| **OpenSCAD** | Code-driven, version-controllable | Great for parametric parts |

### 2.1 Parts to Model

#### Enclosure
| Part | File | Description |
|------|------|-------------|
| Main body | `body_main.stl` | Outer shell, electronics bay cutouts |
| Lid / top cover | `lid_top.stl` | Top-loading access for pad stack |
| Back panel | `panel_back.stl` | Ventilation, wire routing |
| Front face | `panel_front.stl` | Dispense slot, branding cutout |

#### Dispensing Mechanism
| Part | File | Description |
|------|------|-------------|
| Motor mount bracket | `motor_bracket.stl` | Holds motor, aligns with mechanism |
| Paddle wheel / drum | `paddle_wheel.stl` | Contacts pad stack, feeds one at a time |
| Mechanism housing | `mechanism_housing.stl` | Surrounds paddle, guides pad path |
| Dispense chute | `chute_dispense.stl` | Guides pad from mechanism to exit slot |

#### Sensor Mounts
| Part | File | Description |
|------|------|-------------|
| IR break-beam mount | `mount_ir_beam.stl` | Holds TX and RX of IR pair at dispense exit |
| HC-SR04 mount | `mount_ultrasonic.stl` | Aims sensor down into stack chamber |
| ESP32 tray | `tray_esp32.stl` | Snap-fit tray in electronics bay |

#### Payment & UI Components (NEW — M-PESA)
| Part | File | Description |
|------|------|-------------|
| OLED display bezel | `bezel_oled.stl` | Friction-fit frame for 1.3" OLED in front panel |
| Keypad mount | `mount_keypad.stl` | Recess for 4×4 matrix keypad in front panel |
| GSM module tray | `tray_gsm.stl` | Holds SIM800L + routes antenna cable |
| Key lock collar | `collar_keylock.stl` | Housing for admin key lock cylinder in lid |
| Buzzer port plug | `plug_buzzer.stl` | Grille over buzzer hole (blocks humidity ingress) |
| Wall mount bracket | `bracket_wall.stl` | Keyhole slots for wall screws; attaches to back of body |

### 2.2 Design Rules for Local FDM Printing

- Minimum wall thickness: **1.2 mm** (3× 0.4 mm nozzle)
- Minimum bridging gap without support: **≤ 50 mm**
- Snap-fit clearance: **0.2–0.3 mm** per side
- All overhangs **> 45°** must have chamfer or support
- Through-holes for M3 screws: model at **2.7 mm** (print expands ~0.3 mm)
- Label all parts with part name embossed at 0.3 mm depth for assembly
- **Material safety — contact parts**: `paddle_wheel.stl`, `chute_dispense.stl`, and `mechanism_housing.stl` physically touch every pad before it reaches the user. These parts **must use food-safe / body-safe PETG** (e.g., Prusament PETG or equivalent certified grade). PLA is **not acceptable** for contact surfaces. Design all contact surfaces to be wipe-cleanable with isopropyl alcohol without disassembly.

---

## Phase 3 — 3D Simulation & Validation

**Goal**: Virtually prove the mechanism works before printing a single gram of filament.

### 3.1 Simulation Tools

| Tool | Purpose | Notes |
|------|---------|-------|
| **Fusion 360 Motion Study** | Animate paddle wheel rotation, check pad path | Built-in |
| **Blender** (rigid body sim) | Simulate pad gravity-fall through chute | Free |
| **Gazebo / ROS** | Full kinematic simulation | Overkill for this scale |
| **Manual clearance check** | Open all bodies, verify no overlaps | Minimum baseline |

### 3.2 Simulation Checklist

- [ ] Paddle wheel rotates without clipping into housing
- [ ] Single pad separates cleanly from stack on one rotation
- [ ] Pad travels chute without jamming at corners
- [ ] Pad exits at correct angle/position relative to IR beam
- [ ] IR beam mounting clears the pad width with ≥ 5 mm margin
- [ ] HC-SR04 field of view covers full stack cross-section
- [ ] Electronics bay lid closes without pinching wires
- [ ] Top lid opens and closes without tool (snap or hinge)

### 3.3 Jam Simulation

Model the worst-case pad (slightly expanded, maximum thickness) and verify:
- [ ] Mechanism can still actuate without binding
- [ ] There is a manual-override slot to clear a jammed pad

### Phase 3 Exit Criteria

> 🚫 **DO NOT advance to Phase 4 (slicer prep) until ALL simulation checklist items are ticked.**
> Advancing with unchecked items means printing parts that may require remodelling — wasted filament.

| Exit Check | Status |
|------------|--------|
| All Section 3.2 simulation items ticked | `[ ]` |
| All Section 3.3 jam simulation items ticked | `[ ]` |
| No mesh clipping detected in clearance check | `[ ]` |
| Pad path confirmed for both pad brands from Phase 0 | `[ ]` |
| Phase 3 sign-off by owner | `[ ]` |

**Sign-off**: ___________________ Date: ___________

---

## Phase 4 — Slicer Preparation & Print Validation

**Goal**: Confirm all parts are sliceable and within print-bed limits.

### 4.1 Slicer Checklist

| Check | Tool | Pass Criteria |
|-------|------|---------------|
| Watertight mesh | Meshmixer / PrusaSlicer | No open edges |
| Fits print bed | PrusaSlicer / Cura | Within bed dimensions |
| Support volume reasonable | PrusaSlicer | < 20% of part volume |
| Layer adhesion at stress points | Manual review | Correct orientation chosen |
| Estimated print time | Slicer | Documented in BOM |

### 4.2 Print Order (Prototype 1)

Print non-critical, low-filament parts first to validate fit:

1. `mount_ir_beam.stl` — small, tests snap-fit clearance
2. `mount_ultrasonic.stl` — small, tests HC-SR04 fit
3. `tray_esp32.stl` — tests electronics bay geometry
4. `motor_bracket.stl` — tests motor alignment
5. `paddle_wheel.stl` — tests mechanism
6. `mechanism_housing.stl` — must be printed **before** chute; forms the pad path boundary
7. `chute_dispense.stl` — **immediately test-assemble with `mechanism_housing.stl`**; verify pad flows cleanly through the full path using both pad brands
8. `panel_front.stl`
9. `panel_back.stl`
10. `lid_top.stl`
11. `body_main.stl` — largest, print last

---

## Phase 5 — Design Iteration

**Goal**: Fix issues found during simulation and test prints.

- Document every change in [`changelog_3d.md`](./changelog_3d.md)
- Keep old STL versions with version suffix (e.g., `paddle_wheel_v1.stl`)

### Phase 5 Sign-Off Criteria

Sign-off is granted when **all** of the following are true:

| Criterion | Owner | Status |
|-----------|-------|--------|
| Phase 3 simulation fully passed (all gates ticked) | You | `[ ]` |
| At least 1 test print of every part completed | You | `[ ]` |
| Full dry-fit assembly succeeds without forcing | You | `[ ]` |
| Pad dispensed successfully with **both** brands from Phase 0 | You | `[ ]` |
| No open issues remain in `changelog_3d.md` | You | `[ ]` |
| Mechanism confirmed operational by `mechanism_decision.md` prototype section | You | `[ ]` |

**Sign-off**: ___________________ Date: ___________

---

## Phase 6 — Handoff to Electronics Phase

**Goal**: 3D phase is complete when:

- [ ] All STLs printed at least once and physically dry-fit together
- [ ] Mechanism operates by hand (manual rotation test)
- [ ] All sensor mounts confirmed with real sensor modules
- [ ] Electronics bay confirmed to fit ESP32 + motor driver board
- [ ] Wire routing paths confirmed (no sharp bends, no pinch points)
- [ ] Final STL files committed to `<repo-root>/hardware/cad/` in the Git repository initialised in Phase 0

---

## Phase 7 — Backend Server Setup

**Goal**: Deploy a cloud server to receive M-PESA callbacks and relay payment confirmations to the ESP32.

- [ ] Choose hosting platform (Railway / Render free tier recommended)
- [ ] Deploy Node.js or Python app with HTTPS endpoint
- [ ] Register callback URL with Safaricom Daraja (`/mpesa/callback`)
- [ ] Implement MQTT broker or polling endpoint for ESP32
- [ ] Implement transaction log (SQLite or Firebase)
- [ ] Test end-to-end with Daraja sandbox
- [ ] Document server URL and credentials in `payment_integration.md`

---

## Phase 8 — M-PESA Daraja Integration

**Goal**: Integrate STK Push flow with validated payment confirmation.

- [ ] Register on [developer.safaricom.co.ke](https://developer.safaricom.co.ke)
- [ ] Create Daraja app, obtain Consumer Key + Secret
- [ ] Implement STK Push initiation from backend (triggered by ESP32 request)
- [ ] Implement C2B callback handler on backend
- [ ] Validate payment amount, account reference, and phone number
- [ ] Test with Safaricom sandbox, then live environment
- [ ] Document flow in `payment_integration.md`

---

## Phase 9 — Firmware Development (ESP32)

**Goal**: Implement the full device state machine on the ESP32.

### State Machine
```
IDLE → KEYPAD_INPUT → STK_SENT → AWAITING_PAYMENT → DISPENSING → AWAITING_REMOVAL → STOCK_CHECK → IDLE
                                       │
                               TIMEOUT / ERROR
```

### Firmware Tasks
- [ ] WiFi + GSM fallback connectivity manager
- [ ] OLED display driver (U8g2 or Adafruit SSD1306)
- [ ] 4×4 keypad input handler (phone number entry)
- [ ] HTTP/MQTT client — poll backend for payment status
- [ ] Motor control (90° indexed rotation for paddle wheel)
- [ ] IR break-beam detection (pad-taken confirmation)
- [ ] HC-SR04 stock level monitoring
- [ ] LED + buzzer feedback
- [ ] OTA (over-the-air) firmware update capability
- [ ] Document pinout and config in `firmware_spec.md`

---

## Phase 10 — System Integration Testing

**Goal**: Validate the full pay-to-dispense loop before washroom installation.

- [ ] Bench test: enter phone number → STK push received → PIN entered → pad dispenses
- [ ] Test payment timeout (no PIN entered within 60 s)
- [ ] Test failed payment (incorrect PIN)
- [ ] Test out-of-stock detection and display
- [ ] Test GSM fallback when WiFi is disconnected
- [ ] Test with both pad brands from Phase 0
- [ ] Measure and log motor current draw under load
- [ ] Stress test: 50 consecutive dispense cycles

---

## Phase 11 — Washroom Deployment & Monitoring

**Goal**: Install in target washroom and confirm real-world operation.

- [ ] Wall-mount unit at appropriate height (accessibility)
- [ ] Connect to mains power via 5V adapter
- [ ] Confirm GSM signal strength at installation location
- [ ] Run 5 live M-PESA transactions as acceptance test
- [ ] Confirm transaction log on backend matches physical dispenses
- [ ] Set up low-stock alert (email/SMS to owner when < 3 pads remain)
- [ ] Document restocking procedure in `assembly_notes.md`

---

## File Index

| File | Purpose |
|------|---------|
| [`implementation_plan.md`](./implementation_plan.md) | This file — master development plan |
| [`3d_design_spec.md`](./3d_design_spec.md) | Detailed dimensional spec for all parts |
| [`mechanism_decision.md`](./mechanism_decision.md) | Mechanical design decision log |
| [`bom_hardware.md`](./bom_hardware.md) | Bill of materials (hardware + filament) |
| [`print_settings.md`](./print_settings.md) | Per-part slicer settings |
| [`changelog_3d.md`](./changelog_3d.md) | Version log for all CAD changes |
| [`assembly_notes.md`](./assembly_notes.md) | Step-by-step physical assembly guide — ⚠️ **FILE DOES NOT EXIST YET — create stub before Phase 6** |
| [`payment_integration.md`](./payment_integration.md) | M-PESA Daraja API setup, STK Push flow, backend architecture, ESP32 MQTT spec |
| [`firmware_spec.md`](./firmware_spec.md) | ESP32 state machine, pinout, WiFi/GSM config, motor control, OTA |
