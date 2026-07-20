# RFID Relay Control System

[![Arduino](https://img.shields.io/badge/Arduino-00979D?style=for-the-badge&logo=arduino&logoColor=white)](https://www.arduino.cc/)
[![License](https://img.shields.io/badge/License-All%20Rights%20Reserved-red.svg)](#-license)
[![Platform](https://img.shields.io/badge/platform-Arduino%20%7C%20ESP8266%20%7C%20ESP32-blue)](https://www.arduino.cc/)

An Arduino-based access control system that uses RFID tags to toggle a relay for controlling lights, doors, or other electronic devices.

---

## 📋 Table of Contents

- [Features](#-features)
- [Hardware Requirements](#-hardware-requirements)
- [Pin Connections](#-pin-connections)
- [Software Requirements](#-software-requirements)
- [Setup Instructions](#-setup-instructions)
- [Usage](#-usage)
- [Serial Output Examples](#-serial-output-examples)
- [Troubleshooting](#-troubleshooting)
- [Customization](#-customization)
- [Project Structure](#-project-structure)
- [Contributing](#-contributing)
- [License](#-license)
- [Disclaimer](#️-disclaimer)

---

## ✨ Features

| Feature | Description |
|---|---|
| 🚪 RFID Authentication | Only authorized RFID tags can trigger the relay |
| 🔄 Toggle Operation | Each authorized scan toggles the relay ON/OFF |
| 👥 Multi-User Support | Add multiple authorized RFID cards with custom names |
| ⚙️ Configurable Relay Type | Supports both Active LOW and Active HIGH relays |
| 🛡️ Debounce Protection | Prevents multiple triggers from a single scan |
| 📊 Serial Monitoring | Real-time feedback with clear access messages |
| 💾 Memory Efficient | Uses Flash memory for strings and compile-time constants |
| 🔌 Cross-Platform | Works with Arduino Uno, Nano, Mega, ESP8266, ESP32 |

---

## 📦 Hardware Requirements

| Component | Quantity | Specifications |
|---|---|---|
| Arduino Board | 1 | Uno, Nano, Mega, ESP32, or ESP8266 |
| MFRC522 RFID Module | 1 | 13.56 MHz |
| Relay Module | 1 | Active LOW or Active HIGH |
| RFID Tags / Cards | 1+ | 13.56 MHz compatible |
| Connecting Wires | As needed | Female-to-female / Male-to-female |

---

## 🔌 Pin Connections

### MFRC522 → Microcontroller

| MFRC522 Pin | Arduino Uno/Nano | Arduino Mega | ESP32 | ESP8266 |
|---|---|---|---|---|
| SDA (SS) | 5 | 5 | 5 | 5 |
| RST | 21 | 21 | 21 | 21 |
| SCK | 13 | 52 | 18 | 14 |
| MOSI | 11 | 51 | 23 | 13 |
| MISO | 12 | 50 | 19 | 12 |
| VCC | 3.3V | 3.3V | 3.3V | 3.3V |
| GND | GND | GND | GND | GND |

### Relay Module → Arduino

| Relay Pin | Arduino Pin |
|---|---|
| IN | 13 |
| VCC | 5V |
| GND | GND |

> ⚠️ **Note:** SPI pins vary by board. Always verify against your board's pinout diagram before wiring.

---

## 💻 Software Requirements

- [Arduino IDE](https://www.arduino.cc/en/software) v1.8.0 or newer
- **MFRC522 library** by Miguel Balboa (v1.4.10+)

### Installing the MFRC522 Library

1. Open Arduino IDE
2. Go to **Tools → Manage Libraries** (`Ctrl+Shift+I`)
3. Search for `MFRC522`
4. Install **MFRC522 by Miguel Balboa**
5. Wait for installation to complete

---

## 🚀 Setup Instructions

### Step 1 — Find Your RFID Tag UID

Upload the `DumpInfo` example sketch from the MFRC522 library. Open the Serial Monitor and scan your tag to get its UID:

```
Card UID: 52 93 0C 5C
Card type: MIFARE 1KB
```

### Step 2 — Configure Authorized Cards

Open the sketch and update the `AUTHORIZED_CARDS` array with your tag UIDs:

```cpp
constexpr AuthorizedCard AUTHORIZED_CARDS[] = {
    {{0x52, 0x93, 0x0C, 0x5C}, "User 1"},     // Replace with your UID
    {{0xAB, 0xCD, 0x12, 0x34}, "User 2"},     // Add more as needed
    // {{0xXX, 0xXX, 0xXX, 0xXX}, "User N"},
};

```

### Step 3 — Configure Relay Type

Set `RELAY_ACTIVE_LOW` to match your relay module:

```cpp
// Active LOW relay (LOW = ON, HIGH = OFF)
constexpr bool RELAY_ACTIVE_LOW = true;

// Active HIGH relay (HIGH = ON, LOW = OFF)
constexpr bool RELAY_ACTIVE_LOW = false;
```

### Step 4 — Update Pin Definitions

Adjust pin assignments to match your wiring if needed:

```cpp
constexpr int SS_PIN    = 5;   // MFRC522 SDA pin
constexpr int RST_PIN   = 21;  // MFRC522 RST pin
constexpr int RELAY_PIN = 13;  // Relay control pin

```

### Step 5 — Upload the Code

1. Connect your Arduino via USB
2. Select your board: **Tools → Board**
3. Select your port: **Tools → Port**
4. Click **Upload** (→) or press `Ctrl+U`

---

## 🎮 Usage

1. Open the Serial Monitor: **Tools → Serial Monitor** (`Ctrl+Shift+M`)
2. Set the baud rate to **115200**
3. Present an authorized RFID tag to the reader — the relay will toggle ON/OFF
4. Present an unauthorized tag — access denied message will appear

---

## 📊 Serial Output Examples

**System start:**
```
System Ready. Scan your RFID tag.
Authorized cards: 2
```

**Access granted:**
```
Scanned UID: 52 93 0C 5C
✓ ACCESS GRANTED - User 1 | Relay: ON
```

**Access denied:**
```
Scanned UID: FF 12 34 56
✗ ACCESS DENIED - Unauthorized card
```

**Multiple users toggling the relay:**
```
Scanned UID: 52 93 0C 5C
✓ ACCESS GRANTED - User 1 | Relay: ON

Scanned UID: AB CD 12 34
✓ ACCESS GRANTED - User 2 | Relay: OFF
```

---

## 🔧 Troubleshooting

### Reader Not Detected

| Symptom | Solution |
|---|---|
| No output in Serial Monitor | Check baud rate is set to `115200` |
| `PCD_Init() failed` | Verify wiring, especially SDA and RST pins |
| Card not reading | Confirm MFRC522 is powered from 3.3V (not 5V) |

### Relay Not Responding

| Symptom | Solution |
|---|---|
| Relay doesn't click | Check VCC and GND connections |
| Relay stuck ON or OFF | Verify `RELAY_ACTIVE_LOW` setting matches your module |
| Relay chattering | Increase `DEBOUNCE_DELAY_MS` |

### Card Reading Issues

| Symptom | Solution |
|---|---|
| Card not detected | Hold it flat against the reader for 1–2 seconds |
| Inconsistent reading | Check antenna solder joints on the MFRC522 module |
| Wrong UID shown | Ensure correct MFRC522 library version is installed |

### Multiple Toggles from One Scan

- Increase `DEBOUNCE_DELAY_MS` (default: `1000ms`)
- Check whether the tag stays within the reader's field
- Consider adding an LED indicator to provide scan feedback

---

## 🎨 Customization

### Add Visual / Audio Feedback

```cpp
void grantAccess(const char* userName) {
    // LED flash
    digitalWrite(LED_PIN, HIGH);
    delay(100);
    digitalWrite(LED_PIN, LOW);

    // Buzzer beep
    tone(BUZZER_PIN, 1000, 200);

    // ... original relay code
}
```

### Momentary Relay Activation

```cpp
void setRelay(bool on) {
    if (on) {
        digitalWrite(RELAY_PIN, RELAY_ACTIVE_LOW ? LOW : HIGH);
        delay(2000);  // Hold for 2 seconds, then turn off
        digitalWrite(RELAY_PIN, RELAY_ACTIVE_LOW ? HIGH : LOW);
    }
}
```

### Access Logging

```cpp
void grantAccess(const char* userName) {
    logAccess(userName, "GRANTED");  // Log to SD card or EEPROM

    Serial.print(millis() / 1000);
    Serial.print("s - Access granted to: ");
    Serial.println(userName);

    // ... original relay code
}
```

### LCD Display

```cpp
#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void grantAccess(const char* userName) {
    lcd.clear();
    lcd.print("Access Granted");
    lcd.setCursor(0, 1);
    lcd.print(userName);
    delay(2000);

    // ... original relay code
}
```

---

## 📁 Project Structure

```
rfid-relay-control/
├── rfid_relay_control.ino   # Main Arduino sketch
├── README.md                # Documentation
├── LICENSE                  # MIT License
└── .gitignore               # Git ignore rules
```

---

## 🤝 Contributing

Contributions are welcome! To get started:

1. 🍴 Fork the repository
2. 🔧 Create a feature branch: `git checkout -b feature/AmazingFeature`
3. 💾 Commit your changes: `git commit -m 'Add some AmazingFeature'`
4. 📤 Push to the branch: `git push origin feature/AmazingFeature`
5. 🎉 Open a Pull Request

Bug reports and feature requests are tracked on the [GitHub Issues](../../issues) page.

---

## 📄 License

© ayuuXploits. All Rights Reserved.

---

## ⚠️ Disclaimer

This code is provided **"AS IS"** without warranty of any kind.

- Always test thoroughly before deploying in security-sensitive applications
- For production use, consider adding:
  - Encryption for RFID communication
  - Access timeout limits
  - Audit logging
  - Multi-factor authentication
- The author assumes no responsibility for misuse or damages resulting from this project

---

<div align="center">

Made with ❤️ for the Arduino community

⭐ **Star this repo** if you found it useful! ⭐

</div>
