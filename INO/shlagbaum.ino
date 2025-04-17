#include <ESP32Servo.h>
#include <Wire.h>
#include <VL53L0X.h>
#include <PCA9634.h>
#include <BH1750.h>
VL53L0X lox;

Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

PCA9634 fonar(0x1C);

void setup() {
  myservo.attach(13);  // attaches the servo on pin 13 to the servo object
  myservo.write(90);
  fonar.begin();
  Serial.begin(9600);
  // Инициализация датчика
  Wire.begin();
  lox.init();
  lox.setTimeout(500);
  // параметры для режима высокой точности
  lox.setMeasurementTimingBudget(200000);
}

void loop() {
  delay(300);
    float dist = lox.readRangeSingleMillimeters();
  // Вывод измеренных значений aв терминал
  Serial.println("Distance  = " + String(dist, 0) + " mm  ");
  if (dist < 500){
    myservo.write(0);
    fonar.write1(3, 255);
  }
  else{
    myservo.write(90);
    fonar.write1(2, 255);
      /* leds.setBrightness(x, P);
    x - светодиод:
    0,6 - белые
    1, 4 - УФ
    3 - красный RGB
    2 - зеленый RGB
    5 - синий RGB
    P - яркость (0x00-0xff) (0-255)
  */
  }
}
