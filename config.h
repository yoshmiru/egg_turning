#pragma once

// ライブラリ
#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal.h> // I2Cから変更
#include <AHTxx.h>
#include <PID_v1.h>

// ピン定義
#define SERVO_PIN 2
#define BUTTON_PIN 3
#define HEATER_PIN 10

// LCDピン定義 (パラレル接続)
#define LCD_RS 4
#define LCD_EN 5
#define LCD_D4 6
#define LCD_D5 7
#define LCD_D6 8
#define LCD_D7 9

// サーボ設定
#define ROTATION_INTERVAL (90 * 60 * 1000UL) // 90分
#define LEFT_ANGLE 45
#define RIGHT_ANGLE 180

// LCD設定
#define LCD_COLS 16
#define LCD_ROWS 2

// センサー・制御タイミング設定 (ms)
#define SENSOR_READ_INTERVAL 1000      // 1秒ごとにセンサーを読む
#define HEATER_CONTROL_INTERVAL 1000   // 1秒ごとにヒーター制御
#define LCD_REFRESH_INTERVAL 1000      // 1秒ごとにLCDを更新

// PID制御設定
#define SETPOINT_TEMP 38.0 // 目標温度
#define KP 2.0
#define KI 5.0
#define KD 1.0

// PWMのデューティサイクルは0-255の範囲ですが、READMEの0-10という要件を考慮し、
// PID出力(0-10)をPWM(0-255)にマッピングして使用します。
#define HEATER_MIN_OUTPUT 0
#define HEATER_MAX_OUTPUT 10