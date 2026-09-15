# Smart Pad Dispenser — Payment Integration Spec

> **Payment Method**: M-PESA STK Push (Safaricom Daraja API)
> **Flow**: User enters phone number on device keypad → Backend sends STK Push → User enters PIN → Confirmation relayed to ESP32 → Pad dispenses

---

## 1. M-PESA Flow Diagram

```
USER PHONE              ESP32 (Device)           BACKEND SERVER            SAFARICOM
     │                        │                        │                       │
     │   approaches device    │                        │                       │
     │◀──display: price+──────│                        │                       │
     │   "Enter phone no."    │                        │                       │
     │──enters 07XXXXXXXX────▶│                        │                       │
     │                        │──POST /initiate───────▶│                       │
     │                        │   {phone, amount}      │──STK Push API POST───▶│
     │                        │                        │                       │──PIN prompt──▶ USER
     │──enters M-PESA PIN─────────────────────────────────────────────────────▶│
     │                        │                        │◀──callback (SUCCESS)──│
     │                        │◀──MQTT: PAYMENT_OK─────│                       │
     │                        │  DISPENSE PAD          │                       │
     │◀──pad exits────────────│                        │                       │
     │                        │──POST /log_dispense───▶│                       │
```

---

## 2. Safaricom Daraja API Setup

