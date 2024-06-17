#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <MFRC522.h>
#include <Stepper.h>

#define SS_PIN 53  // Pin SS cho RFID trên Arduino Mega
#define RST_PIN 5  // Pin RST cho RFID trên Arduino Mega
MFRC522 rfid(SS_PIN, RST_PIN);

LiquidCrystal_I2C lcd(0x27, 20, 4); // LCD 20x4 với địa chỉ I2C 0x27

const int stepsPerRevolution = 2048; // Số bước cho một vòng quay hoàn chỉnh
Stepper myStepper = Stepper(stepsPerRevolution, 8, 10, 9, 11); //set up chân của step: in4, in3, in2, in1

int parking_slots[] = {26, 28, 34, 36};

int control_slot1_ir_s = 22; // Cảm biến hồng ngoại lối vào 
int control_slot2_ir_s = 24; // Cảm biến hồng ngoại lối ra

String status_sensor[] = {"_","_","_","_"};  // trạng thái bãi đỗ xe : _ : empty , x : busy  
int total_slot; // tổng số vị trí 
int empty_slot; // số vị trí trống
const int interval = 5000;   // khoảng thời gian cửa mở (5 giây)
unsigned long previousMillis = 0;  // lưu thời gian cuối cùng cửa được mở

void setup() {
  Serial.begin(9600);
  Serial.println("Ready to scan RFID card");
  SPI.begin();
  rfid.PCD_Init();
  
  total_slot = sizeof(parking_slots) / sizeof(parking_slots[0]); 
  empty_slot = sizeof(parking_slots) / sizeof(parking_slots[0]); // khi khởi động thì mọi vị trí đều trống, empty = total_slot
  for (int i = 0; i < total_slot; i++) {
    pinMode(parking_slots[i], INPUT_PULLUP);
  }  
  pinMode(control_slot1_ir_s, INPUT_PULLUP);
  pinMode(control_slot2_ir_s, INPUT_PULLUP);

  lcd.init();
  lcd.backlight();

  myStepper.setSpeed(15); // Tốc độ của động cơ bước
}

void loop() {
  checkParkingSlots();   // cập nhật trạng thái bãi đỗ xe : _ : empty , x : busy  
  displayLCD();
  bool rfidStatus = readRFID();

  if (rfidStatus == true) {
      if (digitalRead(control_slot1_ir_s) == LOW ) {    // mở cửa lối vào 
          handleEntry();  
      } 

      if (digitalRead(control_slot2_ir_s) == LOW ) { // mở cửa lối ra
          handleExit();
      } 
      
      // mở cửa thất bại
      if (digitalRead(control_slot2_ir_s) == HIGH && digitalRead(control_slot1_ir_s) == HIGH) {
        LCD_fail();
      }
  }


  // đóng cửa   
  closeGateIfNeeded();
  delay(2000);
}


void displayLCD() {
  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.print("Con trong: ");
  lcd.print(empty_slot);

  lcd.setCursor(0, 1);
  lcd.print("P1: ");
  lcd.print(status_sensor[0]);
  lcd.print(" ");
  lcd.print(status_sensor[1]);

  lcd.setCursor(0, 2);
  lcd.print("P2: ");
  lcd.print(status_sensor[2]);
  lcd.print(" ");
  lcd.print(status_sensor[3]);
}

void LCD_entry(){
  lcd.clear();
  lcd.setCursor(0, 1);
  lcd.print("Quet the thanh cong");
  lcd.setCursor(0, 2);
  lcd.print("Chao mung ban da den");
}

void LCD_exit(){
  lcd.clear();
  lcd.setCursor(0, 1);
  lcd.print("Quet the thanh cong");
  lcd.setCursor(0, 2);
  lcd.print("Hen gap lai");
}

void LCD_fullslot(){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Het cho do xe");
}

void LCD_fail() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Quet the that bai");
}


void checkParkingSlots() {
  for(int i=0; i < total_slot; i++){
     status_sensor[i] = (digitalRead(parking_slots[i]) == LOW) ? "x" : "_";
  }
}

void handleEntry() {
    if (empty_slot > 0) {
      openGateIfNeeded();
      empty_slot -= 1;

      LCD_entry();
      previousMillis = millis();
    }
    else {
      LCD_fullslot();
    } 
}

void handleExit() {
    openGateIfNeeded();
    empty_slot += 1;

    LCD_exit();
    previousMillis = millis();
}

void openGateIfNeeded(){
  myStepper.step(0.25*stepsPerRevolution);
}

void closeGateIfNeeded() {
  if (previousMillis > 0 && millis() - previousMillis >= interval) {
    myStepper.step(-0.25 * stepsPerRevolution);
    previousMillis = 0;  // Reset previousMillis để không quay lại ngay lập tức
    Serial.print("Dong");
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
