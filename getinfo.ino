#include <WiFi.h>
#include <HTTPClient.h>

const char WIFI_SSID[] = "TP-Link_4F90";         
const char WIFI_PASSWORD[] = "NTOContest202324"; 

 String HOST_NAME   = "http://192.168.0.17:8000/"; 
 String PATH_NAME   = "/humidity";     
String queryString = "";

void setup() {
  Serial.begin(9600);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }


  HTTPClient http;

http.begin(HOST_NAME + PATH_NAME + "?" + queryString);
 // http.begin(HOST_NAME + PATH_NAME + "?" + queryString+x);
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
Serial.println(queryString);
  http.end();
}

void loop() {
  
}