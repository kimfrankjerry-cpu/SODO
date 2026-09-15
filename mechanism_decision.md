# Smart Pad Dispenser — Mechanism Decision Log

> This document records the analysis and selection of the dispensing mechanism type.
> Once decided, this becomes the locked specification for Phase 2 CAD modelling.

---

## Decision Status: 🔲 PROVISIONAL — awaiting prototype validation

---

## Options Evaluated

### Option A — Rotating Paddle Wheel

**Description**: A wheel with 4 radial paddles sits at the base of the stack chamber.
Each 90° rotation of the wheel scoops exactly one pad from the bottom of the stack
and carries it to the dispense slot.

**Diagram**:
```
  [PAD STACK]
       │
  ┌────▼────┐
  │  ╱╲     │   Paddle scoops
  │ ╱  ╲    │   one pad per
  │╱    ╲   │   90° turn
  │  ◎   │  │   ◎ = shaft
  │╲    ╱   │
  └──────────┘
       │
   [CHUTE] → EXIT
```

| Criterion | Weight | Score (1–5) | Weighted | Notes |
|-----------|--------|-------------|----------|-------|
| Simplicity | 1× | 5 | 5 | One motor, one moving part |
| Jam risk | **2×** | 4 | 8 | Low if paddle clearance is correct |
| Single-pad reliability | **2×** | 4 | 8 | Depends on stack pressure and pad orientation |
| Print complexity | 1× | 3 | 3 | Paddle wheel geometry is moderate |
| Motor torque requirement | **2×** | 3 | 6 | Moderate — stack weight resists paddle; torque calc required |
| Motor type compatibility | 1× | 5 | 5 | Works with servo, stepper, or DC geared |
| Hygiene / cleanability | **2×** | 3 | 6 | Paddle contacts every pad; food-safe PETG required; wipe-down feasible |
| **TOTAL** | **11×** | **—** | **41 / 55** | |

---

### Option B — Push-Slide Tray

**Description**: A motorised tray slides horizontally beneath the stack.
One pad rests on the tray, which slides out to expose the pad to the dispense slot,
then retracts for the next pad to drop.

| Criterion | Weight | Score (1–5) | Weighted | Notes |
|-----------|--------|-------------|----------|-------|
| Simplicity | 1× | 3 | 3 | Requires linear motion (rack + pinion or lead screw) |
| Jam risk | **2×** | 3 | 6 | Pad can fold at tray edge during retraction |
| Single-pad reliability | **2×** | 4 | 8 | Positive control over pad position |
| Print complexity | 1× | 4 | 4 | Linear guide rails are well-understood |
| Motor torque requirement | **2×** | 4 | 8 | Linear load is lower than rotational stack resistance |
| Motor type compatibility | 1× | 3 | 3 | Needs linear actuator or leadscrew — **different motor than Option A** |
| Hygiene / cleanability | **2×** | 4 | 8 | Tray contacts only pad underside; easier to wipe |
| **TOTAL** | **11×** | **—** | **40 / 55** | |

---

### Option C — Gravity Gate

**Description**: Pads are stacked vertically. A gate at the bottom holds them.
Briefly opening the gate releases exactly one pad into the chute.

| Criterion | Weight | Score (1–5) | Weighted | Notes |
|-----------|--------|-------------|----------|-------|
| Simplicity | 1× | 4 | 4 | Simple gate mechanism |
| Jam risk | **2×** | 2 | 4 | Gravity-sensitive; orientation-dependent; can release multiple |
| Single-pad reliability | **2×** | 2 | 4 | Hard to reliably release exactly one pad |
| Print complexity | 1× | 4 | 4 | Simple parts |
| Motor torque requirement | **2×** | 5 | 10 | Near-zero — gravity does the work; gate just holds/releases |
| Motor type compatibility | 1× | 4 | 4 | Servo works well |
| Hygiene / cleanability | **2×** | 5 | 10 | Gate does not contact pad surface |
| **TOTAL** | **11×** | **—** | **40 / 55** | |

---

### Option C2 — Dual Gravity Gate (Vending-Style Sequential Gates)

**Description**: Two gates open alternately. Gate 1 holds the full stack; Gate 1 opens briefly
to drop one pad onto Gate 2; Gate 1 closes; Gate 2 opens to release the single pad to the chute.
This is the standard mechanism in vending machines and achieves near-100% single-item reliability.
Requires a stepper motor or two servos for precise timing.

