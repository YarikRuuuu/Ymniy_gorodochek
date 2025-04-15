#include "SparkFun_SGP30_Arduino_Library.h"
#include <Wire.h>
#include <WiFi.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <HTTPClient.h>
Adafruit_BME280 bme280;
SGP30 mySensor;
//тут название переменных датчиков светофора,
//пока что будет X1 и X2
int X1;
int X2;

const char* ssid = "ВАШ_SSID";
const char* password = "ВАШ_ПАРОЛЬ";
const char* serverUrl = "http://ваш-сервер.local/endpoint"; // URL сервера

void setup() {
  Serial.begin(9600);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
  }
  Serial.println("WiFi подключен!");
  Wire.begin();
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
  // первые 15 измерений будут неизменными
  delay(1000);
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
        sendSensorData(co2, temperature, humidity, pressure);
  }
  delay(250);
}

void sendSensorData(float co2, float temp, float hum, float press) {
    HTTPClient http;
    
    // Формируем URL с параметрами
    String url = String(serverUrl) + 
                 "?co2=" + String(co2, 1) +
                 "&temp=" + String(temp, 1) + 
                 "&humidity=" + String(hum, 1) + 
                 "&pressure=" + String(press, 1);
    
    http.begin(url);
    int httpCode = http.GET();
    
    if (httpCode == HTTP_CODE_OK) {
        Serial.println("Данные отправлены!");
    } else {
        Serial.printf("Ошибка HTTP: %d\n", httpCode);
    }
    http.end();
}