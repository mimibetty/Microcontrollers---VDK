
#include "DHT.h"
#define DHT11_PIN 13

int redLed = 4;  //  nhiệt độ >= 30
int yellowLed = 5; // có đang bật mode cảm biến 
int btn = 3;  // nút chuyển sang mode cảm biến 
int btnManual = 2;  // nút nhấn thủ công tắt/bật quay 

int input33 = 8;
int input44 = 9;

// trạng thái tắt bật cảm biến
int cambienStatus = 1;

DHT dht11(DHT11_PIN, DHT11);

void setup() {
  Serial.begin(9600);
  dht11.begin(); // initialize the sensor

  pinMode(btn, INPUT_PULLUP);
  pinMode(btnManual, INPUT_PULLUP);
  pinMode(redLed, OUTPUT);

  attachInterrupt(1, cambienTrigger, FALLING);
  attachInterrupt(0, manualTrigger, FALLING);
}

void readTempHum() { // đọc nhiệt độ, độ ẩm
  delay(1000); // wait a few seconds between measurements.

  float tempC = dht11.readTemperature();
  float tempF = dht11.readTemperature(true);

  redTurn(tempC);

  if (isnan(tempC) || isnan(tempF)) {
    Serial.println("Failed to read from DHT11 sensor!");
  } else {
    Serial.print("Temperature: ");
    Serial.print(tempC);
    Serial.print("°C ~ ");
    Serial.print(tempF);
    Serial.println("°F");
  }
}

void redTurn(float tempC){ // > 30 độ thì quay 
  if(tempC <= 30){
    analogWrite(input33, 0);
    digitalWrite(redLed, LOW);
  }
  else{
    analogWrite(input33, 255);
    digitalWrite(redLed, HIGH);
  }
}

void yellowTurn(){
  if (cambienStatus == -1) {
    digitalWrite(yellowLed, LOW);
  } 
  else {
    digitalWrite(yellowLed, HIGH);
  }
  // digitalWrite(yellowLed, digitalRead(13)); // đèn vàng sáng khi cảm biến hoạt động
}

void xoay(int ampli){
  analogWrite(input33, ampli);
  digitalWrite(input44, LOW);
}

void dung(){
  digitalWrite(input33, LOW);
  digitalWrite(input44, LOW);
}

void manualTrigger(){   // nút tắt bật bằng tay
  Serial.println("Manual button clicked");

  cambienStatus = -1;
  digitalWrite(redLed, LOW);

  int status = digitalRead(input33); // = 1 => đang xoay ,  = 0 => đang dừng
  Serial.println(status);
  if(status != 1) { // nếu đang đừng thì xoay 
    xoay(255);
  }
  else{
    dung();
  }
}

void cambienTrigger(){
  Serial.println("Cam bien button clicked");
  cambienStatus *= -1;
}

void loop() {
 
  yellowTurn();
  if(cambienStatus == 1){
    readTempHum();
  }
}