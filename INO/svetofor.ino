#include "SparkFun_SGP30_Arduino_Library.h"
#include <FastLED.h>
#include <Wire.h>
#include <WiFi.h>
#include <FastLED.h>
#include <VL53L0X.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

Adafruit_BME280 bme280;
SGP30 mySensor;
VL53L0X lox_1;
VL53L0X lox_2;


#define NUM_LEDS 3
#define PIN_1 14
#define PIN_2 15

CRGB leds_1[NUM_LEDS];
CRGB leds_2[NUM_LEDS];


const char WIFI_SSID[] = "TP-Link_4F90";         // CHANGE IT
const char WIFI_PASSWORD[] = "NTOContest202324"; // CHANGE IT

String HOST_NAME   = "http://YOUR_DOMAIN.com"; // CHANGE IT
String PATH_TEMP   = "/temperature";      // CHANGE IT
String PATH_HUM   = "/humidity";      // CHANGE IT
String PATH_CO2   = "/co2";      // CHANGE IT
String PATH_TRAF_1   = "/traffic1";      // CHANGE IT
String PATH_TRAF_2   = "/traffic2";      // CHANGE IT
//String PATH_NAME   = "/products/arduino.php";      // CHANGE IT
String queryString = "value=false";

void setup() {
  FastLED.addLeds <WS2812, PIN_1, GRB>(leds_1, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.addLeds <WS2812, PIN_2, GRB>(leds_2, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(50);

  lox_1.init();
  lox_1.setTimeout(500);
  // параметры для режима высокой точности
  lox_1.setMeasurementTimingBudget(200000);

  lox_2.init();
  lox_2.setTimeout(500);
  // параметры для режима высокой точности
  lox_2.setMeasurementTimingBudget(200000);

  bool bme_status = bme280.begin();
  if (!bme_status) {
    Serial.println("Не найден по адресу 0х77, пробую другой...");
    bme_status = bme280.begin(0x76);
    if (!bme_status)
      Serial.println("Датчик не найден, проверьте соединение");
  }
  if (mySensor.begin() == false) {
    Serial.println("No SGP30 Detected. Check connections.");
    while (1);
  }
  mySensor.initAirQuality();
}

void loop() {
  leds_1[2] = CRGB::Red;
  FastLED.show();
  delay(15000);
  leds_1[2] = CRGB( 0, 0, 0);
  FastLED.show();

  leds_1[1] = CRGB::Yellow;
  FastLED.show();
  delay(2000);
  leds_1[1] = CRGB( 0, 0, 0);
  FastLED.show();

  leds_1[0] = CRGB::Green;
  FastLED.show();
  delay(15000);
  leds_1[0] = CRGB( 0, 0, 0);
  FastLED.show();

  leds_1[1] = CRGB::Yellow;
  FastLED.show();
  delay(2000);
  leds_1[1] = CRGB( 0, 0, 0);
  FastLED.show();

  float dist_1 = lox_1.readRangeSingleMillimeters();
  float dist_2 = lox_2.readRangeSingleMillimeters();
  Serial.println("Distance  = " + String(dist_1, 0) + " mm  ");
  Serial.println("Distance  = " + String(dist_2, 0) + " mm  ");

  float t = bme280.readTemperature();
  float h = bme280.readHumidity();
  float p = bme280.readPressure() / 100.0F;
  // Вывод измеренных значений в терминал
  Serial.println("Air temperature = " + String(t, 1) + " *C");
  Serial.println("Air humidity = " + String(h, 1) + " %");
  Serial.println("Air pressure = " + String(p, 1) + " hPa"); // 1 mmHg = 1 hPa / 1.33

  mySensor.measureAirQuality();
  Serial.print("CO2: ");
  Serial.print(mySensor.CO2);
  Serial.print(" ppm\tTVOC: ");
  Serial.print(mySensor.TVOC);
  Serial.println(" ppb");

  delay(250);
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