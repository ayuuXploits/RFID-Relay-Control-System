# RFID Relay Control System

[![Arduino](https://img.shields.io/badge/Arduino-00979D?style=for-the-badge&logo=arduino&logoColor=white)](https://www.arduino.cc/)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Platform](https://img.shields.io/badge/platform-Arduino%20%7C%20ESP8266%20%7C%20ESP32-blue)](https://www.arduino.cc/)

An Arduino-based access control system that uses RFID tags to toggle a relay (e.g., for controlling lights, doors, or other electronic devices).

## 📋 Table of Contents
- [Features](#features)
- [Hardware Requirements](#hardware-requirements)
- [Pin Connections](#pin-connections)
- [Software Requirements](#software-requirements)
- [Setup Instructions](#setup-instructions)
- [Usage](#usage)
- [Serial Output Examples](#serial-output-examples)
- [Troubleshooting](#troubleshooting)
- [Customization](#customization)
- [Project Structure](#project-structure)
- [License](#license)
- [Contributing](#contributing)
- [Disclaimer](#disclaimer)

## ✨ Features

- 🚪 **RFID Authentication** - Only authorized RFID tags can trigger the relay
- 🔄 **Toggle Operation** - Each authorized scan toggles the relay ON/OFF
- 👥 **Multi-User Support** - Add multiple authorized RFID cards with custom names
- ⚙️ **Configurable Relay Type** - Supports both Active LOW and Active HIGH relays
- 🛡️ **Debounce Protection** - Prevents multiple triggers from a single scan
- 📊 **Serial Monitoring** - Real-time feedback with clear access messages
- 💾 **Memory Efficient** - Uses Flash memory for strings and compile-time constants
- 🔌 **Cross-Platform** - Works with Arduino Uno, Nano, Mega, ESP8266, ESP32

## 📦 Hardware Requirements

| Component | Quantity | Specifications |
|-----------|----------|----------------|
| Arduino board | 1 | Uno, Nano, Mega, ESP32, ESP8266 |
| MFRC522 RFID Module | 1 | 13.56MHz |
| Relay Module | 1 | Active LOW or Active HIGH |
| RFID Tags/Cards | 1+ | 13.56MHz compatible |
| Connecting wires | as needed | Female-to-female/Male-to-female |

## 🔌 Pin Connections

### MFRC522 Connections

| MFRC522 Pin | Arduino Uno/Nano | Arduino Mega | ESP32 | ESP8266 |
|-------------|-----------------|--------------|-------|---------|
| SDA (SS) | 5 | 5 | 5 | 5 |
| RST | 21 | 21 | 21 | 21 |
| SCK | 13 | 52 | 18 | 14 |
| MOSI | 11 | 51 | 23 | 13 |
| MISO | 12 | 50 | 19 | 12 |
| VCC | 3.3V | 3.3V | 3.3V | 3.3V |
| GND | GND | GND | GND | GND |

### Relay Connections

| Relay Pin | Arduino Pin |
|-----------|--------------|
| IN | 13 |
| VCC | 5V |
| GND | GND |

> **⚠️ Note:** SPI pins vary by board. Check your board's pinout diagram.

## 💻 Software Requirements

- **Arduino IDE** (1.8.0 or newer) - [Download](https://www.arduino.cc/en/software)
- **MFRC522 library** by Miguel Balboa (v1.4.10+)

### Installing Dependencies

1. Open Arduino IDE
2. Go to **Tools → Manage Libraries** (or `Ctrl+Shift+I`)
3. Search for "MFRC522"
4. Install **MFRC522 by Miguel Balboa**
5. Wait for installation to complete

## 🚀 Setup Instructions

### 1. Find Your RFID Tag UID

Upload the [dumpinfo example](https://github.com/miguelbalboa/rfid/tree/master/examples/ReadUid) from MFRC522 library:

```cpp
// Example output:
// Card UID: 52 93 0C 5C
// Card type: MIFARE 1KB

## 🛠️ Built With

- Arduino Uno / Nano
- MFRC522 RFID Module
- C++ (Arduino framework)
## 🚀 Getting Started

1. Clone this repo
2. Install the MFRC522 library via Arduino Library Manager
3. Wire up the module as shown above
4. Upload `rfid_reader.ino` to your Arduino
5. Open Serial Monitor at 9600 baud and scan a card!

 © 2026 ayuuXploits — All Rights Reserved
