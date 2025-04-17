#include "SparkFun_SGP30_Arduino_Library.h"
#include <FastLED.h>
#include <Wire.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <FastLED.h>
#include <VL53L0X.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

Adafruit_BME280 bme280;
SGP30 mySensor;
VL53L0X lox_1;
VL53L0X lox_2;


#define num_LEDS 3
#define PIN_1 14
#define PIN_2 15

/*
  I2C порт 0x07 - выводы GP16 (SDA), GP17 (SCL)
  I2C порт 0x06 - выводы GP4 (SDA), GP13 (SCL)
  I2C порт 0x05 - выводы GP14 (SDA), GP15 (SCL)
  I2C порт 0x04 - выводы GP5 (SDA), GP23 (SCL)
  I2C порт 0x03 - выводы GP18 (SDA), GP19 (SCL)
*/

#define I2C_HUB_ADDR        0x70
#define EN_MASK             0x08
#define DEF_CHANNEL         0x00
#define MAX_CHANNEL         0x08

CRGB leds_1[num_LEDS];
CRGB leds_2[num_LEDS];

int httpCode;
int temp = -1;
int svetoforTime = 15000;


const char WIFI_SSID[] = "TP-Link_4F90";         // CHANGE IT
const char WIFI_PASSWORD[] = "NTOContest202324"; // CHANGE IT

HTTPClient http;
String HOST_NAME   = "http://192.168.0.17:8000"; 
String PATH_NAME_CO2   = "/co2";
String PATH_NAME_FLAG1   = "/traffic1_duration";
String PATH_NAME_FLAG2   = "/traffic2_duration";
String PATH_NAME_TRAFFIC1   = "/traffic1";
String PATH_NAME_TRAFFIC2   = "/traffic2";
String traffic_1 = "";
String traffic_2 = "";
String co2 = "";
String traffic1_duration = "";
String traffic2_duration = "";
String QUERY = "value=";

int count1 = 0;
int count2 = 0;