| Criterion | Weight | Score (1–5) | Weighted | Notes |
|-----------|--------|-------------|----------|-------|
| Simplicity | 1× | 2 | 2 | Two gates = two actuation events per dispense cycle |
| Jam risk | **2×** | 4 | 8 | Dual containment significantly reduces multi-dispense |
| Single-pad reliability | **2×** | 5 | 10 | Near-100% in commercial vending applications |
| Print complexity | 1× | 2 | 2 | Two gate bodies + linkage = more printed parts |
| Motor torque requirement | **2×** | 4 | 8 | Gravity-assisted; gate only needs to hold/release stack |
| Motor type compatibility | 1× | 3 | 3 | Stepper preferred for timing; servo pair also works |
| Hygiene / cleanability | **2×** | 4 | 8 | Gates contact only pad edges, not pad surface |
| **TOTAL** | **11×** | **—** | **41 / 55** | |

> **Verdict**: Ties with Paddle Wheel on weighted score. Not selected for v1 due to higher print
> complexity and two-gate timing requirement. **Re-evaluate before defaulting to Option B** if
> the Paddle Wheel fails prototype validation.

---

## Decision Matrix

> Scoring is weighted: Jam Risk, Single-pad Reliability, Motor Torque, and Hygiene are **2×** weight.
> Total possible score = 55.

| Mechanism | Weighted Score | Selected |
|-----------|---------------|----------|
| Paddle Wheel | 41 / 55 | 🔲 **PROVISIONAL** — validate with prototype |
| Dual Gravity Gate (C2) | 41 / 55 | Re-evaluate first if Paddle Wheel fails |
| Push-Slide Tray | 40 / 55 | Last resort — requires different motor and enclosure (see warning below) |
| Gravity Gate (single) | 40 / 55 | Not recommended — poor single-pad reliability |

---

## Provisional Decision (Pre-Prototype)

**Selected Mechanism**: Paddle Wheel

**Decision Date**: `[REQUIRED BEFORE PHASE 2 START]`

**Decision Made By**: `[REQUIRED BEFORE PHASE 2 START]`

**Assumed Pad Orientation**: Pads stacked face-up, wrapped-side down, wings folded inward.
> ⚠️ This orientation must be confirmed during Phase 0 measurements and validated in the prototype test.
> If wings fold outward or pads are stacked differently, paddle clearance dimensions must be recalculated.

**Rationale**:
- Joint-highest weighted score (41/55) alongside Option C2
- Single motor, simplest wiring and firmware
- Compatible with DC geared motor (most available locally)
- Low risk of double-dispensing if paddle width < pad width

**Conditions / Risks to Monitor During Prototype**:
1. Stack pressure on bottom pad must not exceed the motor's rated torque at the paddle arm length
2. Paddle width must be verified not to catch on pad wings during scoop motion
3. Contact surfaces (paddle, chute, housing) must use food-safe PETG — not PLA
4. If pads are too thick for the paddle pocket → switch to **Option C2 (Dual Gravity Gate)** first, then Option B

> ⚠️ **Fallback Cost Warning**: Switching to Push-Slide Tray (Option B) is **NOT a drop-in swap.**
> It requires a different motor type (linear actuator), different motor driver, different firmware logic,
> and significant changes to the enclosure geometry. Fallback to Option B invalidates most Phase 2 CAD work.
>
> **Fallback trigger threshold**: If compressed pad thickness > `[MEASURE in Phase 0]` mm, the paddle pocket
> cannot fit one pad cleanly. Establish this threshold in Phase 0 before committing to CAD.

---

## Confirmed Decision (Post-Prototype)

> Complete this section after the Phase 5 prototype validation test.

**Prototype Test Date**: `[DATE]`

**Test Result**: `[ ]` PASS / `[ ]` FAIL

**If PASS** — mechanism is confirmed; proceed to Phase 6 electronics handoff.
**If FAIL** — document failure mode below and select fallback from Decision Matrix.

**Failure Mode (if applicable)**: ___________________________________________

**Fallback Selected (if applicable)**: ___________________________________________

**Confirmed By**: ___________________ Date: ___________

---

## Revision History

| Version | Date | Change |
|---------|------|--------|
| v0.1 | `[DATE]` | Initial options analysis |
| v0.2 | `[DATE]` | Decision recorded |
