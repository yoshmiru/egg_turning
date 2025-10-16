#include "config.h"

// AHT20センサーオブジェクトをこのファイルで定義
AHTxx aht20(AHTXX_ADDRESS_X38, AHT2x_SENSOR);

// タイマー
unsigned long lastSensorReadTime = 0;

/**
 * @brief AHT20センサーを初期化する
 */
void initSensor() {
  while (aht20.begin() != true) {
    Serial.println(F("AHT2x not connected or fail to load calibration coefficient"));
    delay(5000);
  }
  Serial.println(F("AHT20 OK"));
}

/**
 * @brief センサーの読み取りを処理し、引数のポインタ経由で値を返す
 * @param temp 温度を格納する変数のポインタ
 * @param hum 湿度を格納する変数のポインタ
 */
void handleSensor(float* temp, float* hum) {
  if (millis() - lastSensorReadTime >= SENSOR_READ_INTERVAL) {
    lastSensorReadTime = millis();

    float tempReading = aht20.readTemperature();
    if (tempReading != AHTXX_ERROR) {
      *temp = tempReading;
    }

    float humReading = aht20.readHumidity();
    if (humReading != AHTXX_ERROR) {
      *hum = humReading;
    }
  }
}
