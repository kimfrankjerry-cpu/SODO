#define TINY_GSM_MODEM_SIM800

#include <Arduino.h>
#include <TinyGsmClient.h>
#include <ArduinoHttpClient.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Keypad.h>
#include <ESP32Servo.h>
#include "config.h"

// --- Global Objects ---
Adafruit_SSD1306 display(128, 64, &Wire, -1);
Servo dispenserServo;

// GSM Modem setup
HardwareSerial SerialAT(2); // Use UART2
TinyGsm modem(SerialAT);
// For HTTPS, we must use Secure client. (Note: SIM800L requires good power for HTTPS)
TinyGsmClientSecure gsmClient(modem); 

// Backend configuration (Extract host from BACKEND_URL in a real app, hardcoding for SIM800L simplicity)
const char server[] = "f766028488640e.lhr.life"; 
const int  port = 443;
HttpClient http(gsmClient, server, port);

// Keypad setup
const byte ROWS = 4; 
const byte COLS = 4; 
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {KP_ROW1, KP_ROW2, KP_ROW3, KP_ROW4};
byte colPins[COLS] = {KP_COL1, KP_COL2, KP_COL3, KP_COL4};
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// --- State Machine ---
enum State {
  STATE_IDLE,
  STATE_KEYPAD_INPUT,
  STATE_STK_SENT,
  STATE_AWAITING_PAYMENT,
  STATE_DISPENSING,
  STATE_AWAITING_PAD_REMOVAL,
  STATE_STOCK_CHECK,
  STATE_OUT_OF_STOCK,
  STATE_OFFLINE
};

State currentState = STATE_IDLE;

String phoneNumber = "";
String currentCheckoutId = "";
unsigned long stateTimer = 0;
unsigned long pollTimer = 0;

// --- Helper Functions ---
void setLEDs(bool red, bool amber, bool green) {
  digitalWrite(LED_RED_PIN, red ? HIGH : LOW);
  digitalWrite(LED_AMBER_PIN, amber ? HIGH : LOW);
  digitalWrite(LED_GREEN_PIN, green ? HIGH : LOW);
}

void beep(int ms) {
  digitalWrite(BUZZER_PIN, HIGH);
  delay(ms);
  digitalWrite(BUZZER_PIN, LOW);
}

void showMessage(String title, String line1, String line2 = "") {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println(title);
  display.drawLine(0, 10, 128, 10, SSD1306_WHITE);
  
  display.setCursor(0, 20);
  display.println(line1);
  if (line2 != "") {
    display.setCursor(0, 35);
    display.println(line2);
  }
  display.display();
}

int getStackDistanceMm() {
  digitalWrite(HC_TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(HC_TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(HC_TRIG_PIN, LOW);
  long duration = pulseIn(HC_ECHO_PIN, HIGH, 30000); 
  if (duration == 0) return 999; 
  return duration * 0.034 / 2;
}

// --- GSM Network Functions ---
void connectGSM() {
  showMessage("CELLULAR", "Init Modem...", "Please wait.");
  
  // Restart Modem
  SerialAT.begin(9600, SERIAL_8N1, MODEM_RX_PIN, MODEM_TX_PIN);
  delay(3000);
  
  if (!modem.restart()) {
    Serial.println("Failed to restart modem");
    currentState = STATE_OFFLINE;
    return;
  }
  
  showMessage("CELLULAR", "Connecting to", "Network...");
  if (!modem.waitForNetwork(60000L)) {
    Serial.println("Network connection failed");
    currentState = STATE_OFFLINE;
    return;
  }
  
  showMessage("CELLULAR", "Connecting to", "GPRS (Internet)...");
  if (!modem.gprsConnect(GSM_APN, GSM_USER, GSM_PASS)) {
    Serial.println("GPRS connection failed");
    currentState = STATE_OFFLINE;
    return;
  }


  showMessage("CELLULAR", "Connected!", "GPRS Ready.");
  delay(2000);
  currentState = STATE_STOCK_CHECK;
}

bool sendSTKPush(String phone) {
  if (!modem.isGprsConnected()) return false;
  
  if (phone.startsWith("0")) {
    phone = "254" + phone.substring(1);
  }

  String payload = "{\"phone\":\"" + phone + "\"}";
  
  http.beginRequest();
  http.post("/mpesa/initiate");
  http.sendHeader("Content-Type", "application/json");
  http.sendHeader("Content-Length", payload.length());
  http.beginBody();
  http.print(payload);
  http.endRequest();
  
  int httpCode = http.responseStatusCode();
  if (httpCode == 200) {
    String response = http.responseBody();
    JsonDocument doc;
    deserializeJson(doc, response);
    if (doc["success"]) {
      currentCheckoutId = doc["checkoutRequestId"].as<String>();
      return true;
    }
  }
  
  return false;
}

String checkPaymentStatus() {
  if (!modem.isGprsConnected()) return "FAILED";
  
  String path = "/payment/status/" + currentCheckoutId;
  http.get(path);
  
  int httpCode = http.responseStatusCode();
  String status = "PENDING";
  
  if (httpCode == 200) {
    String response = http.responseBody();
    JsonDocument doc;
    deserializeJson(doc, response);
    status = doc["status"].as<String>();
  }
  
  return status;
}

// --- Setup ---
void setup() {
  Serial.begin(115200);
  
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_RED_PIN, OUTPUT);
  pinMode(LED_AMBER_PIN, OUTPUT);
  pinMode(LED_GREEN_PIN, OUTPUT);
  
  pinMode(IR_TX_PIN, OUTPUT);
  pinMode(IR_RX_PIN, INPUT);
  digitalWrite(IR_TX_PIN, HIGH);
  
  pinMode(HC_TRIG_PIN, OUTPUT);
  pinMode(HC_ECHO_PIN, INPUT);
  
  dispenserServo.attach(SERVO_PIN);
  dispenserServo.write(0);
  
  Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED init failed");
    while (1);
  }
  
  connectGSM();
}

