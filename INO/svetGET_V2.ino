#include <WiFi.h>
#include <HTTPClient.h>
#include <Wire.h>
#include <PCA9634.h>
#include <BH1750.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#define I2C_HUB_ADDR        0x70
#define EN_MASK             0x08
#define DEF_CHANNEL         0x00
#define MAX_CHANNEL         0x08

const char WIFI_SSID[] = "TP-Link_4F90";         // CHANGE IT
const char WIFI_PASSWORD[] = "NTOContest202324"; // CHANGE IT

bool manual = false;

String HOST_NAME   = "192.168.0.17:8000"; // CHANGE IT
String PATH_NAME_1   = "/lampa1";      // CHANGE IT
String PATH_NAME_2   = "/lampa2";      // CHANGE IT
String PATH_NAME_3   = "/lampa3";      // CHANGE IT
String PATH_NAME_4   = "/lampa4";      // CHANGE IT
String queryString = "value=10";

BH1750 LightSensor_7;
BH1750 LightSensor_6;
BH1750 LightSensor_5;
BH1750 LightSensor_4;

PCA9634 fonar7(0x1C);
PCA9634 fonar6(0x0C);
PCA9634 fonar5(0x18);
PCA9634 fonar4(0x14);

Adafruit_BME280 bme280;

void setup() {
  Wire.begin();
  Serial.begin(9600);
  delay(1000);

  fonar7.begin();

  fonar6.begin();

  fonar5.begin();

  fonar4.begin();


  setBusChannel(0x07);
  LightSensor_7.begin();

  setBusChannel(0x06);
  LightSensor_6.begin();

  setBusChannel(0x05);
  LightSensor_5.begin();

  setBusChannel(0x04);
  LightSensor_4.begin();

  setBusChannel(0x03);
  bool bme_status = bme280.begin();
  if (!bme_status) {
    Serial.println("Not found THP80 at 0x77...");
    bme_status = bme280.begin(0x76);
    if (!bme_status)
      Serial.println("No THP80 sensor Detected. Check connections.");
  }

/*
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());

  HTTPClient http;

  http.begin(HOST_NAME + PATH_NAME_1 + "?" + queryString);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  int httpCode = http.GET();

  // httpCode will be negative on error
  if (httpCode > 0) {
    // file found at server
    if (httpCode == HTTP_CODE_OK) {
      String payload = http.getString();
      Serial.println(payload);
    } else {
      // HTTP header has been send and Server response header has been handled
      Serial.printf("[HTTP] GET... code: %d\n", httpCode);
    }
  } else {
    Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
  }

  http.end();*/
}

void loop() {

  setBusChannel(0x07);
  float light_7 = LightSensor_7.readLightLevel();
  setBusChannel(0x06);
  float light_6 = LightSensor_6.readLightLevel();
  setBusChannel(0x05);
  float light_5 = LightSensor_5.readLightLevel();
  setBusChannel(0x04);
  float light_4 = LightSensor_4.readLightLevel();
  Serial.println("Ambient light intensity 7: " + String(light_7, 1) + " lx // " + "6: " + String(light_6, 1) + " lx // " + "5: " + String(light_5, 1) + " lx // " + "4: " + String(light_4, 1) + " lx");
  delay(300);

  if (!manual){
    setBusChannel(0x07);
    float light_7 = LightSensor_7.readLightLevel();
    if (light_7 < 800){
      fonar7.setLedDriverMode(0, PCA9634_LEDON);
    }
    else if (light_7 >= 800) {
      fonar7.setLedDriverMode(0, PCA9634_LEDOFF);
    }
    setBusChannel(0x06);
    float light_6 = LightSensor_6.readLightLevel();
    if (light_6 < 800){
        fonar6.setLedDriverMode(0, PCA9634_LEDON);
    }
    else if (light_6 >= 800) {
      fonar6.setLedDriverMode(0, PCA9634_LEDOFF);
    }
    setBusChannel(0x05);
    float light_5 = LightSensor_5.readLightLevel();
    if (light_5 < 800){
        fonar5.setLedDriverMode(0, PCA9634_LEDON);
    }
    else if (light_5 >= 800) {
      fonar5.setLedDriverMode(0, PCA9634_LEDOFF);
    }
    setBusChannel(0x04);
    float light_4 = LightSensor_4.readLightLevel();
    if (light_4 < 800){
        fonar4.setLedDriverMode(0, PCA9634_LEDON);
    }
    else if (light_4 >= 800) {
      fonar4.setLedDriverMode(0, PCA9634_LEDOFF);
    }
  }

  setBusChannel(0x03);
  float t = bme280.readTemperature();
  float h = bme280.readHumidity();
  float p = bme280.readPressure() / 100.0F;
  // Вывод измеренных значений в терминал
  Serial.println("Air temperature = " + String(t, 1) + " *C");
  Serial.println("Air humidity = " + String(h, 1) + " %");
  Serial.println("Air pressure = " + String(p, 1) + " hPa"); // 1 mmHg = 1 hPa / 1.33
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
