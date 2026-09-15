# Smart Pad Dispenser — 3D CAD Changelog

> Log every design change here. Keep old STL files with version suffixes (e.g., `paddle_wheel_v1.stl`).
> Never delete old versions — they are your rollback path.

---

## Format

```
## vX.Y — YYYY-MM-DD
### Changed
- `filename.stl`: What changed and why
### Added
- `filename.stl`: New part and its purpose
### Removed / Deprecated
- `filename.stl`: Why it was retired
### Known Issues
- Description of any open problems in this version
```

---

## v0.1 — `[DATE]`

### Added
- Initial scaffolding of all part files (no geometry yet, placeholder only)
- `body_main.stl`: Outer enclosure shell — dimensions TBD
- `lid_top.stl`: Top-loading lid — dimensions TBD
- `panel_front.stl`: Front face with dispense slot — dimensions TBD
- `panel_back.stl`: Back panel — dimensions TBD
- `motor_bracket.stl`: Motor mount — motor dimensions TBD
- `paddle_wheel.stl`: Dispensing wheel — mechanism TBD
- `mechanism_housing.stl`: Paddle wheel surround — TBD
- `chute_dispense.stl`: Pad exit chute — TBD
- `mount_ir_beam.stl`: IR break-beam sensor mounts — TBD
- `mount_ultrasonic.stl`: HC-SR04 mount — TBD
- `tray_esp32.stl`: ESP32 electronics tray — TBD

### Known Issues
- All dimensions are placeholders — blocked on Phase 0 measurements

---

## v0.2 — `[DATE]` ← Fill in as you iterate

### Changed
- _To be recorded_

### Known Issues
- _To be recorded_

---

> **Tip**: Commit CAD source files (`.f3d`, `.FCStd`, `.scad`) to version control, not just STLs.
> STLs are exports — source files let you edit parameters later.
