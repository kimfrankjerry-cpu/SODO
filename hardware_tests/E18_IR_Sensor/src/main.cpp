/*
  ESP32 + E18-D80NK infrared obstacle sensor

  Wiring (E18-D80NK -> ESP32):
    Brown (VCC)    -> 5V / VIN
    Blue  (GND)    -> GND
    Black (signal) -> GPIO 27  (direct if the wire idles at ~0 V,
                                via 10k/20k divider if it idles at ~5 V)

  Detection level is set by DETECT_LEVEL below. Adjust range with the screw
  on the back of the sensor. Open the Serial Monitor at 115200 baud.
*/

#include <Arduino.h>

#define IR_PIN  27
#define LED_PIN 2   // onboard LED on most ESP32 DevKit boards

// Signal level that means "object detected". Most E18-D80NK units pull LOW,
// but this one reads HIGH when an object is present.
#define DETECT_LEVEL HIGH

int lastState = -1;
unsigned long lastPrint = 0;

void setup() {
  Serial.begin(115200);
  delay(500);
  Serial.println("\nE18-D80NK IR sensor test");

  pinMode(IR_PIN, INPUT_PULLUP);   // open-collector output needs a pull-up
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  int state = digitalRead(IR_PIN);
  bool detected = (state == DETECT_LEVEL);

  digitalWrite(LED_PIN, detected ? HIGH : LOW);

  // Print on every change, plus a status line once a second
  if (state != lastState || millis() - lastPrint >= 1000) {
    Serial.println(detected ? "Object detected" : "Clear");
    lastState = state;
    lastPrint = millis();
  }

  delay(20);
}
