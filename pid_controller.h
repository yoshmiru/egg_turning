#pragma once

#include <Arduino.h>

// 関数プロトタイプ宣言
void initPid();
void handleHeater(double current_temp);

// グローバル変数（外部参照用）
// 実体は pid_controller.cpp にあります
extern double pidOutput; // LCD表示用にinoファイルから参照

