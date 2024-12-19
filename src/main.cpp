// Blynk Connection
#define Esp8266Serial Serial2
#define Esp8266_Baud 9600

#include <LiquidCrystal_I2C.h>
#include <Arduino.h>
#include <MFRC522.h>
#include <Wire.h>

// Devices Constant
#define OPEN 1
#define CLOSE 0
#define DETECTED 0
#define NOT_DETECTED 1
#define ON 1
#define OFF 0
#define LIGHT 0
#define DARK 1
#define NUM_SENSORS 3
#define ONLINE 1
#define OFFLINE 0
#define RST_PIN 5
#define SS_PIN 53

// Object Define
LiquidCrystal_I2C classLCD(0x27, 20, 4);
MFRC522 cardScanner(SS_PIN, RST_PIN);
const int rotations = 1; // Number of full rotations
const int rotationTime = 1200; // Time for one full rotation in milliseconds (adjust as needed)
#pragma region Pin define
// Dorm room 2
int dormRoomButtonPin_2 = 34;
int dormRoomLedPin_2 = 35;
int systemMode;

// Class room
int classRoomButtonLedPin = 36;
int classRoomLedPin = 37;
int classroomServoPin = 38;
int classroomSensorPin = 39;
#pragma endregion

#pragma region States Define
bool currentDormRoom_2LedState = false;
int oldDormRoomButton_2Value;
#pragma endregion

#pragma region Function declarations
void setup();
void loop();
void handleRoomLedLogic();
void handleClassroomDoorLogic();
void setupServo();
void setupPin();
void setupOldValue(); 
void syncStatesToBlynk();
String checkCardId();
#pragma endregion

void setup() {
  Serial.begin(9600);
  SPI.begin();
  cardScanner.PCD_Init();
  Esp8266Serial.begin(Esp8266_Baud);
  delay(10);
  Serial.println("Connecting ...");
  classLCD.init();
  classLCD.backlight();
  classLCD.setCursor(0, 0);
  classLCD.print("Ready!");
  setupPin();
  setupOldValue();
}

void loop() {
  handleRoomLedLogic();
  handleClassroomDoorLogic();
}

#pragma region Setup


void setupPin() {
  pinMode(dormRoomButtonPin_2, INPUT_PULLUP);
  pinMode(dormRoomLedPin_2, OUTPUT);
  pinMode(classRoomButtonLedPin, INPUT_PULLUP);
  pinMode(classRoomLedPin, OUTPUT);
}

void setupOldValue() {
  oldDormRoomButton_2Value = digitalRead(dormRoomButtonPin_2);
}
#pragma endregion

#pragma region Room led logic
void handleRoomLedLogic() {
  int newDormRoomButton_2Value = digitalRead(dormRoomButtonPin_2);
  if (newDormRoomButton_2Value != oldDormRoomButton_2Value) {
    currentDormRoom_2LedState = !currentDormRoom_2LedState;
    digitalWrite(dormRoomLedPin_2, currentDormRoom_2LedState);
    oldDormRoomButton_2Value = newDormRoomButton_2Value;
  }
}
#pragma endregion

void handleClassroomDoorLogic() {
  String cardId = checkCardId();
  if (cardId != "") {
    Esp8266Serial.print(cardId);
  }

  if (Esp8266Serial.available()) {
    Serial.println("here");
    String response = Esp8266Serial.readStringUntil('\n');
    // classLCD.clear();
    classLCD.setCursor(0, 1);
    classLCD.print(response);
    Serial.println(response + "\n");
  }
}

String checkCardId() {
  if (!cardScanner.PICC_IsNewCardPresent() || !cardScanner.PICC_ReadCardSerial())
    return "";
  String uid = "";
  for (byte i = 0; i < cardScanner.uid.size; i++) {
    uid += String(cardScanner.uid.uidByte[i], HEX);
  }
  cardScanner.PICC_HaltA();
  uid.toUpperCase();
  Serial.println("UID: " + uid);
  return uid;
}


#pragma region Blynk logic
void syncStatesToBlynk() {
}
#pragma endregion