### 2.1 Requirements
- [ ] Safaricom Business account with registered Till Number OR Paybill
- [ ] Daraja developer account: [developer.safaricom.co.ke](https://developer.safaricom.co.ke)
- [ ] Create an app on Daraja → obtain:
  - `Consumer Key`
  - `Consumer Secret`
  - `Business Short Code` (Till or Paybill number)
  - `Passkey` (for STK Push)

### 2.2 API Credentials (fill in after registration)

| Parameter | Sandbox Value | Production Value |
|-----------|--------------|-----------------|
| Consumer Key | `[SANDBOX KEY]` | `[PROD KEY]` |
| Consumer Secret | `[SANDBOX SECRET]` | `[PROD SECRET]` |
| Business Short Code | `174379` (sandbox) | `[YOUR TILL/PAYBILL]` |
| Passkey | `[SANDBOX PASSKEY]` | `[PROD PASSKEY]` |
| Callback URL | `https://[your-server]/mpesa/callback` | Same |
| STK Push URL | `https://sandbox.safaricom.co.ke/...` | `https://api.safaricom.co.ke/...` |

### 2.3 STK Push Request (Backend sends this to Safaricom)

```json
POST https://sandbox.safaricom.co.ke/mpesa/stkpush/v1/processrequest

{
  "BusinessShortCode": "174379",
  "Password": "<base64(ShortCode + Passkey + Timestamp)>",
  "Timestamp": "20241001120000",
  "TransactionType": "CustomerBuyGoodsOnline",
  "Amount": 10,
  "PartyA": "2547XXXXXXXX",
  "PartyB": "174379",
  "PhoneNumber": "2547XXXXXXXX",
  "CallBackURL": "https://your-server.com/mpesa/callback",
  "AccountReference": "PAD001",
  "TransactionDesc": "Sanitary Pad"
}
```

> **Note**: Phone number format must be `2547XXXXXXXX` (drop leading 0, add 254).
> The ESP32 should strip the leading `0` and prepend `254` before sending to backend.

### 2.4 Callback Response (Safaricom sends this to Backend)

**Success**:
```json
{
  "Body": {
    "stkCallback": {
      "MerchantRequestID": "...",
      "CheckoutRequestID": "...",
      "ResultCode": 0,
      "ResultDesc": "The service request is processed successfully.",
      "CallbackMetadata": {
        "Item": [
          { "Name": "Amount", "Value": 10 },
          { "Name": "MpesaReceiptNumber", "Value": "NLJ7RT61SV" },
          { "Name": "PhoneNumber", "Value": 254712345678 }
        ]
      }
    }
  }
}
```

**Failure** (user cancelled or wrong PIN):
```json
{
  "Body": {
    "stkCallback": {
      "ResultCode": 1032,
      "ResultDesc": "Request cancelled by user"
    }
  }
}
```

---

## 3. Backend Server Specification

### 3.1 Hosting
| Option | Cost | Recommendation |
|--------|------|---------------|
| Railway (free tier) | KES 0 | ✅ Start here |
| Render (free tier) | KES 0 | Alternative |
| DigitalOcean VPS (1 GB) | ~KES 600/mo | Production-grade |

**Requirement**: Server must have a valid HTTPS certificate (free via Let's Encrypt / Railway auto-TLS).

### 3.2 Endpoints Required

| Method | Path | Description |
|--------|------|-------------|
| `POST` | `/mpesa/initiate` | ESP32 calls this to start STK Push. Body: `{phone, amount}` |
| `POST` | `/mpesa/callback` | Safaricom calls this with payment result |
| `GET` | `/payment/status/{checkoutRequestId}` | ESP32 polls this to check if payment confirmed |
| `POST` | `/dispense/log` | ESP32 logs successful dispense |
| `GET` | `/admin/inventory` | Admin checks stock level |
| `GET` | `/admin/transactions` | Admin views transaction history |

### 3.3 ESP32 ↔ Backend Communication Options

| Method | Pros | Cons |
|--------|------|------|
| **HTTP Polling** (recommended for v1) | Simple; no persistent connection needed | ~2–5 s delay |
| MQTT (PubSub) | Real-time push to ESP32 | Requires MQTT broker |
| WebSocket | Real-time, no broker | More complex on ESP32 |

**v1 recommendation**: ESP32 polls `GET /payment/status/{id}` every 2 seconds with a 60-second timeout.

### 3.4 Transaction State Flow (Backend)

```
PENDING → PROCESSING → CONFIRMED → DISPENSED
    └──────────────────→ FAILED (timeout, cancelled, wrong PIN)
```

---

## 4. Backend Stack Recommendation

### Option A — Node.js (Express)
```
server/
├── index.js            # Express app
├── routes/
│   ├── mpesa.js        # /mpesa/initiate + /mpesa/callback
│   ├── payment.js      # /payment/status
│   └── admin.js        # /admin/* routes
├── db/
│   └── transactions.js # SQLite via better-sqlite3
├── services/
│   └── daraja.js       # Safaricom API client
└── .env                # CONSUMER_KEY, SECRET, PASSKEY (never commit)
```

### Option B — Python (FastAPI)
```
server/
├── main.py
├── routers/
│   ├── mpesa.py
│   └── admin.py
├── services/
│   └── daraja.py
└── database.py         # SQLite via SQLAlchemy
```

---

## 5. ESP32 Integration

### 5.1 Libraries Required (PlatformIO / Arduino)
```ini
; platformio.ini
[env:esp32dev]
platform = espressif32
board = esp32dev
framework = arduino
lib_deps =
    bblanchon/ArduinoJson       ; JSON parsing for API responses
    knolleary/PubSubClient       ; MQTT (if used)
    adafruit/Adafruit SSD1306   ; OLED display
    adafruit/Adafruit GFX Library
    plerup/EspSoftwareSerial    ; SIM800L communication
```

### 5.2 ESP32 Pin Assignment (Preliminary)

| Pin | Function | Component |
|-----|----------|-----------|
| GPIO 21 | SDA | OLED (I2C) |
| GPIO 22 | SCL | OLED (I2C) |
| GPIO 16 | RX | SIM800L TX |
| GPIO 17 | TX | SIM800L RX |
| GPIO 4 | Motor PWM | Motor driver IN1 |
| GPIO 5 | Motor DIR | Motor driver IN2 |
| GPIO 18 | IR TX power | IR break-beam |
| GPIO 19 | IR RX signal | IR break-beam |
| GPIO 23 | HC-SR04 TRIG | Ultrasonic |
| GPIO 25 | HC-SR04 ECHO | Ultrasonic |
| GPIO 26 | Buzzer | Passive buzzer |
| GPIO 27 | LED Red | Status |
| GPIO 32 | LED Green | Status |
| GPIO 33 | LED Amber | Status |
| GPIO 34–39 | Keypad rows/cols | 4×4 matrix (8 pins) |

> ⚠️ Verify pin availability and conflicts before wiring. Document final pinout in `firmware_spec.md`.

---

## 6. Security Considerations

| Risk | Mitigation |
|------|-----------|
| Replay attack (reuse old payment callback) | Validate `MpesaReceiptNumber` is unique; store in DB |
| Overpayment | Only validate amount ≥ price; refund policy documented |
| Underpayment | Reject if amount < price; do not dispense |
| Callback URL spoofing | Validate Safaricom IP whitelist + HMAC if available |
| Credential exposure | Store keys in `.env`; never commit to Git |
| Double-dispense on retry | Transaction state machine prevents re-dispensing confirmed payments |

---

## 7. Testing Plan

### 7.1 Sandbox Testing (Daraja)
- [ ] STK Push initiated successfully (Safaricom sandbox)
- [ ] Success callback received and parsed
- [ ] Failure callback (code 1032) handled correctly
- [ ] Timeout (60 s) handled — display "Payment not received. Please try again."
- [ ] Amount validation (under/over payment) handled

### 7.2 End-to-End Device Test
- [ ] Phone number entered on keypad → formatted to `254...` correctly
- [ ] Backend receives initiate request and calls Daraja
- [ ] ESP32 receives payment confirmation within 10 s of PIN entry
- [ ] Pad dispenses immediately on confirmation
- [ ] Transaction logged on backend

---

## 8. Revision History

| Version | Date | Notes |
|---------|------|-------|
| v0.1 | `[DATE]` | Initial spec — STK Push, backend, ESP32 integration |
