#include <ESP32Servo.h>
#include <Wire.h>
#include <VL53L0X.h>
#include <PCA9634.h>
#include <BH1750.h>
#include <WiFi.h>
#include <HTTPClient.h>
VL53L0X lox;

Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

PCA9634 fonar(0x1C);

HTTPClient http;

int httpCode;

String HOST_NAME   = "http://192.168.0.17:8000"; 
String PATH_NAME_FLAG   = "/barrier_control";
String PATH_NAME_BARRIER   = "/barrier_automation";
String barrier_control = "";
String barrier_automation = "";
String QUERY = "value=";

void setup() {
  myservo.attach(13);  // attaches the servo on pin 13 to the servo object
  myservo.write(90);
  fonar.begin();
  Serial.begin(9600);
  // Инициализация датчика
  Wire.begin();
  lox.init();
  lox.setTimeout(500);
  // параметры для режима высокой точности
  lox.setMeasurementTimingBudget(200000);

  http.begin(HOST_NAME + PATH_NAME_BARRIER);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  httpCode = http.GET();
  Serial.println("barrier_automation: "+barrier_automation);
  http.end();
}

void loop() {

  if (barrier_automation == "false"){
    float dist = lox.readRangeSingleMillimeters();
    // Вывод измеренных значений aв терминал
    Serial.println("Distance  = " + String(dist, 0) + " mm  ");
    if (dist < 500){
      myservo.write(0);
      fonar.write1(3, 255);
    }
    else{
      myservo.write(90);
      fonar.write1(2, 255);
        /* leds.setBrightness(x, P);
      x - светодиод:
      0,6 - белые
      1, 4 - УФ
      3 - красный RGB
      2 - зеленый RGB
      5 - синий RGB
      P - яркость (0x00-0xff) (0-255)
    */
    }
  } else if (barrier_automation == "true"){
      
      http.begin(HOST_NAME + PATH_NAME_FLAG);
      http.addHeader("Content-Type", "application/x-www-form-urlencoded");
      httpCode = http.GET();
      Serial.println("barrier_control: "+barrier_control);
      http.end();

      if (barrier_control == "true"){
        myservo.write(90);
        fonar.write1(2, 255);
      } else if (barrier_control == "false"){
        myservo.write(0);
        fonar.write1(3, 255);
      }
  }
}