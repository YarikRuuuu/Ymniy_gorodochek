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

/*
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
*/

void setup() {

  Serial.begin(9600);
  Wire.begin();

  Serial.println("New boot");


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

  setBusChannel(0x04);
  mySensor.measureAirQuality();
  Serial.print("CO2: ");
  Serial.print(mySensor.CO2);
  Serial.print(" ppm\tTVOC: ");
  Serial.print(mySensor.TVOC);
  Serial.println(" ppb");

  setBusChannel(0x07);
  float dist_1 = lox_1.readRangeSingleMillimeters();
  // Вывод измеренных значений в терминал
  Serial.println("Distance  = " + String(dist_1, 0) + " mm  ");

  setBusChannel(0x06);
  float dist_2 = lox_2.readRangeSingleMillimeters();
  // Вывод измеренных значений в терминал
  Serial.println("Distance  = " + String(dist_2, 0) + " mm  ");

  light(1, 3, 1);
    light(2, 3, 3);
  delay(2000);
  light(1, 3, 0);
    light(2, 3, 0);

  light(1, 2, 2);
    light(2, 2, 2);
  delay(2000);
  light(1, 2, 0);
    light(2, 2, 0);


  light(1, 1, 3);
    light(2, 1, 1);
  delay(2000);
  light(1, 1, 0);
    light(2, 1, 0);

  light(1, 2, 2);
    light(2, 2, 2);
  delay(2000);
  light(1, 2, 0);
    light(2, 2, 0);
}


bool setBusChannel(uint8_t i2c_channel)
{
  if (i2c_channel >= MAX_CHANNEL)
  {
    return false;
  }
  else
  {
    Wire.beginTransmission(I2C_HUB_ADDR);
    Wire.write(i2c_channel | EN_MASK); // для микросхемы PCA9547
	// Wire.write(0x01 << i2c_channel); // Для микросхемы PW548A
    Wire.endTransmission();
    return true;
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
