#include "DHT.h"
#define DHT11_PIN 13
int yellowLed = 6;
int redLed = 5;
int blueLed = 4;
int btn = 3;

// turn on/off LCD
int option = 1;

DHT dht11(DHT11_PIN, DHT11);

void setup() {
  Serial.begin(9600);
  dht11.begin(); // initialize the sensor

  pinMode(btn, INPUT_PULLUP);
  pinMode(yellowLed, OUTPUT);
  pinMode(redLed, OUTPUT);
  pinMode(blueLed, OUTPUT);

  attachInterrupt(1, displayLCD, FALLING);
}

void readTempHum() {
  // wait a few seconds between measurements.
  delay(1000);

  // read humidity
  float humi  = dht11.readHumidity();
  // read temperature as Celsius
  float tempC = dht11.readTemperature();
  // read temperature as Fahrenheit
  float tempF = dht11.readTemperature(true);

  redTurn(tempC);
  blueTurn(humi);
  
  // check if any reads failed
  if (isnan(humi) || isnan(tempC) || isnan(tempF)) {
    Serial.println("Failed to read from DHT11 sensor!");
  } else {
    Serial.print("DHT11# Humidity: ");
    Serial.print(humi);
    Serial.print("%");
    Serial.print("  |  ");
    Serial.print("Temperature: ");
    Serial.print(tempC);
    Serial.print("°C ~ ");
    Serial.print(tempF);
    Serial.println("°F");
  }
}

void redTurn(float tempC){
  if(tempC <= 30){
    analogWrite(redLed, 0);
  }
  else if(tempC <=31){
    analogWrite(redLed, 10);
  }
  else{
    analogWrite(redLed, 255);
  }
}

void blueTurn(float humi){
  if(humi >= 80){
    digitalWrite(blueLed, HIGH);
  }
  else{
    digitalWrite(blueLed, LOW);
  }
}

void yellowTurn(){
  // read the input on analog pin 0:
  int sensorValue = analogRead(A0);
  sensorValue /= 4;
  // print out the value you read:
  analogWrite(yellowLed, sensorValue);
}

void displayLCD(){
  Serial.println("btn Clicked");
  option *= -1;
}

void loop() {
  yellowTurn();
  if(option == 1){
    readTempHum();
  }
}