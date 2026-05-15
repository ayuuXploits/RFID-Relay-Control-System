#include <SPI.h>
#include <MFRC522.h>

// ========== PIN DEFINITIONS ==========
constexpr int SS_PIN = 5;
constexpr int RST_PIN = 21;
constexpr int RELAY_PIN = 13;

// ========== CONFIGURATION ==========
// Relay type: true = Active LOW (LOW = ON, HIGH = OFF), false = Active HIGH
constexpr bool RELAY_ACTIVE_LOW = true;

// Debounce delay in milliseconds after card scan
constexpr unsigned long DEBOUNCE_DELAY_MS = 1000;

// Serial baud rate
constexpr unsigned long SERIAL_BAUD = 115200;

// ========== AUTHORIZED UIDS ==========
// Add multiple authorized UIDs here
struct AuthorizedCard {
    byte uid[4];
    const char* name;
};

constexpr AuthorizedCard AUTHORIZED_CARDS[] = {
    {{0x52, 0x93, 0x0C, 0x5C}, "User 1"},
    // Add more cards here:
    // {{0xXX, 0xXX, 0xXX, 0xXX}, "User 2"},
};

constexpr int NUM_AUTHORIZED_CARDS = sizeof(AUTHORIZED_CARDS) / sizeof(AUTHORIZED_CARDS[0]);

// ========== GLOBAL VARIABLES ==========
MFRC522 rfid(SS_PIN, RST_PIN);
bool relayState = false;
unsigned long lastScanTime = 0;

// ========== FORWARD DECLARATIONS ==========
void setupRelay();
void printHex(byte* buffer, byte size);
bool isAuthorized(byte* scannedUid, byte uidSize);
void grantAccess(const char* userName);
void denyAccess();
void setRelay(bool on);

// ========== SETUP ==========
void setup() {
    Serial.begin(SERIAL_BAUD);
    while (!Serial && millis() < 2000);  // Wait for Serial on native USB boards
    
    SPI.begin();
    rfid.PCD_Init();
    
    setupRelay();
    
    Serial.println(F("System Ready. Scan your RFID tag."));
    Serial.print(F("Authorized cards: "));
    Serial.println(NUM_AUTHORIZED_CARDS);
}

// ========== MAIN LOOP ==========
void loop() {
    // Debounce check
    if (millis() - lastScanTime < DEBOUNCE_DELAY_MS) {
        return;
    }
    
    // Check for new card
    if (!rfid.PICC_IsNewCardPresent()) {
        return;
    }
    
    // Try to read the card serial
    if (!rfid.PICC_ReadCardSerial()) {
        return;
    }
    
    // Process the scanned card
    processScannedCard();
    
    // Halt and stop encryption
    rfid.PICC_HaltA();
    rfid.PCD_StopCrypto1();
    
    lastScanTime = millis();
}

// ========== CARD PROCESSING ==========
void processScannedCard() {
    Serial.print(F("Scanned UID: "));
    printHex(rfid.uid.uidByte, rfid.uid.size);
    
    // Check authorization
    int authorizedIndex = isAuthorized(rfid.uid.uidByte, rfid.uid.size);
    
    if (authorizedIndex >= 0) {
        grantAccess(AUTHORIZED_CARDS[authorizedIndex].name);
    } else {
        denyAccess();
    }
}

// ========== AUTHORIZATION ==========
int isAuthorized(byte* scannedUid, byte uidSize) {
    // Check UID size
    if (uidSize != 4) {  // Assuming all our cards are 4-byte UIDs
        return -1;
    }
    
    // Compare with each authorized card
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
    setRelay(false);  // Start with relay OFF
}

void setRelay(bool on) {
    relayState = on;
    
    if (RELAY_ACTIVE_LOW) {
        digitalWrite(RELAY_PIN, on ? LOW : HIGH);
    } else {
        digitalWrite(RELAY_PIN, on ? HIGH : LOW);
    }
}

void toggleRelay() {
    setRelay(!relayState);
}

// ========== ACCESS HANDLING ==========
void grantAccess(const char* userName) {
    toggleRelay();
    Serial.print(F("✓ ACCESS GRANTED - "));
    Serial.print(userName);
    Serial.print(F(" | Relay: "));
    Serial.println(relayState ? F("ON") : F("OFF"));
    
    // Optional: Add visual/audio feedback here
    // e.g., blink an LED or beep a buzzer
}

void denyAccess() {
    Serial.println(F("✗ ACCESS DENIED - Unauthorized card"));
    
    // Optional: Denial feedback
    // e.g., quick blink of error LED
}

// ========== UTILITY FUNCTIONS ==========
void printHex(byte* buffer, byte size) {
    for (byte i = 0; i < size; i++) {
        if (buffer[i] < 0x10) {
            Serial.print('0');
        }
        Serial.print(buffer[i], HEX);
        if (i < size - 1) Serial.print(' ');
    }
    Serial.println();
}

// ========== DEBUG FUNCTION (optional) ==========
void printAuthorizedCards() {
    Serial.println(F("\nAuthorized Cards:"));
    for (int i = 0; i < NUM_AUTHORIZED_CARDS; i++) {
        Serial.print(F("  Card "));
        Serial.print(i + 1);
        Serial.print(F(": "));
        printHex(AUTHORIZED_CARDS[i].uid, 4);
        Serial.print(F("    Name: "));
        Serial.println(AUTHORIZED_CARDS[i].name);
    }
}
