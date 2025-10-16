#include "config.h"

// LCDオブジェクトをこのファイルで定義 (パラレル接続用)
LiquidCrystal lcd(LCD_RS, LCD_EN, LCD_D4, LCD_D5, LCD_D6, LCD_D7);

// タイマー
unsigned long lastLcdRefreshTime = 0;

/**
 * @brief LCDを初期化する
 */
void initLcd() {
  lcd.begin(LCD_COLS, LCD_ROWS); // I2C用のinit()から変更
  lcd.clear();
}

/**
 * @brief LCD表示を処理する
 * @param temp 現在温度
 * @param hum 現在湿度
 * @param heater_output ヒーター出力値
 */
void handleLcd(float temp, float hum, double heater_output) {
  if (millis() - lastLcdRefreshTime >= LCD_REFRESH_INTERVAL) {
    lastLcdRefreshTime = millis();

    lcd.clear();
    lcd.setCursor(0, 0);

    // 1行目: 温度
    lcd.print("T:");
    lcd.print(temp, 1);
    lcd.print("C");

    // 2行目: 湿度とヒーター出力
    lcd.setCursor(0, 1);
    lcd.print("H:");
    lcd.print(hum, 1);
    lcd.print("% ");
    lcd.print("P:");
    lcd.print((int)heater_output);
  }
}