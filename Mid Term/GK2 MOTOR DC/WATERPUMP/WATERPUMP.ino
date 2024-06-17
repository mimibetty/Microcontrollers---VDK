#include <Servo.h>

// Khai báo các chân GPIO
const int rainSensorPin = 2;    // Chân cảm biến mưa
const int servoPin = 13;         // Chân điều khiển Servo

//relay L298N
int input33 = 8;
int input44 = 9;

// Chân điều khiển động cơ DC
const int buttonDC = 18;         // Chân nút nhấn cho động cơ DC
const int buttonServo = 2;      // Chân nút nhấn cho Servo
const int buttonChangeState = 3;      // Chân nút nhấn tắt bật cảm biến

Servo servo;                    // Tạo đối tượng Servo
bool motorState = false;
int servoPosition = 0;
bool statusCB = 0;   // CB is running? 

void xoay(int ampli){
  analogWrite(input33, HIGH);
  digitalWrite(input44, LOW);
}

void dung(){
  digitalWrite(input33, LOW);
  digitalWrite(input44, LOW);
}

void setup() {
  pinMode(rainSensorPin, INPUT);
  pinMode(buttonDC, INPUT_PULLUP);
  pinMode(buttonServo, INPUT_PULLUP);
  pinMode(buttonChangeState, INPUT_PULLUP);
  
  Serial.begin(115200);

  attachInterrupt(digitalPinToInterrupt(buttonDC), toggleMotor, FALLING);
  attachInterrupt(digitalPinToInterrupt(buttonServo), toggleServo, FALLING);
  attachInterrupt(digitalPinToInterrupt(buttonChangeState), ChangeStatus, FALLING);

  servo.attach(servoPin);
  servo.write(servoPosition);
}

void loop() {
  if (statusCB == 1) {
      if (digitalRead(rainSensorPin) == LOW) { // Mưa được phát hiện
        servo.write(60);
        xoay(255);

        motorState = 1;
        servoPosition = 60;

        Serial.print("Mua ");
      } else {
        servo.write(0);
        dung();

        motorState = 0;
        servoPosition = 0;

        Serial.print("Ko Mua ");
      }
  }

  Serial.print("Goc quay: ");
  Serial.println(servo.read());
  delay(200); // Để tránh đọc sai do dộng mưa
}

void toggleMotor() {
  motorState = !motorState;
  if (motorState == 0) {
    dung();
  }
  else {
    xoay(255);
  }
  Serial.print("Da bam nut DC\n");

}

void toggleServo() {
  servoPosition = servoPosition == 0 ? 60 : 0;
  servo.write(servoPosition);
  Serial.print("Da bam nut servo\n");
}
void ChangeStatus() {
  statusCB = !statusCB;
}