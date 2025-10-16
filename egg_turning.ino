#include "config.h"
#include "pid_controller.h"
#include "lcd_display.h"
#include "sensor_reader.h"

// グローバル変数
// ----------------------------------------------------------------

// サーボ
Servo sg90;
int currentAngle = LEFT_ANGLE;
unsigned long lastRotationTime = 0;

// センサーデータ
float temperature = 0.0;
float humidity = 0.0;


// プロトタイプ宣言
// ----------------------------------------------------------------
void rotateServo();
void handleButton();
void handleAutomaticRotation();


// メイン処理
// ----------------------------------------------------------------

void setup() {
  Serial.begin(115200);

  // サーボ
  sg90.attach(SERVO_PIN, 510, 2400);
  sg90.write(currentAngle);
  lastRotationTime = millis();

  // ボタン
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  // ヒーターピン
  pinMode(HEATER_PIN, OUTPUT);

  // センサー
  initSensor();

  // PID
  initPid();

  // LCD
  initLcd();
}

void loop() {
  handleButton();
  handleAutomaticRotation();
  handleSensor(&temperature, &humidity); // センサー値をポインタ経由で更新
  handleHeater(temperature);             // 現在温度を引数として渡す
  handleLcd(temperature, humidity, pidOutput); // 表示するデータを引数として渡す
}


// 関数定義
// ----------------------------------------------------------------

/**
 * @brief サーボの角度をLEFT_ANGLEとRIGHT_ANGLEの間で切り替える
 */
void rotateServo() {
  currentAngle = (currentAngle == LEFT_ANGLE) ? RIGHT_ANGLE : LEFT_ANGLE;
  sg90.write(currentAngle);
  Serial.print("Servo moved to: ");
  Serial.println(currentAngle);
  lastRotationTime = millis();
}

/**
 * @brief ボタン入力を処理する
 */
void handleButton() {
  if (digitalRead(BUTTON_PIN) == LOW) {
    delay(50); // 簡単なデバウンス
    if (digitalRead(BUTTON_PIN) == LOW) {
      Serial.println("Button pressed. Rotating servo.");
      rotateServo();
      while (digitalRead(BUTTON_PIN) == LOW) {
        delay(10); // ボタンが離されるまで待つ
      }
    }
  }
}

/**
 * @brief サーボの自動回転を処理する
 */
void handleAutomaticRotation() {
  if (millis() - lastRotationTime >= ROTATION_INTERVAL) {
    Serial.println("Interval reached. Rotating servo.");
    rotateServo();
  }
}
