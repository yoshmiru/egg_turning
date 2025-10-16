#include "config.h"
#include "pid_controller.h"
#include "lcd_display.h"
#include "sensor_reader.h"
#include "servo_handler.h"

// グローバル変数
// ----------------------------------------------------------------

// 各モジュールで管理されない、モジュール間で受け渡されるデータ
float temperature = 0.0;
float humidity = 0.0;


// メイン処理
// ----------------------------------------------------------------

void setup() {
  Serial.begin(115200);

  // 各モジュールの初期化
  initServo();
  initSensor();
  initPid();
  initLcd();
}

void loop() {
  // 各モジュールのループ処理
  handleButton();
  handleAutomaticRotation();
  handleSensor(&temperature, &humidity); // センサー値をポインタ経由で更新
  handleHeater(temperature);             // 現在温度を引数として渡す
  handleLcd(temperature, humidity, pidOutput); // 表示するデータを引数として渡す
}