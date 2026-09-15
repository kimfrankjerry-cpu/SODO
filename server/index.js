const express = require('express');
const axios = require('axios');
const dotenv = require('dotenv');
const Database = require('better-sqlite3');
const rateLimit = require('express-rate-limit');

dotenv.config();

const app = express();
app.use(express.json());

// Initialize SQLite Database
const db = new Database('transactions.db');
db.exec(`
  CREATE TABLE IF NOT EXISTS transactions (
    checkoutRequestId TEXT PRIMARY KEY,
    phone TEXT,
    amount INTEGER,
    status TEXT,
    receipt TEXT,
    timestamp DATETIME DEFAULT CURRENT_TIMESTAMP
  )
`);

// Helper: Get Safaricom OAuth Token
async function getAuthToken() {
  const env = process.env.DARAJA_ENVIRONMENT === 'production' ? 'api' : 'sandbox';
  const url = `https://${env}.safaricom.co.ke/oauth/v1/generate?grant_type=client_credentials`;
  const auth = Buffer.from(`${process.env.DARAJA_CONSUMER_KEY}:${process.env.DARAJA_CONSUMER_SECRET}`).toString('base64');

  const response = await axios.get(url, {
    headers: { Authorization: `Basic ${auth}` }
  });
  return response.data.access_token;
}

// Rate Limiter for STK Push (Max 3 requests per minute per IP)
const stkLimiter = rateLimit({
  windowMs: 1 * 60 * 1000,
  max: 3,
  message: { error: "Too many payment requests. Please wait a minute." }
});

// 1. Initiate STK Push (ESP32 calls this)
app.post('/mpesa/initiate', stkLimiter, async (req, res) => {
  const { phone } = req.body;
  const amount = process.env.PAD_PRICE_KES; // HARDCODED SERVER PRICE (Fixes Price Tampering)
  
  if (!phone) {
    return res.status(400).json({ error: "Missing phone number" });
  }

  try {
    const token = await getAuthToken();
    const env = process.env.DARAJA_ENVIRONMENT === 'production' ? 'api' : 'sandbox';
    const url = `https://${env}.safaricom.co.ke/mpesa/stkpush/v1/processrequest`;
    
    // Format timestamp: YYYYMMDDHHmmss
    const timestamp = new Date().toISOString().replace(/[^0-9]/g, '').slice(0, 14);
    const passkey = process.env.DARAJA_PASSKEY;
    const shortcode = process.env.DARAJA_BUSINESS_SHORTCODE;
    const password = Buffer.from(`${shortcode}${passkey}${timestamp}`).toString('base64');

    const payload = {
      BusinessShortCode: shortcode,
      Password: password,
      Timestamp: timestamp,
      TransactionType: "CustomerPayBillOnline",
      Amount: amount,
      PartyA: phone,
      PartyB: shortcode,
      PhoneNumber: phone,
      CallBackURL: `${process.env.CALLBACK_URL}/mpesa/callback?token=${process.env.CALLBACK_TOKEN}`,
      AccountReference: "SODO Dispenser",
      TransactionDesc: "Sanitary Pad Payment"
    };

    const response = await axios.post(url, payload, {
      headers: { Authorization: `Bearer ${token}` }
    });

    const checkoutRequestId = response.data.CheckoutRequestID;

    // Save pending transaction to DB
    const insert = db.prepare('INSERT INTO transactions (checkoutRequestId, phone, amount, status) VALUES (?, ?, ?, ?)');
    insert.run(checkoutRequestId, phone, amount, 'PENDING');

    res.json({ success: true, checkoutRequestId, message: "STK push sent" });

  } catch (error) {
    console.error("STK Push Error:", error.response ? error.response.data : error.message);
    res.status(500).json({ error: "Failed to initiate payment" });
  }
});

// 2. Safaricom Callback (Safaricom calls this)
app.post('/mpesa/callback', (req, res) => {
  // Validate Security Token to prevent Callback Spoofing
  if (req.query.token !== process.env.CALLBACK_TOKEN) {
    console.warn("BLOCKED SPOOFED CALLBACK ATTEMPT!");
    return res.status(403).send("Forbidden");
  }

  console.log("Received callback:", JSON.stringify(req.body, null, 2));
  
  const callbackData = req.body?.Body?.stkCallback;
  if (!callbackData) {
    return res.status(400).send("Invalid payload");
  }

  const checkoutRequestId = callbackData.CheckoutRequestID;
  const resultCode = callbackData.ResultCode;

  if (resultCode === 0) {
    // Payment Successful
    const metadata = callbackData.CallbackMetadata.Item;
    const receipt = metadata.find(i => i.Name === 'MpesaReceiptNumber')?.Value;
    
    const update = db.prepare('UPDATE transactions SET status = ?, receipt = ? WHERE checkoutRequestId = ?');
    update.run('CONFIRMED', receipt, checkoutRequestId);
    console.log(`Payment confirmed! Receipt: ${receipt}`);
  } else {
    // Payment Failed or Cancelled
    const update = db.prepare('UPDATE transactions SET status = ? WHERE checkoutRequestId = ?');
    update.run('FAILED', checkoutRequestId);
    console.log(`Payment failed: ${callbackData.ResultDesc}`);
  }

  // Always return success to Safaricom so they stop retrying
  res.json({ success: true });
});

// 3. Check Payment Status (ESP32 polls this)
app.get('/payment/status/:id', (req, res) => {
  const checkoutRequestId = req.params.id;
  const row = db.prepare('SELECT status, receipt FROM transactions WHERE checkoutRequestId = ?').get(checkoutRequestId);
  
  if (!row) {
    return res.status(404).json({ error: "Transaction not found" });
  }

  res.json({ status: row.status, receipt: row.receipt });
});

const PORT = process.env.PORT || 3000;
app.listen(PORT, () => {
  console.log(`SODO Backend running on port ${PORT}`);
  console.log(`Make sure CALLBACK_URL is set to your public tunnel url in .env`);
});
