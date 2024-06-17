// Fill-in information from your Blynk Template here
#define BLYNK_TEMPLATE_ID "TMPL6P1uGIC_z"
#define BLYNK_TEMPLATE_NAME "VDK"

#define BLYNK_FIRMWARE_VERSION "0.1.0"

#define BLYNK_PRINT Serial
//#define BLYNK_DEBUG

#define APP_DEBUG

// Uncomment your board, or configure a custom board in Settings.h
//#define USE_SPARKFUN_BLYNK_BOARD
#define USE_NODE_MCU_BOARD
//#define USE_WITTY_CLOUD_BOARD
//#define USE_WEMOS_D1_MINI

#include "BlynkEdgent.h"
#include <Servo.h>

#define ledCB D7
#define servoPin D6
#define rainSensorPin A0
#define input33 D1
#define input44 D2

#define buttonDC D3
#define buttonServo D4
#define buttonChangeState D5 

Servo servo;                    // Tạo đối tượng Servo
int motorState = 0;
int servoPosition = 0;
int statusCB = 0;   // CB is running? 

boolean buttonDC_state = HIGH;
boolean buttonServo_state = HIGH;
boolean buttonChangeState_state = HIGH;
unsigned long times = millis();
WidgetLED led_connect(V0);


void setup() {
  Serial.begin(115200);
  delay(100);

  pinMode(rainSensorPin, INPUT);

  // pinMode(servoPin, OUTPUT);
  pinMode(ledCB, OUTPUT);
  pinMode(input33, OUTPUT);


  pinMode(buttonDC, INPUT_PULLUP);
  pinMode(buttonServo, INPUT_PULLUP);
  pinMode(buttonChangeState, INPUT_PULLUP);

  servo.attach(servoPin);
  servo.write(servoPosition);

  BlynkEdgent.begin();
}

void loop() {
  // if connection linked,  V0 light blinking
  checkConnection();
  Serial.print("cam bien: ");
  Serial.println(statusCB);  

  delay(200);
  checkMua();
  checkButton();
  checkCamBien();

  Blynk.virtualWrite(V1, motorState);
  Blynk.virtualWrite(V2, servoPosition);
  Blynk.virtualWrite(V3, statusCB);
  Blynk.virtualWrite(V4, analogRead(rainSensorPin));
}
void checkConnection(){
  BlynkEdgent.run();
  if (millis() - times > 1000) {
    Blynk.virtualWrite(V5, millis() / 1000);
    if (led_connect.getValue()) {
      led_connect.off();
    } else {
      led_connect.on();
    }
    times = millis();
  }
}


// Nhận tín hiệu click từ blink
// V1: button dc
// V2: button servo
// V3: button status
// V4: rain status

BLYNK_WRITE(V1) {
  int p = param.asInt();
  Serial.println("blynk btn motor");
  statusCB = 0;
  toggleMotor();
}
BLYNK_WRITE(V2) {
  int p = param.asInt();
  Serial.println("blynk btn servo");
  statusCB = 0;
  toggleServo();
}
BLYNK_WRITE(V3) {
  int p = param.asInt();
  Serial.println("blynk btn status");

  ChangeStatus();
}

void checkMua(){
  if (statusCB == 1) {
      if (analogRead(rainSensorPin) <900) { // Mưa được phát hiện
        servo.write(180);
        xoay(255);

        motorState = 1;
        servoPosition = 180;
        Serial.print("Mua: ");
        Serial.println(analogRead(rainSensorPin));

      } else {
        servo.write(0);
        dung();

        motorState = 0;
        servoPosition = 0;

        Serial.print("Ko Mua: ");
        Serial.println(analogRead(rainSensorPin));

      }
  }

  Serial.print("Goc quay: ");
  Serial.println(servo.read());
  delay(200); // Để tránh đọc sai do dộng mưa
}

void toggleMotor() {
  motorState = !motorState;
  if (motorState == 0) {
    Serial.println("Dung Motor");
    dung();
  }
  else {
    Serial.println("Chay Motor");
    xoay(255);
  }
  Serial.println("Da bam nut DC\n");
}

void toggleServo() {
  servoPosition = servoPosition == 0 ? 180 : 0;
  servo.write(servoPosition);
  // this
  // int servoTest = servoPosition == 0 ? 1 : 0;
  // digitalWrite( servoPin, servoTest);
  Serial.print("servo ");
  Serial.println(servoPosition);
  delay(500);
}
void ChangeStatus() {
  statusCB = !statusCB;
}


void checkButton() {
  if (digitalRead(buttonDC) == LOW) {
    if (buttonDC_state == HIGH) {
      Serial.println("btn DC check");
      //< Thay đổi code chô này >

      //  Cập nhật vào trên blynk
      toggleMotor();
      buttonDC_state = LOW;
      statusCB = 0;

      delay(200);
    }
  } else {
    buttonDC_state = HIGH;
  }

  if (digitalRead(buttonServo) == LOW) {
    if (buttonServo_state == HIGH) {
      //< Thay đổi code chô này >

      //  Cập nhật vào trên blynk
      Serial.println("btn servo");

      toggleServo();
      buttonServo_state = LOW;
      statusCB = 0;

      delay(200);
    }
  } else {
    buttonServo_state = HIGH;
  }

  if (digitalRead(buttonChangeState) == LOW) {
    if (buttonChangeState_state == HIGH) {
      Serial.println("btn status");

      //< Thay đổi code chô này >

      //  Cập nhật vào trên blynk
      ChangeStatus();
      buttonChangeState_state = LOW;
      delay(200);
    }
  } else {
    buttonChangeState_state = HIGH;
  }
}

void xoay(int ampli){
  digitalWrite(input33, HIGH);
  // digitalWrite(input44, LOW);

  Serial.println("Xoay");
}

void dung(){
  digitalWrite(input33, LOW);
  // digitalWrite(input44, LOW);
  Serial.println("Dung");

}
void checkCamBien(){
  digitalWrite(ledCB, statusCB);
}


