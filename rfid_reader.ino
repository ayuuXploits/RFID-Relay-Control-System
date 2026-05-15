/*
 * RFID Relay Control System
 * 
 * Controls a relay using authorized RFID tags.
 * Each authorized scan toggles the relay ON/OFF.
 *
 * Hardware:
 *   - MFRC522 RFID module (13.56 MHz)
 *   - Relay module (Active LOW or Active HIGH)
 *
 * ⚠️ Pin Conflict Warning:
 *   On Arduino Uno/Nano, pin 13 is also the SPI SCK pin.
 *   Do NOT use pin 13 for the relay on these boards.
 *   Use a different pin (e.g., pin 7) and update RELAY_PIN below.
 *   On ESP32/ESP8266, pin 13 is safe for the relay.
 *
 * See README.md for full wiring diagram and setup instructions.
 */

#include <SPI.h>
#include <MFRC522.h>

// ========== PIN DEFINITIONS ==========
constexpr int SS_PIN    = 5;    // MFRC522 SDA (SS) pin
constexpr int RST_PIN   = 21;   // MFRC522 RST pin
constexpr int RELAY_PIN = 7;    // Relay control pin
                                // ⚠️ Avoid pin 13 on Uno/Nano (shared with SPI SCK)

// ========== CONFIGURATION ==========
// Relay type: true = Active LOW (LOW = ON, HIGH = OFF)
//             false = Active HIGH (HIGH = ON, LOW = OFF)
constexpr bool RELAY_ACTIVE_LOW = true;

// Debounce delay in milliseconds after a card scan
constexpr unsigned long DEBOUNCE_DELAY_MS = 1000;

// Serial baud rate
constexpr unsigned long SERIAL_BAUD = 115200;

// ========== AUTHORIZED CARDS ==========
// Add all authorized RFID UIDs and their user names here.
// Use the MFRC522 DumpInfo example sketch to find a card's UID.
//
// Example output:
//   Card UID: 52 93 0C 5C
//   Card type: MIFARE 1KB
struct AuthorizedCard {
    byte uid[4];        // 4-byte UID (MIFARE Classic / MIFARE Ultralight)
    const char* name;   // Display name shown in Serial Monitor
};

constexpr AuthorizedCard AUTHORIZED_CARDS[] = {
    {{0x52, 0x93, 0x0C, 0x5C}, "User 1"},
    // Add more cards here:
    // {{0xAB, 0xCD, 0x12, 0x34}, "User 2"},
    // {{0xXX, 0xXX, 0xXX, 0xXX}, "User N"},
};

constexpr int NUM_AUTHORIZED_CARDS = sizeof(AUTHORIZED_CARDS) / sizeof(AUTHORIZED_CARDS[0]);

// ========== GLOBAL VARIABLES ==========
MFRC522 rfid(SS_PIN, RST_PIN);
bool relayState    = false;
unsigned long lastScanTime = 0;

// ========== FORWARD DECLARATIONS ==========
void setupRelay();
void processScannedCard();
void printHex(byte* buffer, byte size);
int  isAuthorized(byte* scannedUid, byte uidSize);
void grantAccess(const char* userName);
void denyAccess();
void setRelay(bool on);
void toggleRelay();

// ========== SETUP ==========
void setup() {
    Serial.begin(SERIAL_BAUD);
    while (!Serial && millis() < 2000);   // Wait for Serial on native-USB boards

    SPI.begin();
    rfid.PCD_Init();

    if (!rfid.PCD_PerformSelfTest()) {
        Serial.println(F("ERROR: PCD_Init() failed. Check wiring and power."));
        while (true);   // Halt — nothing works without the reader
    }
    rfid.PCD_Init();    // Re-initialize after self-test

    setupRelay();

    Serial.println(F("System Ready. Scan your RFID tag."));
    Serial.print(F("Authorized cards: "));
    Serial.println(NUM_AUTHORIZED_CARDS);
}

