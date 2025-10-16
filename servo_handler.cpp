#include "config.h"

// サーボオブジェクトと関連変数をこのファイルで定義
Servo sg90;
int currentAngle = LEFT_ANGLE;
unsigned long lastRotationTime = 0;

// プロトタイプ宣言 (このファイル内でのみ使用)
void rotateServo();

/**
 * @brief サーボとボタンを初期化する
 */
void initServo() {
  sg90.attach(SERVO_PIN, 510, 2400);
  sg90.write(currentAngle);
  lastRotationTime = millis();

  pinMode(BUTTON_PIN, INPUT_PULLUP);
}

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
