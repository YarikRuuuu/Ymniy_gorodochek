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
String queryString = "value=10";

BH1750 LightSensor_1;
BH1750 LightSensor_2;
BH1750 LightSensor_3;
BH1750 LightSensor_4;

PCA9634 fonar7(0x1C);
PCA9634 fonar6(0x1C);
PCA9634 fonar5(0x1C);
PCA9634 fonar4(0x1C);


void setup() {
  Wire.begin();
  Serial.begin(9600);
  delay(1000);



  setBusChannel(0x07);
  fonar7.begin();
  fonar7.setLedDriverMode(0, PCA9634_LEDON);

  setBusChannel(0x06);
  fonar6.begin();
  fonar6.setLedDriverMode(0, PCA9634_LEDON);
  
  setBusChannel(0x05);
  fonar5.begin();
  fonar5.setLedDriverMode(0, PCA9634_LEDON);
  
  setBusChannel(0x04);
  fonar4.begin();
  fonar4.setLedDriverMode(0, PCA9634_LEDON);

  setBusChannel(0x03);
  LightSensor_1.begin();

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

  http.end();
}

void loop() {
  setBusChannel(0x03);
  float light = LightSensor_1.readLightLevel();
  Serial.println("Ambient light intensity 1: " + String(light, 1) + " lx");
  
  /*
  setBusChannel(0x07);
  fonar7.setLedDriverMode(0, PCA9634_LEDOFF);
  setBusChannel(0x06);
  fonar6.setLedDriverMode(0, PCA9634_LEDOFF);
  setBusChannel(0x05);
  fonar5.setLedDriverMode(0, PCA9634_LEDOFF);
  setBusChannel(0x04);
  fonar4.setLedDriverMode(0, PCA9634_LEDOFF);
  */
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