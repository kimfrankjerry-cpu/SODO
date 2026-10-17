/*
  ESP32 + VL53L0X distance reader

  Wiring (VL53L0X -> ESP32):
    VIN   -> 3V3
    GND   -> GND
    SDA   -> GPIO 21
    SCL   -> GPIO 22
    XSHUT -> GPIO 4   (optional - lets the ESP reset the sensor)
    GPIO1 -> not connected (interrupt pin, not needed here)

  Open the Serial Monitor at 115200 baud to see readings.
*/

#include <Arduino.h>
#include <Wire.h>
#include <VL53L0X.h>

#define SDA_PIN   21
#define SCL_PIN   22
#define XSHUT_PIN 4

VL53L0X sensor;

void setup() {
  Serial.begin(115200);
  delay(500);
  Serial.println("\nVL53L0X distance test");

  // Reset the sensor via XSHUT so it starts in a known state
  pinMode(XSHUT_PIN, OUTPUT);
  digitalWrite(XSHUT_PIN, LOW);
  delay(10);
  digitalWrite(XSHUT_PIN, HIGH);
  delay(10);

  Wire.begin(SDA_PIN, SCL_PIN);

  sensor.setTimeout(500);
  if (!sensor.init()) {
    Serial.println("Sensor not found - check wiring (SDA/SCL swapped? VIN on 3V3?)");
    while (true) delay(1000);
  }

  // Optional: uncomment for longer range (up to ~2 m) at the cost of accuracy
  // sensor.setSignalRateLimit(0.1);
  // sensor.setVcselPulsePeriod(VL53L0X::VcselPeriodPreRange, 18);
  // sensor.setVcselPulsePeriod(VL53L0X::VcselPeriodFinalRange, 14);

  sensor.startContinuous();   // take readings back-to-back
  Serial.println("Sensor ready.");
}

void loop() {
  uint16_t mm = sensor.readRangeContinuousMillimeters();

  if (sensor.timeoutOccurred()) {
    Serial.println("Timeout");
  } else if (mm >= 8190) {
    Serial.println("Out of range");
  } else {
    Serial.printf("Distance: %u mm  (%.1f cm)\n", mm, mm / 10.0);
  }

  delay(100);
}
