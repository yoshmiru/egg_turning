#pragma once

#include <Arduino.h>

// 関数プロトタイプ宣言
void initLcd();
void handleLcd(float temp, float hum, double heater_output);
