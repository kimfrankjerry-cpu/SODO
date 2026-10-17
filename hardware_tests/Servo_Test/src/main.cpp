/*
  ESP32 + MG946R / MG996R 360-degree servo: 3 clockwise turns per trigger

  Wiring (servo -> ESP32 / supply):
    Brown  (GND)    -> external supply GND  AND  ESP32 GND (common ground!)
    Red    (V+)     -> external 5-6 V supply, 2 A or more
    Orange (signal) -> GPIO 26

  A 360 servo has no position feedback, so "one rotation" is timed: it spins
  at TURN_SPEED for TURN_MS, then stops. Calibrate TURN_MS once (see below).
  Accuracy drifts with supply voltage and load.

  Trigger a turn: press the BOOT button, or type "r" + Enter.
  Calibrate from the Serial Monitor (115200 baud), no re-upload needed:
    t1200  set turn time to 1200 ms      f  flip direction (if not clockwise)
    p      print current settings
  Then copy the printed values into the #defines below to make them permanent.
*/

#include <Arduino.h>
#include <ESP32Servo.h>

#define SERVO_PIN  26
#define BUTTON_PIN 0     // BOOT button on ESP32 DevKit, LOW when pressed

#define STOP_US    1500  // pulse that stops the servo
#define RANGE_US   500   // full speed = STOP_US +/- RANGE_US
#define TURN_SPEED 50    // percent of full speed used for a turn
// Calibrated 2026-10-17 at 50% speed (5-6 V supply, no load):
//   5320 ms -> ~630 deg, 6080 ms -> ~715 deg (2 turns), 9300 ms -> 3 turns.
// The servo coasts slightly after stopping, so time does not scale exactly
// linearly with the number of turns.
#define TURN_MS    9300  // run time per trigger: 3 full turns
#define CW_SIGN    -1    // 1 or -1: which pulse direction is clockwise

Servo servo;
unsigned long turnMs = TURN_MS;
int cwSign = CW_SIGN;

void setSpeed(int speed) {
  servo.writeMicroseconds(STOP_US + speed * RANGE_US / 100);
}

void printSettings() {
  Serial.printf("TURN_MS %lu, CW_SIGN %d, TURN_SPEED %d%%\n",
                turnMs, cwSign, TURN_SPEED);
}

void oneTurn() {
  Serial.printf("Turning clockwise (%lu ms)... ", turnMs);
  setSpeed(cwSign * TURN_SPEED);
  delay(turnMs);
  setSpeed(0);
  Serial.println("stopped");
}

void setup() {
  Serial.begin(115200);
  delay(500);
  Serial.println("\n360 servo: one turn per trigger");

  pinMode(BUTTON_PIN, INPUT_PULLUP);
  servo.setPeriodHertz(50);                 // standard 50 Hz servo signal
  servo.attach(SERVO_PIN, STOP_US - RANGE_US, STOP_US + RANGE_US);
  setSpeed(0);

  printSettings();
  Serial.println("Press BOOT or type 'r' to turn. 't<ms>' sets time, 'f' flips.");
}

void loop() {
  if (digitalRead(BUTTON_PIN) == LOW) {
    delay(30);                              // debounce
    if (digitalRead(BUTTON_PIN) == LOW) {
      oneTurn();
      while (digitalRead(BUTTON_PIN) == LOW) delay(10);  // wait for release
    }
  }

  if (Serial.available()) {
    String in = Serial.readStringUntil('\n');
    in.trim();
    if (in.equalsIgnoreCase("r")) {
      oneTurn();
    } else if (in.equalsIgnoreCase("f")) {
      cwSign = -cwSign;
      printSettings();
    } else if (in.equalsIgnoreCase("p")) {
      printSettings();
    } else if (in.length() > 1 && (in[0] == 't' || in[0] == 'T')) {
      long ms = in.substring(1).toInt();
      if (ms > 0) turnMs = ms;
      printSettings();
    }
  }
}
