#include <SPI.h>
#include <MFRC522.h>

// Define pins for readability and easy changes.
const int SS_PIN = 5;
const int RST_PIN = 21;
const int RELAY_PIN = 13;

MFRC522 rfid(SS_PIN, RST_PIN);

// Store the authorized UID as a byte array (more memory efficient than String)
// Replace these bytes with your specific UID
byte authorizedUID[] = {0x52, 0x93, 0x0C, 0x5C};

bool lightState = false;

void setup() {
  Serial.begin(115200);
  SPI.begin();
  rfid.PCD_Init();

  pinMode(RELAY_PIN, OUTPUT);
  // Set relay to OFF initially (Assuming Active LOW relay: HIGH = OFF, LOW = ON)
  // If your relay is Active HIGH, change this to LOW.
  digitalWrite(RELAY_PIN, HIGH); 

  Serial.println("System Ready. Scan your tag.");
}

void loop() {
  // Check if a new card is present
  if (!rfid.PICC_IsNewCardPresent()) return;
  
  // Verify if we can read the serial
  if (!rfid.PICC_ReadCardSerial()) return;

  Serial.print("Scanned UID: ");
  printHex(rfid.uid.uidByte, rfid.uid.size);

  // Compare the scanned UID with the authorized UID
  if (checkUID(rfid.uid.uidByte, rfid.uid.size)) {
    toggleRelay();
  } else {
    Serial.println(" - Unauthorized!");
  }

  // Halt PICC and stop encryption to reset the reader for the next scan
  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();
}

// Helper to compare UIDs
bool checkUID(byte *scanned, byte size) {
  if (size != sizeof(authorizedUID)) return false;
  return memcmp(scanned, authorizedUID, size) == 0;
}

// Helper to print Hex values to Serial
void printHex(byte *buffer, byte size) {
  for (byte i = 0; i < size; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
  }
  Serial.println();
}

void toggleRelay() {
  lightState = !lightState;
  // If active LOW, write LOW to turn ON, HIGH to turn OFF
  digitalWrite(RELAY_PIN, lightState ? LOW : HIGH);
  
  Serial.print(" - Access Granted! Relay: ");
  Serial.println(lightState ? "ON" : "OFF");
  
  delay(1000); // Debounce: Wait 1 second so the card isn't read 100 times per second
}
