#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Keypad.h>
// Removed ESP32Servo to speed up Wokwi compilation

// --- Settings ---
#define PAD_PRICE_KES    2
#define PAYMENT_TIMEOUT  60000 
#define POLL_INTERVAL    2000  
#define MIN_STOCK_MM     15    
#define MOTOR_90_MS      1000  
#define BACKEND_URL      "https://25386f9daf6cdb.lhr.life"

// --- Pin Definitions ---
#define I2C_SDA_PIN      21
#define I2C_SCL_PIN      22
#define SERVO_PIN        4
#define IR_TX_PIN        18
#define IR_RX_PIN        19
#define HC_TRIG_PIN      23
#define HC_ECHO_PIN      25
#define BUZZER_PIN       26
#define LED_RED_PIN      27
#define LED_GREEN_PIN    32
#define LED_AMBER_PIN    33

#define KP_ROW1          34
#define KP_ROW2          35
#define KP_ROW3          36
#define KP_ROW4          39
#define KP_COL1          13
#define KP_COL2          14
#define KP_COL3          15
#define KP_COL4          12  

// --- Global Objects ---
Adafruit_SSD1306 display(128, 64, &Wire, -1);

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

enum State {
  STATE_IDLE,
  STATE_KEYPAD_INPUT,
  STATE_STK_SENT,
  STATE_AWAITING_PAYMENT,
  STATE_DISPENSING,
  STATE_AWAITING_PAD_REMOVAL,
  STATE_STOCK_CHECK,
  STATE_OUT_OF_STOCK,
  STATE_OFFLINE,
  STATE_ERROR
};

State currentState = STATE_IDLE;
String phoneNumber = "";
String currentCheckoutId = "";
unsigned long stateTimer = 0;
unsigned long pollTimer = 0;
int dispenseRetries = 0;

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

// --- Wokwi Wi-Fi Network Functions ---
void connectWiFi() {
  showMessage("WIFI", "Connecting to", "Wokwi-GUEST");
  // Wokwi's virtual AP has no password
  WiFi.begin("Wokwi-GUEST", "", 6); 
  
  int retries = 0;
  while (WiFi.status() != WL_CONNECTED && retries < 15) {
    delay(500);
    retries++;
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    showMessage("WIFI", "Connected!", WiFi.localIP().toString());
    delay(2000);
    currentState = STATE_STOCK_CHECK;
  } else {
    currentState = STATE_OFFLINE;
  }
}

bool sendSTKPush(String phone) {
  if (WiFi.status() != WL_CONNECTED) return false;
  
  HTTPClient http;
  String url = String(BACKEND_URL) + "/mpesa/initiate";
  http.begin(url);
  http.addHeader("Content-Type", "application/json");
  
  if (phone.startsWith("0")) phone = "254" + phone.substring(1);

  String payload = "{\"phone\":\"" + phone + "\"}";
  int httpCode = http.POST(payload);
  
  if (httpCode == 200) {
    String response = http.getString();
    JsonDocument doc;
    deserializeJson(doc, response);
    if (doc["success"]) {
      currentCheckoutId = doc["checkoutRequestId"].as<String>();
      http.end();
      return true;
    }
  }
  http.end();
  return false;
}

String checkPaymentStatus() {
  if (WiFi.status() != WL_CONNECTED) return "FAILED";
  
  HTTPClient http;
  String url = String(BACKEND_URL) + "/payment/status/" + currentCheckoutId;
  http.begin(url);
  
  int httpCode = http.GET();
  String status = "PENDING";
  
  if (httpCode == 200) {
    String response = http.getString();
    JsonDocument doc;
    deserializeJson(doc, response);
    status = doc["status"].as<String>();
  }
  
  http.end();
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
  pinMode(HC_ECHO_PIN, INPUT_PULLUP);
  
  // Init Servo via native ledc (removes heavy ESP32Servo library)
  ledcSetup(0, 50, 16); 
  ledcAttachPin(SERVO_PIN, 0);
  ledcWrite(0, 3276); // 0 degrees
  
  Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED init failed");
    while (1);
  }
  
  connectWiFi();
}

// --- Main Loop ---
void loop() {
  // Prevent network drops from interrupting an active dispense cycle
  if (WiFi.status() != WL_CONNECTED && 
      currentState != STATE_OFFLINE && 
      currentState != STATE_DISPENSING && 
      currentState != STATE_AWAITING_PAD_REMOVAL && 
      currentState != STATE_ERROR) {
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
      
      // 90 degrees = 4915, 0 degrees = 3276
      ledcWrite(0, 4915);
      delay(MOTOR_90_MS);
      ledcWrite(0, 3276);
      delay(MOTOR_90_MS);
      
      stateTimer = millis();
      currentState = STATE_AWAITING_PAD_REMOVAL;
      break;
    }
    
    case STATE_AWAITING_PAD_REMOVAL: {
      setLEDs(false, false, (millis() / 500) % 2 == 0);
      showMessage("READY", "Please take your pad", "from the slot below.");
      
      bool padTaken = digitalRead(IR_RX_PIN) == HIGH; // Note: Manually trigger IR pin in sim
      
      if (padTaken) {
        dispenseRetries = 0;
        delay(2000);
        currentState = STATE_STOCK_CHECK;
      } else if (millis() - stateTimer > 10000) { 
        if (dispenseRetries < 2) {
          dispenseRetries++;
          showMessage("RETRYING", "Pad may be stuck...", "Please wait.");
          beep(500); delay(1000);
          currentState = STATE_DISPENSING; // Spin motor again!
        } else {
          dispenseRetries = 0;
          currentState = STATE_ERROR;
        }
      }
      break;
    }
    
    case STATE_STOCK_CHECK: {
      int dist = getStackDistanceMm();
      if (dist > MIN_STOCK_MM && dist != 999) {
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
      showMessage("OFFLINE", "No Wi-Fi Network", "Attempting reconnect...");
      
      if (millis() - pollTimer > 15000) {
        pollTimer = millis();
        connectWiFi(); 
      }
      break;
    }
    
    case STATE_ERROR: {
      setLEDs(true, false, false);
      showMessage("SYSTEM ERROR", "Pad jammed!", "Contact admin.");
      
      if (millis() - pollTimer > 3000) {
        pollTimer = millis();
        beep(500); // Annoyingly beep to alert staff
      }
      break;
    }
  }
}
