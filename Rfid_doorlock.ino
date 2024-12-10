#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);  // Instance of the RFID

const int relayPin = 7;  // Relay control pin
const int ledPin = 6;    // LED pin
const int buzzerPin = 5; // Buzzer pin

String accessCard = "23 5F BF 27";  // Replace with your RFID card UID

void setup() {
  // Initialize Serial communication
  Serial.begin(9600);
  SPI.begin();         // Init SPI bus
  mfrc522.PCD_Init();  // Init MFRC522 card

  pinMode(relayPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);

  digitalWrite(relayPin, LOW);  // Door locked by default
}

void loop() {
  // Look for new cards
  if (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  String cardID = "";
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    cardID += String(mfrc522.uid.uidByte[i], HEX);
    cardID += " ";
  }
  cardID.trim();

  // Check if the card ID matches the authorized card
  if (cardID == accessCard) {
    Serial.println("Access Granted");
    digitalWrite(ledPin, HIGH);     // Turn on LED
    digitalWrite(buzzerPin, HIGH);  // Turn on buzzer
    digitalWrite(relayPin, HIGH);   // Unlock door (activate relay)
    delay(5000);                    // Keep door unlocked for 5 seconds
    digitalWrite(relayPin, LOW);    // Lock door (deactivate relay)
    digitalWrite(ledPin, LOW);      // Turn off LED
    digitalWrite(buzzerPin, LOW);   // Turn off buzzer
  } else {
    Serial.println("Access Denied");
    digitalWrite(buzzerPin, HIGH);  // Turn on buzzer
    delay(2000);
    digitalWrite(buzzerPin, LOW);   // Turn off buzzer
  }

  // Halt PICC
  mfrc522.PICC_HaltA();
}
