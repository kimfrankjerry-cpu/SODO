# Smart Pad Dispenser — Firmware Specification

> **MCU**: ESP32 (38-pin WROOM-32)
> **Framework**: Arduino (via PlatformIO)
> **Connectivity**: WiFi (primary) + SIM800L GSM (fallback)
> **Payment**: M-PESA STK Push via backend server

---

## 1. State Machine

```
┌──────────────────────────────────────────────────────────────┐
│                         IDLE                                 │
│  OLED: price + "Pay via M-PESA"                             │
│  LED: Red                                                    │
│  Action: wait for keypad input                               │
└────────────────────┬─────────────────────────────────────────┘
                     │ first key pressed
┌────────────────────▼─────────────────────────────────────────┐
│                    KEYPAD_INPUT                              │
│  OLED: "Enter phone: 07__________"                          │
│  Action: collect 10 digits, backspace supported              │
│  Timeout: 30 s of inactivity → back to IDLE                 │
└────────────────────┬─────────────────────────────────────────┘
                     │ 10 digits entered + confirm (#)
┌────────────────────▼─────────────────────────────────────────┐
│                    STK_SENT                                  │
│  OLED: "Check your phone for M-PESA prompt"                 │
│  LED: Amber                                                  │
│  Action: POST /mpesa/initiate to backend                    │
│  Timeout: 5 s for backend to respond → ERROR                │
└────────────────────┬─────────────────────────────────────────┘
                     │ backend responds with checkoutRequestId
┌────────────────────▼─────────────────────────────────────────┐
│                 AWAITING_PAYMENT                             │
│  OLED: "Awaiting payment... [countdown]"                    │
│  LED: Amber (slow blink)                                    │
│  Action: poll GET /payment/status/{id} every 2 s            │
│  Timeout: 60 s → PAYMENT_TIMEOUT                            │
└───────┬────────────────────────────┬────────────────────────┘
        │ CONFIRMED                  │ FAILED / CANCELLED
┌───────▼───────────┐    ┌──────────▼─────────────────────────┐
│    DISPENSING     │    │         PAYMENT_FAILED             │
│  OLED: "Paid! ✓" │    │  OLED: "Payment failed."           │
│  LED: Green       │    │         "Please try again."        │
│  Buzzer: 2 beeps  │    │  LED: Red (fast blink x3)          │
│  Motor: 90° CW    │    │  Buzzer: 1 long beep               │
└───────┬───────────┘    │  → back to IDLE after 4 s          │
        │ IR blocked     └────────────────────────────────────┘
┌───────▼─────────────────────────────────────────────────────┐
│                 AWAITING_PAD_REMOVAL                        │
│  OLED: "Take your pad →"                                   │
│  LED: Green (slow blink)                                   │
│  Action: wait for IR beam to clear                         │
│  Timeout: 30 s → log uncollected pad, return to IDLE       │
└───────┬─────────────────────────────────────────────────────┘
        │ IR cleared
┌───────▼─────────────────────────────────────────────────────┐
│                    STOCK_CHECK                              │
│  Action: HC-SR04 measures stack height                     │
│  If height > MIN_STOCK_MM → IDLE                           │
│  If height ≤ MIN_STOCK_MM → OUT_OF_STOCK                  │
└───────┬────────────────┬────────────────────────────────────┘
        │ OK             │ empty
        ▼                ▼
      IDLE       ┌───────────────────────────────────────────┐
                 │           OUT_OF_STOCK                    │
                 │  OLED: "Out of stock. Please check back." │
                 │  LED: Red (solid)                         │
                 │  Action: POST /admin/alert to backend     │
                 │  Stays here until restocked (HC-SR04)     │
                 └───────────────────────────────────────────┘
```

---

## 2. Pin Assignment

| GPIO | Direction | Function | Component |
|------|-----------|----------|-----------|
| 21 | I/O | SDA | OLED display (I2C) |
| 22 | I/O | SCL | OLED display (I2C) |
| 16 | IN | RX ← SIM800L TX | GSM module |
| 17 | OUT | TX → SIM800L RX | GSM module |
| 4 | OUT | Motor IN1 / PWM | L298N / DRV8833 |
| 5 | OUT | Motor IN2 | L298N / DRV8833 |
| 18 | OUT | IR TX enable | IR break-beam TX |
| 19 | IN | IR RX signal | IR break-beam RX |
| 23 | OUT | HC-SR04 TRIG | Ultrasonic sensor |
| 25 | IN | HC-SR04 ECHO | Ultrasonic sensor |
| 26 | OUT | Buzzer | Passive buzzer |
| 27 | OUT | LED Red | Status LED |
| 32 | OUT | LED Green | Status LED |
| 33 | OUT | LED Amber | Status LED |
| 34 | IN | Keypad Row 1 | 4×4 matrix |
| 35 | IN | Keypad Row 2 | 4×4 matrix |
| 36 | IN | Keypad Row 3 | 4×4 matrix |
| 39 | IN | Keypad Row 4 | 4×4 matrix |
| 13 | OUT | Keypad Col 1 | 4×4 matrix |
| 14 | OUT | Keypad Col 2 | 4×4 matrix |
| 15 | OUT | Keypad Col 3 | 4×4 matrix |
| 12 | OUT | Keypad Col 4 | 4×4 matrix |

