#include <WiFi.h>
#include <HTTPClient.h>
#include <Wire.h>
#include <PCA9634.h>
#include <BH1750.h>

#define I2C_HUB_ADDR        0x70
#define EN_MASK             0x08
#define DEF_CHANNEL         0x00
#define MAX_CHANNEL         0x08


const char WIFI_SSID[] = "TP-Link_4F90";         // CHANGE IT
const char WIFI_PASSWORD[] = "NTOContest202324"; // CHANGE IT

String HOST_NAME   = "http://YOUR_DOMAIN.com"; // CHANGE IT
String PATH_NAME_1   = "/lampa1";      // CHANGE IT
String PATH_NAME_2   = "/lampa2";      // CHANGE IT
String PATH_NAME_3   = "/lampa3";      // CHANGE IT
String PATH_NAME_4   = "/lampa4";      // CHANGE IT
//String PATH_NAME   = "/products/arduino.php";      // CHANGE IT
String queryString = "value=false";

BH1750 LightSensor_1;
BH1750 LightSensor_2;
BH1750 LightSensor_3;
BH1750 LightSensor_4;


void setup() {
  Wire.begin();
  Serial.begin(9600);
  delay(1000);

    // Инициализация датчика #1
  setBusChannel(0x07); // функция смены I2C-порта
  LightSensor_1.begin();

  // Инициализация датчика #2
  setBusChannel(0x06); // функция смены I2C-порта
  LightSensor_2.begin();

  // Инициализация датчика #3
  setBusChannel(0x05); // функция смены I2C-порта
  LightSensor_3.begin();

  // Инициализация датчика #4
  setBusChannel(0x04); // функция смены I2C-порта
  LightSensor_4.begin();


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

  http.begin(HOST_NAME + PATH_NAME_1);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  int httpCode = http.POST(queryString);

  // httpCode will be negative on error
  if (httpCode > 0) {
    // file found at server
    if (httpCode == HTTP_CODE_OK) {
      String payload = http.getString();
      Serial.println(payload);
    } else {
      // HTTP header has been send and Server response header has been handled
      Serial.printf("[HTTP] POST... code: %d\n", httpCode);
    }
  } else {
    Serial.printf("[HTTP] POST... failed, error: %s\n", http.errorToString(httpCode).c_str());
  }

  http.end();



}

void loop() {
    // Считывание датчиков
  setBusChannel(0x07);
  float light1 = LightSensor_1.readLightLevel();
  setBusChannel(0x06);
  float light2 = LightSensor_2.readLightLevel();
  setBusChannel(0x05);
  float light3 = LightSensor_3.readLightLevel();
  setBusChannel(0x04);
  float light4 = LightSensor_4.readLightLevel();
  setBusChannel(0x03);
  float light5 = LightSensor_5.readLightLevel();
  
  Serial.println("Ambient light intensity 1: " + String(light1, 1) + " lx");
  Serial.println("Ambient light intensity 2: " + String(light2, 1) + " lx");
  Serial.println("Ambient light intensity 3: " + String(light3, 1) + " lx");
  Serial.println("Ambient light intensity 4: " + String(light4, 1) + " lx");
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