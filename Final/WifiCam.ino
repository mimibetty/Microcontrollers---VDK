#include "WifiCam.hpp"
#include <WiFi.h>
#include <WebServer.h>
#include <string.h>

#define STEPS_PER_REV 4096
#define MICROSECONDS_PER_MINUTE 60000000L

int RPM = 15;
unsigned long delayPerStep = 60L * 1000L * 1000L / STEPS_PER_REV / RPM;

#define IN1 12
#define IN2 13
#define IN3 15
#define IN4 14
#define contacHT1 2
#define contacHT2 4
#define rainSensor 16

int MOTOR_PINS[] = {IN1, IN2, IN3, IN4};
bool isRotating = false; 
static const char* WIFI_SSID = "Toan Tri Tue";
static const char* WIFI_PASS = "toantritue";

esp32cam::Resolution initialResolution;

String status = "";
WebServer server(80);

void setup() {
  Serial.begin(115200);
  Serial.println();
  delay(2000);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(contacHT1, INPUT);
  pinMode(contacHT2, INPUT);

  WiFi.persistent(false); 
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  delay(100);
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.printf("WiFi failure %d\n", WiFi.status());
    delay(5000);
    ESP.restart();
  }
  Serial.println("WiFi connected");
  Serial.println(WiFi.localIP());

  server.on("/", handle_OnConnect);
  server.on("/open", handle_open);
  server.on("/close", handle_close);
  server.on("/stop", handle_stop);
  server.on("/reset", handle_reset);
  server.begin();
  Serial.print("HTTP server started");
}
void setSpeed(int newRPM)
{
  RPM = newRPM;
  delayPerStep = 60L * 1000L * 1000L / STEPS_PER_REV / RPM; 
}
void rotateMotor(int steps, bool direction) 
{
  int sequence[][4] = {{1, 0, 0, 0},
                       {1, 1, 0, 0},
                       {0, 1, 0, 0},
                       {0, 1, 1, 0},
                       {0, 0, 1, 0},
                       {0, 0, 1, 1},
                       {0, 0, 0, 1},
                       {1, 0, 0, 1}};

  int stepSequence;
  if (direction) {
    for (int x = 0; x < steps; x++) {
      if (!isRotating || (digitalRead(contacHT1) == 0)) return; 
      stepSequence = x % 8;
      for (int pin = 0; pin < 4; pin++) {
        digitalWrite(MOTOR_PINS[pin], sequence[stepSequence][pin]);
      }
      delayMicroseconds(delayPerStep);
    }  
  } else {
    for (int x = steps - 1; x >= 0; x--) {
      if (!isRotating || (digitalRead(contacHT2) == 0) ) return; 
      stepSequence = x % 8;
      for (int pin = 0; pin < 4; pin++) {
        digitalWrite(MOTOR_PINS[pin], sequence[stepSequence][pin]);
      }
      delayMicroseconds(delayPerStep);
    }
  }
}
void loop() {
  server.handleClient();
  if (status.equals("open")) {
    Serial.print("open\n");
    rotateMotor(15,true);
    isRotating = true;
  }
  else if (status.equals("close")) {
    Serial.print("close\n");
    rotateMotor(15,false); 
    isRotating = true;
  }
  else if (status.equals("stop")) {
    Serial.print("stop\n");
    isRotating = false;
  }
  if (status.equals("")){
    int rain_status = digitalRead(rainSensor);
    if (rain_status == 0) {
      rotateMotor(15,true);
      isRotating = true;
    }
    else {
      rotateMotor(15, false);
      isRotating = true;
    }
  }
  
}
void handle_OnConnect(){
  status = "";
  Serial.print("Connect!");
  server.send(200, "text/html", SendHTML(status));
}
void handle_open(){
  status = "open";
  Serial.print("Open!!");
  server.send(200, "text/html", SendHTML(status));
}
void handle_close(){
  status = "close";
  Serial.print("Close!!");
  server.send(200, "text/html", SendHTML(status));
}
void handle_stop(){
  status = "stop";
  Serial.print("Stop!!");
  server.send(200, "text/html", SendHTML(status));
}
void handle_reset(){
  status = "";
  Serial.print("reset!!");
  server.send(200, "text/html", SendHTML(status));
}
String SendHTML(String stt){
  String ptr = "";
  ptr += "<!DOCTYPE html>\n";
  ptr += "<html lang=\"en\">\n";
  ptr += "<head>\n";
  ptr += "<meta charset=\"UTF-8\">\n";
  ptr += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n";
  ptr += "<title>VDK_Module</title>\n";
  ptr += "<style>\n";
  ptr += "    body {\n";
  ptr += "        font-family: Arial, sans-serif;\n";
  ptr += "        margin: 0;\n";
  ptr += "        padding: 0;\n";
  ptr += "        background-color: #f4f4f4;\n";
  ptr += "    }\n";
  ptr += "\n";
  ptr += "    .container {\n";
  ptr += "        width: 80%;\n";
  ptr += "        margin: 0 auto;\n";
  ptr += "        padding-top: 50px;\n";
  ptr += "    }\n";
  ptr += "\n";
  ptr += "    h1 {\n";
  ptr += "        text-align: center;\n";
  ptr += "        color: #333;\n";
  ptr += "    }\n";
  ptr += "\n";
  ptr += "    h3 {\n";
  ptr += "        text-align: center;\n";
  ptr += "        color: #666;\n";
  ptr += "    }\n";
  ptr += "\n";
  ptr += "    a {\n";
  ptr += "        text-decoration: none;\n";
  ptr += "    }\n";
  ptr += "    .button-container {\n";
  ptr += "        display: flex;\n";
  ptr += "        flex-direction: column;\n";
  ptr += "        align-items: center; \n";
  ptr += "        margin-top: 30px;\n";
  ptr += "    }\n";
  ptr += "\n";
  ptr += "\n";
  ptr += "    .button {\n";
  ptr += "        padding: 10px 20px;\n";
  ptr += "        margin: 10px 10px;\n";
  ptr += "        width: 80px;\n";
  ptr += "        height: 25px;\n";
  ptr += "        border: none;\n";
  ptr += "        border-radius: 5px;\n";
  ptr += "        cursor: pointer;\n";
  ptr += "        font-size: 16px;\n";
  ptr += "        position: relative;\n";
  ptr += "        text-align: center;\n";
  ptr += "    }\n";
  ptr += "\n";
  ptr += "    .button:hover {\n";
  ptr += "        opacity: 0.8;\n";
  ptr += "    }\n";
  ptr += "\n";
  ptr += "    .open {\n";
  ptr += "        background-color: #4CAF50;\n";
  ptr += "        color: white;\n";
  ptr += "    }\n";
  ptr += "\n";
  ptr += "    .close {\n";
  ptr += "        background-color: #f44336;\n";
  ptr += "        color: white;\n";
  ptr += "    }\n";
  ptr += "\n";
  ptr += "    .stop {\n";
  ptr += "        background-color: #FFC107;\n";
  ptr += "        color: white;\n";
  ptr += "    }\n";
  ptr += "\n";
  ptr += "    .reset {\n";
  ptr += "        background-color: #2196F3;\n";
  ptr += "        color: white;\n";
  ptr += "    }\n";
  ptr += "</style>\n";
  ptr += "</head>\n";
  ptr += "<body>\n";
  ptr += "<div class=\"container\">\n";
  ptr += "    <h1>MÁI CHE MƯA THÔNG MINH</h1>\n";
  ptr += "    <h3>CHỨC NĂNG</h3>\n";
  ptr += "    <div class=\"button-container\">\n";
  ptr += "        <a href=\"open\" class=\"button open\">Mở</a>\n";
  ptr += "        <a href=\"close\" class=\"button close\">Đóng</a>\n";
  ptr += "        <a href=\"stop\" class=\"button stop\">Dừng</a>\n";
  ptr += "        <a href=\"reset\" class=\"button reset\">Reset</a>\n";
  ptr += "    </div>\n";
  ptr += "</div>\n";
  ptr += "</body>\n";
  ptr += "</html>\n";
  return ptr;
}