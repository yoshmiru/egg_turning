#pragma once

#include <Arduino.h>

// 関数プロトタイプ宣言
void initSensor();
void handleSensor(float* temp, float* hum);
