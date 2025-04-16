#include "SparkFun_SGP30_Arduino_Library.h"
#include <Wire.h>
#include <WiFi.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <HTTPClient.h>
#include <FastLED.h>
#include <VL53L0X.h>
Adafruit_BME280 bme280;
SGP30 mySensor;
VL53L0X lox1;
VL53L0X lox2;

#define NUM_LEDS 3;
#define PIN 14;
#define PIN1 15;
unsigned long myTime;
int rec = 15;
int trfc1 = 0;
int trfc2 = 0;

CRGB leds1[NUM_LEDS];
CRGB leds2[NUM_LEDS];

// Я НЕ СДЕЛАЛ РАЗНЫЕ ПИНЫ НА СВЕТОФОР И НА РАССТОЯНИЕ 
// НО ПОКА ОНИ БУДУТ lox1 и lox2 это растояние, leds1 и leds2;

const char* ssid = "TP-Link_4F90";
const char* password = "NTOContest202324";

String HOST_NAME   = "http://YOUR_DOMAIN.com"; // CHANGE IT
String PATH_NAME1  = "/temperature";
String PATH_NAME2  = "/pressure";
String PATH_NAME3  = "/humidity";
String PATH_NAME4  = "/co2";
String PATH_NAME5  = "/traffic1";
String PATH_NAME6  = "/traffic2";

void setup() {
  Serial.begin(9600);
  myTime = micros();
  if (((myTime / 1000000) % 60) == 0) {
      rec = trf1 / (distance1(trfc1) + distance2(trfc2)) * 30;
  }
  FastLED.addLeds <WS2812, PIN, GRB>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(50);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
  }
  Serial.println("WiFi подключен!");
  Wire.begin();
  lox1.init();
  lox2.init();
  lox1.setTimeout(500);
  lox1.setMeasurementTimingBudget(200000);
  lox2.setTimeout(500);
  lox2.setMeasurementTimingBudget(200000);
  if (mySensor.begin() == false) {
    Serial.println("No SGP30 Detected. Check connections.");
    while (1);
  }
  mySensor.initAirQuality();
  // Инициализация датчика
  bool bme_status = bme280.begin();
  if (!bme_status) {
    Serial.println("Не найден по адресу 0х77, пробую другой...");
    bme_status = bme280.begin(0x76);
    if (!bme_status)
      Serial.println("Датчик не найден, проверьте соединение");
  }
}

void loop() {
  leds1[2] = CRGB::Red;
  FastLED.show();
  delay(rec * 1000);
  leds1[2] = CRGB( 0, 0, 0);
  FastLED.show();

  leds2[2] = CRGB::Green;
  FastLED.show();
  delay(rec * 1000);
  leds2[2] = CRGB( 0, 0, 0);
  FastLED.show();

  leds1[1] = CRGB::Yellow;
  FastLED.show();
  delay(2000);
  leds1[1] = CRGB( 0, 0, 0);
  FastLED.show();

  leds2[1] = CRGB::Yellow;
  FastLED.show();
  delay(2000);
  leds2[1] = CRGB( 0, 0, 0);
  FastLED.show();

  leds1[0] = CRGB::Green;
  FastLED.show();
  delay(30000 - (rec * 1000));
  leds1[0] = CRGB( 0, 0, 0);
  FastLED.show();

  leds2[0] = CRGB::Red;
  FastLED.show();
  delay(30000 - (rec * 1000));
  leds2[0] = CRGB( 0, 0, 0);
  FastLED.show();

  leds1[1] = CRGB::Yellow;
  FastLED.show();
  delay(2000);
  leds1[1] = CRGB( 0, 0, 0);
  FastLED.show();

  leds2[1] = CRGB::Yellow;
  FastLED.show();
  delay(2000);
  leds2[1] = CRGB( 0, 0, 0);
  FastLED.show();

  // первые 15 измерений будут неизменными
  delay(1000);
  float dist1 = lox1.readRangeSingleMillimeters();
  float dist2 = lox2.readRangeSingleMillimeters();
  Serial.println("Distance  = " + String(dist1, 0) + " mm  ");
  Serial.println("Distance  = " + String(dist2, 0) + " mm  ");
  mySensor.measureAirQuality();
  Serial.print(mySensor.CO2);
  float co2 = mySensor.CO2;
  // Измерение
  float temperature = bme280.readTemperature();
  float humidity = bme280.readHumidity();
  float pressure = bme280.readPressure() / 100.0F;
  // Вывод измеренных значений в терминал
  Serial.println("Air temperature = " + String(temperature, 1) + " *C");
  Serial.println("Air humidity = " + String(humidity, 1) + " %");
  Serial.println("Air pressure = " + String(pressure, 1) + " hPa"); // 1 mmHg = 1 hPa / 1.33
  if (WiFi.status() == WL_CONNECTED) {
        sendSensorData(co2, temperature, humidity, pressure, trf1, trf2;
  }
  // СДЕЛАЙТЕ ПРОСТО СЧЕТЧИК МАШИН И НАЗОВИТЕ ИХ trf1 и trf2
  delay(250);
}

void distance1(int trfc1) {
  bool h = false;
  if (lox1.readRangeSingleMillimeters() < #######) {
      h = true;
  }
  if (h && lox1.readRangeSingleMillimeters() > #######) {
      trfc1 += 1
  }
  if (lox1.readRangeSingleMillimeters() > #######) {
      h = false;
  }
}

void distance2(int trfc2) {
  bool h1 = false;
  if (lox2.readRangeSingleMillimeters() < #######) {
      h1 = true;
  }
  if (h1 && lox2.readRangeSingleMillimeters() > #######) {
      trfc2 += 1
  }
  if (lox2.readRangeSingleMillimeters() > #######) {
      h1 = false;
  }
}

void sendSensorData(float co2, float temp, float hum, float press, float trf1, float trf2) {
    HTTPClient http;
    
    // Отправка температуры
    http.begin(HOST_NAME + PATH_NAME1);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    int httpCode = http.POST("temperature=" + String(temp, 1));
    processResponse(http, httpCode);
    http.end();

    // Отправка влажности
    http.begin(HOST_NAME + PATH_NAME3);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    httpCode = http.POST("humidity=" + String(hum, 1));
    processResponse(http, httpCode);
    http.end();

    // Отправка давления
    http.begin(HOST_NAME + PATH_NAME2);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    httpCode = http.POST("pressure=" + String(press, 1));
    processResponse(http, httpCode);
    http.end();

    // Отправка CO2
    http.begin(HOST_NAME + PATH_NAME4);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    httpCode = http.POST("co2=" + String(co2, 1));
    processResponse(http, httpCode);
    http.end();

    // Отправка traffic1
    http.begin(HOST_NAME + PATH_NAME5);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    httpCode = http.POST("traffic1=" + String(trf1, 1));
    processResponse(http, httpCode);
    http.end();

    // Отправка traffic2
    http.begin(HOST_NAME + PATH_NAME6);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    httpCode = http.POST("traffic2=" + String(trf2, 1));
    processResponse(http, httpCode);
    http.end();
}

void processResponse(HTTPClient &http, int httpCode) {
    if (httpCode > 0) {
        if (httpCode == HTTP_CODE_OK) {
            Serial.println("Данные отправлены!");
        } else {
            Serial.printf("Ошибка HTTP: %d\n", httpCode);
        }
    } else {
        Serial.printf("[HTTP] POST... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }
}