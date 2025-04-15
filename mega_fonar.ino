#include <Wire.h>
#include <PCA9634.h>

PCA9634 testModule(0x1C);

void setup() {
  Serial.begin(9600);
  testModule.begin();
  testModule.setLedDriverMode(0, PCA9634_LEDOFF);
}

void loop() {
  testModule.setLedDriverMode(0, PCA9634_LEDON);
}