void setup() {

  Serial.begin(9600);
  Wire.begin();

  Serial.println("New boot");

  http.begin(HOST_NAME + PATH_NAME_FLAG1);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  httpCode = http.GET();
  Serial.println("traffic1_duration: "+traffic1_duration);
  http.end();

  http.begin(HOST_NAME + PATH_NAME_FLAG2);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  httpCode = http.GET();
  Serial.println("traffic1_duration: "+traffic2_duration);
  http.end();

  if (traffic1_duration.length() != 7){
    temp = traffic1_duration.toInt();
  }
  if (traffic2_duration.length() != 7){
    temp = traffic2_duration.toInt();
  }


  FastLED.addLeds <WS2812, PIN_1, GRB>(leds_1, num_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.addLeds <WS2812, PIN_2, GRB>(leds_2, num_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(50);
  

  setBusChannel(0x07);
  lox_1.init();
  lox_1.setTimeout(500);
  // параметры для режима высокой точности
  lox_1.setMeasurementTimingBudget(200000);

  setBusChannel(0x06);
  lox_2.init();
  lox_2.setTimeout(500);
  // параметры для режима высокой точности
  lox_2.setMeasurementTimingBudget(200000);

  setBusChannel(0x05);
  bool bme_status = bme280.begin(0x77);
  if (!bme_status) {
    Serial.println("Not found THP80 at 0x77...");
    bme_status = bme280.begin(0x76);
    if (!bme_status)
      Serial.println("No THP80 sensor Detected. Check connections.");
  }


  setBusChannel(0x04);
  if (mySensor.begin() == false) {
    Serial.println("No CO2 sensor Detected. Check connections.");
  }
  mySensor.initAirQuality();
}

void loop() {

  if (temp == -1){
    setBusChannel(0x04);
    mySensor.measureAirQuality();
    Serial.print("CO2: ");
    Serial.print(mySensor.CO2);
    Serial.print(" ppm\tTVOC: ");
    Serial.print(mySensor.TVOC);
    Serial.println(" ppb");


    http.begin(HOST_NAME + PATH_NAME_CO2 + "?" + QUERY + co2);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    httpCode = http.GET();
    Serial.println("co2: "+co2);
    http.end();

    if (mySensor.CO2 <= 600){
      Serial.println("CO2 concentration level: 1");
    }
    else if (mySensor.CO2 <= 1000){
      Serial.println("CO2 concentration level: 2");
    }
    else if (mySensor.CO2 > 1000){
      Serial.println("CO2 concentration level: 3");
    }

    setBusChannel(0x07);
    float dist_1 = lox_1.readRangeSingleMillimeters();
    // Вывод измеренных значений в терминал
    Serial.println("Distance_1  = " + String(dist_1, 0) + " mm  ");

    setBusChannel(0x06);
    float dist_2 = lox_2.readRangeSingleMillimeters();
    // Вывод измеренных значений в терминал
    Serial.println("Distance_2  = " + String(dist_2, 0) + " mm  ");

  /*
    setBusChannel(0x05);
    float t = bme280.readTemperature();
    float h = bme280.readHumidity();
    float p = bme280.readPressure() / 100.0F;
    // Вывод измеренных значений в терминал
    Serial.println("Air temperature = " + String(t, 1) + " *C");
    Serial.println("Air humidity = " + String(h, 1) + " %");
    Serial.println("Air pressure = " + String(p, 1) + " hPa"); // 1 mmHg = 1 hPa / 1.33
    */

    Serial.println("Traffic light is Red/Green");
    light(1, 3, 1); //красный на 1
      light(2, 3, 3); //зеленый на 2
    delay(svetoforTime);
    light(1, 3, 0); 
      light(2, 3, 0);

    light(1, 2, 2); //желтый после красного на 1
      light(2, 1, 1); //красный после зеленого на 2
    delay(2000);
    light(1, 2, 0);
      light(2, 1, 0);

    Serial.println("Traffic light is Green/Red");
    light(1, 1, 3); //зеленый после желтого на 1
      light(2, 1, 1); // еще красный после зеленого на 2
    delay(svetoforTime-2000);
    light(1, 1, 0);
      light(2, 1, 0);

    light(1, 1, 3); // еще зеленый после желтого на 1
      light(2, 2, 2); //желтый после красного на 2
    delay(2000);
    light(1, 1, 0);
      light(2, 2, 0);
  } else{

    http.begin(HOST_NAME + PATH_NAME_TRAFFIC1);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    httpCode = http.GET();
    Serial.println("count1: "+count1);
    http.end();

    http.begin(HOST_NAME + PATH_NAME_TRAFFIC2);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    httpCode = http.GET();
    Serial.println("count2: "+count2);
    http.end();

    svetoforTime = 30000*count1/(count1 + count2);
  }

  
}


bool setBusChannel(uint8_t i2c_channel)
{
  if (i2c_channel >= MAX_CHANNEL)
  {
    return false;
  }
  else
  {

  }
}

void light(int leds, int num, int color) {
  if (leds == 1){
    if (color == 0){
        leds_1[num-1] = CRGB(0,0,0);
    }
    if (color == 1){
        leds_1[num-1] = CRGB::Red;
    }
    else if (color == 2){
        leds_1[num-1] = CRGB::Yellow;
    }
    else if (color == 3){
        leds_1[num-1] = CRGB::Green;
    }
  }

  if (leds == 2){
    if (color == 0){
        leds_2[num-1] = CRGB(0,0,0);
    }
    if (color == 1){
        leds_2[num-1] = CRGB::Red;
    }
    else if (color == 2){
        leds_2[num-1] = CRGB::Yellow;
    }
    else if (color == 3){
        leds_2[num-1] = CRGB::Green;
    }
  }
  FastLED.show();
}
