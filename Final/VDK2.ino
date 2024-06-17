#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <MFRC522.h>
#include <Stepper.h>

#define SS_PIN 53
#define RST_PIN 5
MFRC522 rfid(SS_PIN, RST_PIN);

LiquidCrystal_I2C lcd(0x27, 20, 4);

const int stepsPerRevolution = 2048;
Stepper myStepper(stepsPerRevolution, 8, 10, 9, 11);

int parking_slots[] = {26, 28, 34, 36};
int control_slot1_ir_s = 22;
int control_slot2_ir_s = 24;
String status_sensor[] = {"_", "_", "_", "_"};
int empty_slot;
const int interval = 5000;
unsigned long previousMillis = 0;

void setup() {
  Serial.begin(9600);
  Serial.println("Ready to scan RFID card");
  SPI.begin();
  rfid.PCD_Init();
  int total_slot = sizeof(parking_slots) / sizeof(parking_slots[0]);
  empty_slot = total_slot;
  for (int pin : parking_slots) pinMode(pin, INPUT_PULLUP);
  pinMode(control_slot1_ir_s, INPUT_PULLUP);
  pinMode(control_slot2_ir_s, INPUT_PULLUP);

  lcd.init();
  lcd.backlight();
  myStepper.setSpeed(15);
}

void displayLCD(const String& line1, const String& line2) {
  lcd.clear();
  lcd.setCursor(0, 1);
  lcd.print(line1);
  lcd.setCursor(0, 2);
  lcd.print(line2);
}

void loop() {
  checkParkingSlots();
  bool rfidStatus = readRFID();
  if (rfidStatus) {
    handleEntryAndExit();
  }
  closeGateIfNeeded();
}

void checkParkingSlots() {
  for (int i = 0; i < sizeof(parking_slots) / sizeof(parking_slots[0]); i++) {
    status_sensor[i] = digitalRead(parking_slots[i]) == LOW ? "x" : "_";
  }
}

bool readRFID() {
  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) {
    return false;
  }
  Serial.print("Card ID: ");
  for (byte i = 0; i < rfid.uid.size; i++) {
    Serial.print(rfid.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(rfid.uid.uidByte[i], HEX);
  }
  Serial.println();
  return true;
}

void handleEntryAndExit() {
  if (digitalRead(control_slot1_ir_s) == LOW && empty_slot > 0) {
    myStepper.step(0.25 * stepsPerRevolution);
    empty_slot--;
    displayLCD("Quet the thanh cong", "Chao mung ban da den");
    previousMillis = millis();
  } else if (digitalRead(control_slot2_ir_s) == LOW) {
    myStepper.step(0.25 * stepsPerRevolution);
    empty_slot++;
    displayLCD("Quet the thanh cong", "Hen gap lai");
    previousMillis = millis();
  } else {
    displayLCD("Quet the that bai", "");
  }
}

void closeGateIfNeeded() {
  if (millis() - previousMillis >= interval) {
    myStepper.step(-0.25 * stepsPerRevolution);
  }
}