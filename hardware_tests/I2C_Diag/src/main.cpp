// I2C / VL53L0X wiring diagnostic for ESP32
#include <Arduino.h>
#include <Wire.h>

const int PIN_A = 21, PIN_B = 22, XSHUT = 4;

void lineLevels(const char *label) {
  // With internal pulldown, a line only reads HIGH if an external pull-up
  // (on the powered sensor breakout) is connected to it.
  pinMode(PIN_A, INPUT_PULLDOWN); pinMode(PIN_B, INPUT_PULLDOWN); pinMode(XSHUT, INPUT_PULLDOWN);
  delay(5);
  int a = digitalRead(PIN_A), b = digitalRead(PIN_B), x = digitalRead(XSHUT);
  pinMode(PIN_A, INPUT_PULLUP); pinMode(PIN_B, INPUT_PULLUP);
  delay(5);
  int au = digitalRead(PIN_A), bu = digitalRead(PIN_B);
  Serial.printf("[%s] pulldown: GPIO21=%d GPIO22=%d GPIO4(XSHUT)=%d | pullup: GPIO21=%d GPIO22=%d\n",
                label, a, b, x, au, bu);
  pinMode(PIN_A, INPUT); pinMode(PIN_B, INPUT); pinMode(XSHUT, INPUT);
}

int scan(int sda, int scl, uint32_t freq, const char *label) {
  Wire.end();
  Wire.begin(sda, scl, freq);
  Wire.setTimeOut(50);
  int found = 0;
  Serial.printf("  scan %-28s SDA=%d SCL=%d %6lu Hz : ", label, sda, scl, (unsigned long)freq);
  for (uint8_t addr = 1; addr < 127; addr++) {
    Wire.beginTransmission(addr);
    if (Wire.endTransmission() == 0) { Serial.printf("0x%02X ", addr); found++; }
  }
  Serial.println(found ? "" : "(nothing)");
  return found;
}

void readModelId(int sda, int scl) {
  Wire.end();
  Wire.begin(sda, scl, 100000);
  Wire.beginTransmission(0x29); Wire.write(0xC0); Wire.endTransmission(false);
  if (Wire.requestFrom(0x29, 2) == 2) {
    uint8_t id = Wire.read(), type = Wire.read();
    Serial.printf("  0x29 model ID reg 0xC0 = 0x%02X (VL53L0X = 0xEE), 0xC1 = 0x%02X (expect 0xAA)\n", id, type);
  } else {
    Serial.println("  could not read model ID");
  }
}

void runAll() {
  Serial.println("\n===== I2C DIAGNOSTIC =====");

  lineLevels("XSHUT released");

  Serial.println("XSHUT released (floating, breakout pull-up keeps sensor on):");
  pinMode(XSHUT, INPUT); delay(20);
  int f1 = scan(21, 22, 100000, "normal 100k");
  int f2 = scan(21, 22, 10000,  "normal 10k (slow)");
  int f3 = scan(22, 21, 100000, "SWAPPED 100k");
  int f4 = scan(22, 21, 10000,  "SWAPPED 10k (slow)");

  Serial.println("XSHUT driven HIGH after reset pulse (as your sketch does):");
  pinMode(XSHUT, OUTPUT); digitalWrite(XSHUT, LOW); delay(10); digitalWrite(XSHUT, HIGH); delay(20);
  int f5 = scan(21, 22, 100000, "normal 100k");
  int f6 = scan(22, 21, 100000, "SWAPPED 100k");
  int x  = digitalRead(XSHUT);
  Serial.printf("  GPIO4 readback while driven HIGH = %d\n", x);
  pinMode(XSHUT, INPUT);

  if (f1 || f2 || f5) readModelId(21, 22);
  else if (f3 || f4 || f6) readModelId(22, 21);

  Serial.println("===== END =====");
}

void setup() {
  Serial.begin(115200);
  delay(500);
}

void loop() {
  runAll();
  delay(5000);
}
