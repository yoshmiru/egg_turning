#include <ESP32Servo.h>

// README.mdの要件に基づいてピンと定数を定義
#define SERVO_PIN 10
#define BUTTON_PIN 9
#define ROTATION_INTERVAL (90 * 60 * 1000UL) // 90分 (unsigned long)

Servo sg90;
int currentAngle = 0; // 現在のサーボの角度 (0 or 180)
unsigned long lastRotationTime = 0; // 最後にサーボが回転した時刻

void setup() {
  Serial.begin(115200); // デバッグ用にシリアル通信を開始

  // サーボモーターをGPIO10に接続 (min/max pulse widthは元のコードの値を参考に設定)
  sg90.attach(SERVO_PIN, 510, 2400);

  // ボタンをGPIO9に接続し、内蔵プルアップ抵抗を有効にする
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  // 初期位置にサーボを移動
  sg90.write(currentAngle);
  Serial.println("Servo set to initial position: 0 degrees");

  // タイマーを初期化
  lastRotationTime = millis();
}

void loop() {
  // 90分経過したかチェック
  if (millis() - lastRotationTime >= ROTATION_INTERVAL) {
    rotateServo();
  }

  // ボタンが押されたかチェック (INPUT_PULLUPなのでLOWが押された状態)
  if (digitalRead(BUTTON_PIN) == LOW) {
    // 簡単なデバウンス処理
    delay(50);
    if (digitalRead(BUTTON_PIN) == LOW) {
      Serial.println("Button pressed. Rotating servo.");
      rotateServo();
      // ボタンが離されるまで待つ
      while(digitalRead(BUTTON_PIN) == LOW) {
        delay(10);
      }
    }
  }
}

// サーボの角度を0度と180度の間で切り替える関数
void rotateServo() {
  if (currentAngle == 0) {
    currentAngle = 180;
  } else {
    currentAngle = 0;
  }

  sg90.write(currentAngle);
  Serial.print("Servo moved to: ");
  Serial.println(currentAngle);

  // 最後の回転時刻を更新
  lastRotationTime = millis();
}