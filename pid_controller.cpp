#include "config.h"

// PID制御用の変数をこのファイルで定義します。
// 他ファイルからはpid_controller.hを通して extern宣言で参照されます。
double pidSetpoint = SETPOINT_TEMP;
double pidInput = 0.0;
double pidOutput = 0.0;

PID pid(&pidInput, &pidOutput, &pidSetpoint, KP, KI, KD, DIRECT);

// タイマー
unsigned long lastHeaterControlTime = 0;

/**
 * @brief PIDコントローラーを初期化する
 */
void initPid() {
  pid.SetMode(AUTOMATIC);
  pid.SetOutputLimits(HEATER_MIN_OUTPUT, HEATER_MAX_OUTPUT);

  // PWM周波数をAC電源周波数(50Hz)と干渉しにくいように調整
  // Arduino UnoのPin 9, 10のデフォルト周波数は490Hz
  // 50Hzより十分長くするため、30.64Hzに設定
  TCCR1B = TCCR1B & 0b11111000 | 0x05;
}

/**
 * @brief ヒーター制御を処理する
 * @param current_temp 現在の温度。PIDの入力として使用する。
 */
void handleHeater(double current_temp) {
  if (millis() - lastHeaterControlTime >= HEATER_CONTROL_INTERVAL) {
    lastHeaterControlTime = millis();

    pidInput = current_temp;
    pid.Compute();

    // PIDの出力(0-10)をPWMのデューティサイクル(0-255)にマッピング
    int pwmValue = map(pidOutput, HEATER_MIN_OUTPUT, HEATER_MAX_OUTPUT, 0, 255);
    analogWrite(HEATER_PIN, pwmValue);
  }
}
