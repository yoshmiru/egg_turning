#include <ESP32Servo.h>

Servo sg90;　//Servoクラスの型の変数を宣言

void setup() {
  // put your setup code here, to run once:
  sg90.attach(SERVO_PIN,510,2400); //サーボモーターの初期化
  sg90.write(90);
}

void loop() {
  // put your main code here, to run repeatedly:

}