> ⚠️ GPIO 34–39 are INPUT-ONLY on ESP32 (no internal pull-up). Use external 10kΩ pull-ups on keypad row pins.
> ⚠️ GPIO 12 must be LOW at boot or ESP32 fails to boot. Use with caution for keypad column.

---

## 3. WiFi + GSM Connectivity Manager

```
Boot
  │
  ├─▶ Try WiFi (30 s timeout)
  │       │ success → use WiFi for all HTTP calls
  │       │ fail ──▶ Try SIM800L GSM
  │                   │ success → use GSM for all HTTP calls
  │                   │ fail ──▶ enter OFFLINE mode
  │                               OLED: "No connection. Check network."
  │                               Retry both every 60 s
  │
  └─▶ If WiFi drops during operation → seamlessly switch to GSM
```

### Config (stored in `config.h` or SPIFFS)

```cpp
// config.h
#define WIFI_SSID        "YourSSID"
#define WIFI_PASSWORD    "YourPassword"
#define BACKEND_URL      "https://your-server.railway.app"
#define PAD_PRICE_KES    10          // Price per pad
#define PAYMENT_TIMEOUT  60000       // ms — 60 seconds
#define POLL_INTERVAL    2000        // ms — poll every 2 s
#define MIN_STOCK_MM     15          // HC-SR04 distance threshold (mm)
#define GSM_APN          "safaricom" // Safaricom APN
```

---

## 4. Key Functions

### 4.1 Phone Number Handling
```cpp
String formatPhone(String raw) {
    // Input:  "0712345678"
    // Output: "254712345678"
    if (raw.startsWith("0")) {
        return "254" + raw.substring(1);
    }
    return raw; // already formatted
}
```

### 4.2 Motor Control (Paddle Wheel — 90° index)
```cpp
void dispense() {
    // Rotate motor until encoder counts 90° (or timed if no encoder)
    digitalWrite(MOTOR_IN1, HIGH);
    digitalWrite(MOTOR_IN2, LOW);
    delay(MOTOR_90_MS);    // calibrate: time for 90° at current PWM
    digitalWrite(MOTOR_IN1, LOW);
    digitalWrite(MOTOR_IN2, LOW);
}
```

> ⚠️ `MOTOR_90_MS` must be calibrated empirically once motor and paddle wheel are assembled.
> A rotary encoder on the motor shaft would eliminate this calibration need (consider for v2).

### 4.3 Stock Level
```cpp
int getStackDistanceMm() {
    // Returns distance from HC-SR04 to top of pad stack in mm
    // Lower distance = more pads (sensor is above stack)
    long duration = pulseIn(ECHO_PIN, HIGH);
    return duration * 0.034 / 2;
}

bool isOutOfStock() {
    return getStackDistanceMm() > MIN_STOCK_MM;
}
```

### 4.4 Payment Polling
```cpp
String pollPaymentStatus(String checkoutId) {
    // Returns "CONFIRMED", "FAILED", "PENDING"
    // HTTP GET /payment/status/{checkoutId}
    // Parse JSON response: { "status": "CONFIRMED" }
}
```

---

## 5. OLED Display Layout

```
┌────────────────────────┐
│  🧴 PAD DISPENSER      │   ← line 0: title / icon
│                        │
│  Price: KES 10         │   ← line 2: price
│  Pay via M-PESA        │   ← line 3: instruction
│  Till: 123456          │   ← line 4: Till/Paybill number
│                        │
│  [Enter phone no.]     │   ← line 6: prompt (state-dependent)
└────────────────────────┘
```

---

## 6. OTA (Over-The-Air Updates)

Use **Arduino OTA** or **ESP32 HTTPS OTA** to update firmware without physical access.

```cpp
#include <ArduinoOTA.h>

void setupOTA() {
    ArduinoOTA.setHostname("pad-dispenser-01");
    ArduinoOTA.setPassword("admin-ota-password"); // change this
    ArduinoOTA.begin();
}

// Call ArduinoOTA.handle() in loop()
```

> 🔒 Change the default OTA password before deploying. OTA is active only on WiFi (not GSM).

---

## 7. Libraries (PlatformIO)

```ini
; platformio.ini
[env:esp32dev]
platform = espressif32
board = esp32dev
framework = arduino
monitor_speed = 115200
lib_deps =
    bblanchon/ArduinoJson@^7.0.0
    adafruit/Adafruit SSD1306@^2.5.7
    adafruit/Adafruit GFX Library@^1.11.9
    plerup/EspSoftwareSerial@^8.1.0
    arduino-libraries/Keypad@^3.1.1
```

---

## 8. Open Items

| Item | Status |
|------|--------|
| Calibrate `MOTOR_90_MS` | `[ ]` Requires assembled prototype |
| Confirm GPIO 12 boot-safe workaround | `[ ]` |
| Set `MIN_STOCK_MM` threshold | `[ ]` Requires physical measurement |
| Finalise `PAD_PRICE_KES` | `[ ]` Business decision |
| Finalise WiFi credentials for washroom | `[ ]` |
| Finalise backend URL after deployment | `[ ]` |

---

## 9. Revision History

| Version | Date | Notes |
|---------|------|-------|
| v0.1 | `[DATE]` | Initial spec — state machine, pin assignment, WiFi+GSM, OTA |