// ========== MAIN LOOP ==========
void loop() {
    // Debounce: ignore scans that arrive too soon after the last one
    if (millis() - lastScanTime < DEBOUNCE_DELAY_MS) {
        return;
    }

    // Wait for a new card in the reader's field
    if (!rfid.PICC_IsNewCardPresent()) {
        return;
    }

    // Try to read the card serial number
    if (!rfid.PICC_ReadCardSerial()) {
        return;
    }

    processScannedCard();

    // Put the card and reader back to idle
    rfid.PICC_HaltA();
    rfid.PCD_StopCrypto1();

    lastScanTime = millis();
}

// ========== CARD PROCESSING ==========
void processScannedCard() {
    Serial.print(F("Scanned UID: "));
    printHex(rfid.uid.uidByte, rfid.uid.size);

    int authorizedIndex = isAuthorized(rfid.uid.uidByte, rfid.uid.size);

    if (authorizedIndex >= 0) {
        grantAccess(AUTHORIZED_CARDS[authorizedIndex].name);
    } else {
        denyAccess();
    }
}

// ========== AUTHORIZATION ==========
// Returns the index of the matching card in AUTHORIZED_CARDS, or -1 if not found.
// Note: Only 4-byte UIDs are supported (MIFARE Classic, MIFARE Ultralight).
int isAuthorized(byte* scannedUid, byte uidSize) {
    if (uidSize != 4) {
        return -1;  // Unsupported UID length
    }

    for (int i = 0; i < NUM_AUTHORIZED_CARDS; i++) {
        if (memcmp(scannedUid, AUTHORIZED_CARDS[i].uid, 4) == 0) {
            return i;
        }
    }

    return -1;  // Not found
}

// ========== RELAY CONTROL ==========
void setupRelay() {
    pinMode(RELAY_PIN, OUTPUT);
    setRelay(false);    // Ensure relay starts in the OFF state
}

void setRelay(bool on) {
    relayState = on;
    digitalWrite(RELAY_PIN, (RELAY_ACTIVE_LOW ? !on : on) ? HIGH : LOW);
}

void toggleRelay() {
    setRelay(!relayState);
}

// ========== ACCESS HANDLING ==========
void grantAccess(const char* userName) {
    toggleRelay();

    Serial.print(F("\u2713 ACCESS GRANTED - "));
    Serial.print(userName);
    Serial.print(F(" | Relay: "));
    Serial.println(relayState ? F("ON") : F("OFF"));

    // Optional: add visual/audio feedback here
    // e.g. blink an LED or beep a buzzer
}

void denyAccess() {
    Serial.println(F("\u2717 ACCESS DENIED - Unauthorized card"));

    // Optional: add denial feedback here
    // e.g. rapid-blink an error LED
}

// ========== UTILITY FUNCTIONS ==========
// Prints a byte array as space-separated uppercase hex (e.g. "52 93 0C 5C")
void printHex(byte* buffer, byte size) {
    for (byte i = 0; i < size; i++) {
        if (buffer[i] < 0x10) {
            Serial.print('0');
        }
        Serial.print(buffer[i], HEX);
        if (i < size - 1) {
            Serial.print(' ');
        }
    }
    Serial.println();
}

// ========== DEBUG HELPER (optional) ==========
// Call printAuthorizedCards() from setup() to list all configured cards
// on boot — useful when verifying a freshly programmed board.
void printAuthorizedCards() {
    Serial.println(F("\nAuthorized Cards:"));
    for (int i = 0; i < NUM_AUTHORIZED_CARDS; i++) {
        Serial.print(F("  Card "));
        Serial.print(i + 1);
        Serial.print(F(": "));
        printHex((byte*)AUTHORIZED_CARDS[i].uid, 4);
        Serial.print(F("    Name: "));
        Serial.println(AUTHORIZED_CARDS[i].name);
    }
}
