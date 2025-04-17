#include <Wire.h>
#include <HTTPClient.h>
#include <VL53L0X.h>

VL53L0X lox_1;
VL53L0X lox_2;

int httpCode;
int startTime = 0;

const char WIFI_SSID[] = "TP-Link_4F90";         // CHANGE IT
const char WIFI_PASSWORD[] = "NTOContest202324"; // CHANGE IT

HTTPClient http;
String HOST_NAME   = "http://192.168.0.17:8000"; 
String PATH_NAME_TRAFFIC1   = "/traffic1";
String PATH_NAME_TRAFFIC2   = "/traffic2";
String traffic_1 = "";
String traffic_2 = "";
String dist_1 = "";
String dist_2 = "";
String QUERY = "value=";

int count1 = 0;
int count2 = 0;


void setup() {
  Serial.begin(9600);
  Wire.begin();

  lox_1.init();
  lox_1.setTimeout(500);
  // параметры для режима высокой точности
  lox_1.setMeasurementTimingBudget(200000);

  lox_2.init();
  lox_2.setTimeout(500);
  // параметры для режима высокой точности
  lox_2.setMeasurementTimingBudget(200000);
}

void loop() {
  float dist1 = lox_1.readRangeSingleMillimeters();
  // Вывод измеренных значений в терминал
  Serial.println("Distance_1  = " + String(dist1, 0) + " mm  ");

  float dist2 = lox_2.readRangeSingleMillimeters();
  // Вывод измеренных значений в терминал
  Serial.println("Distance_2  = " + String(dist2, 0) + " mm  ");
  
  if (dist1 < 200){
    count1 += 1;
    delay(2000);
  }
  else if (dist2 < 200){
    count2 += 1;
    delay(2000);
  }

  if ((startTime+60000) < millis()){
    startTime = millis();
  } else{
    http.begin(HOST_NAME + PATH_NAME_TRAFFIC1 + "?" + QUERY + String(count1, 1));
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    httpCode = http.GET();
    Serial.println("count1: "+count1);
    http.end();

    http.begin(HOST_NAME + PATH_NAME_TRAFFIC2 + "?" + QUERY + String(count2, 1));
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    httpCode = http.GET();
    Serial.println("count2: "+count2);
    http.end();
  }
}
