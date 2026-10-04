#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

// --- Network & API ---
#define GSM_APN          "safaricom" // Safaricom APN
#define GSM_USER         ""
#define GSM_PASS         ""
#define BACKEND_URL      "https://f766028488640e.lhr.life" // Replace with your Railway URL later

// --- Settings ---
#define PAD_PRICE_KES    2
#define PAYMENT_TIMEOUT  60000 // ms (60 seconds)
#define POLL_INTERVAL    2000  // ms (2 seconds)
#define MIN_STOCK_MM     150   // mm sensor-to-stack-top distance = empty (calibrate; HC-SR04 min range ~20 mm)
#define MOTOR_90_MS      1000  // ms to run servo for 90 deg (needs calibration)

// --- Pin Definitions ---
// SIM800L GSM Module (Hardware Serial 2)
#define MODEM_RX_PIN     16
#define MODEM_TX_PIN     17

// OLED (I2C)
#define I2C_SDA_PIN      21
#define I2C_SCL_PIN      22

// Servo Motor (MG996R)
#define SERVO_PIN        4

// IR Break-Beam
#define IR_TX_PIN        18
#define IR_RX_PIN        19

// Ultrasonic HC-SR04
#define HC_TRIG_PIN      23
#define HC_ECHO_PIN      25

// UI Feedback
#define BUZZER_PIN       26
#define LED_RED_PIN      27
#define LED_GREEN_PIN    32
#define LED_AMBER_PIN    33

// Keypad (4x4 Matrix)
// Note: Rows 34-39 need external 10k pull-ups to 3V3!
#define KP_ROW1          34
#define KP_ROW2          35
#define KP_ROW3          36
#define KP_ROW4          39
#define KP_COL1          13
#define KP_COL2          14
#define KP_COL3          15
#define KP_COL4          5   // Was GPIO12 (boot strap pin) - moved to GPIO5

#endif // CONFIG_H