// --- Main Loop ---
void loop() {
  if (!modem.isNetworkConnected() && currentState != STATE_OFFLINE) {
    currentState = STATE_OFFLINE;
  }

  switch (currentState) {
    case STATE_IDLE: {
      setLEDs(true, false, false);
      showMessage("PAD DISPENSER", "Price: KES " + String(PAD_PRICE_KES), "Press any key to buy");
      
      char key = keypad.getKey();
      if (key) {
        phoneNumber = "";
        phoneNumber += key;
        stateTimer = millis();
        currentState = STATE_KEYPAD_INPUT;
        beep(100);
      }
      break;
    }
    
    case STATE_KEYPAD_INPUT: {
      setLEDs(true, false, false);
      showMessage("ENTER PHONE", phoneNumber, "Press # to confirm");
      
      char key = keypad.getKey();
      if (key) {
        stateTimer = millis();
        beep(100);
        if (key == '#') {
          if (phoneNumber.length() >= 10) {
            currentState = STATE_STK_SENT;
          } else {
            showMessage("ERROR", "Number too short!", "Must be 10 digits");
            delay(2000);
          }
        } else if (key == '*') {
          if (phoneNumber.length() > 0) {
            phoneNumber = phoneNumber.substring(0, phoneNumber.length() - 1);
          }
        } else if (phoneNumber.length() < 12) {
          phoneNumber += key;
        }
      }
      
      if (millis() - stateTimer > 30000) {
        currentState = STATE_IDLE;
      }
      break;
    }
    
    case STATE_STK_SENT: {
      setLEDs(false, true, false);
      showMessage("PLEASE WAIT", "Sending M-PESA prompt", "to " + phoneNumber + "...");
      
      if (sendSTKPush(phoneNumber)) {
        stateTimer = millis();
        pollTimer = millis();
        currentState = STATE_AWAITING_PAYMENT;
      } else {
        showMessage("ERROR", "Network error.", "Please try again.");
        beep(500);
        delay(3000);
        currentState = STATE_IDLE;
      }
      break;
    }
    
    case STATE_AWAITING_PAYMENT: {
      setLEDs(false, (millis() / 500) % 2 == 0, false);
      
      long timeLeft = (PAYMENT_TIMEOUT - (millis() - stateTimer)) / 1000;
      showMessage("AWAITING PAYMENT", "Enter PIN on phone", "Timeout in: " + String(timeLeft) + "s");
      
      if (millis() - pollTimer > POLL_INTERVAL) {
        pollTimer = millis();
        String status = checkPaymentStatus();
        
        if (status == "CONFIRMED") {
          currentState = STATE_DISPENSING;
        } else if (status == "FAILED") {
          showMessage("PAYMENT FAILED", "Transaction cancelled", "or failed.");
          beep(1000);
          delay(4000);
          currentState = STATE_IDLE;
        }
      }
      
      if (millis() - stateTimer > PAYMENT_TIMEOUT) {
        showMessage("TIMEOUT", "Payment not received.", "Please try again.");
        beep(1000);
        delay(4000);
        currentState = STATE_IDLE;
      }
      break;
    }
    
    case STATE_DISPENSING: {
      setLEDs(false, false, true);
      showMessage("PAID!", "Dispensing your pad...", "Please wait.");
      beep(100); delay(100); beep(100);
      
      dispenserServo.write(90);
      delay(MOTOR_90_MS);
      dispenserServo.write(0);
      delay(MOTOR_90_MS);
      
      stateTimer = millis();
      currentState = STATE_AWAITING_PAD_REMOVAL;
      break;
    }
    
    case STATE_AWAITING_PAD_REMOVAL: {
      setLEDs(false, false, (millis() / 500) % 2 == 0);
      showMessage("READY", "Please take your pad", "from the slot below.");
      
      bool padTaken = digitalRead(IR_RX_PIN) == HIGH;
      
      if (padTaken) {
        delay(2000);
        currentState = STATE_STOCK_CHECK;
      } else if (millis() - stateTimer > 30000) {
        currentState = STATE_STOCK_CHECK;
      }
      break;
    }
    
    case STATE_STOCK_CHECK: {
      int dist = getStackDistanceMm();
      if (dist > MIN_STOCK_MM) {
        currentState = STATE_OUT_OF_STOCK;
      } else {
        currentState = STATE_IDLE;
      }
      break;
    }
    
    case STATE_OUT_OF_STOCK: {
      setLEDs(true, false, false);
      showMessage("OUT OF STOCK", "Please check back", "later.");
      
      if (millis() - pollTimer > 5000) {
        pollTimer = millis();
        if (getStackDistanceMm() <= MIN_STOCK_MM) {
          currentState = STATE_IDLE;
        }
      }
      break;
    }
    
    case STATE_OFFLINE: {
      setLEDs(true, true, false);
      showMessage("OFFLINE", "No Cellular Network", "Attempting reconnect...");
      
      if (millis() - pollTimer > 15000) {
        pollTimer = millis();
        connectGSM(); 
      }
      break;
    }
  }
}
