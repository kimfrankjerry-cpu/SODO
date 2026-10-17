# ESP32 sensor and actuator tests

PlatformIO projects for an ESP32 DevKit (`esp32dev`, Arduino framework). Each folder is a separate project: open it in VS Code with the PlatformIO extension, then Upload and open the Serial Monitor (115200 baud).

| Project | Hardware | Status |
|---|---|---|
| [Servo_Test](Servo_Test) | MG946R / MG996R 360° continuous-rotation servo | Working, roughly calibrated |
| [E18_IR_Sensor](E18_IR_Sensor) | E18-D80NK IR obstacle sensor | Working; detection level flipped to HIGH, not yet re-tested |
| [VL53L0X_Distance](VL53L0X_Distance) | VL53L0X time-of-flight distance sensor | Not working yet, see below |
| [I2C_Diag](I2C_Diag) | Diagnostic firmware for the I²C bus | Tool |

## Servo_Test

Each press of the **BOOT** button (or `r` over serial) turns the servo 3 full turns clockwise, then stops.

Wiring: brown → supply GND **and** ESP32 GND, red → external 5–6 V (2 A+), orange → **GPIO 26**.

A 360° servo has no position feedback, so turns are timed. Calibration at 50% speed:

| Run time | Rotation |
|---|---|
| 5320 ms | ~630° |
| 6080 ms | ~715° (2 turns, ~5° short) |
| 9300 ms | 3 turns |

The servo coasts a little after stopping, so time doesn't scale exactly with turns. The calibration drifts with supply voltage and load, so it needs redoing if either changes. It can be adjusted over serial without re-uploading: `t<ms>` sets the run time, `f` flips the direction, `p` prints the settings.

## E18_IR_Sensor

Wiring: brown → 5V/VIN, blue → GND, black → GPIO 27. This unit reads HIGH when an object is detected (`DETECT_LEVEL` in `main.cpp`). The trigger distance is set with the screw on the back.

## VL53L0X_Distance

The sensor never answered on I²C with SDA/SCL on GPIO 21/22. Later testing showed that pins on the **right-hand header** of this particular board (D18, D21, D22) don't make contact, while left-side pins work. The next step is to move SDA/SCL to left-side pins (for example GPIO 32/33) and set them in `Wire.begin(sda, scl)`.

## Board note

On this ESP32 board, use the left-hand header pins (D13, D14, D25, D26, D27, D32, D33) until the right-hand header problem is fixed.
