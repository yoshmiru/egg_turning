// include the library
#include <Servo.h>
#include <LiquidCrystal.h>
#include <Wire.h>
#include <AHTxx.h>

// README.mdの要件に基づいてピンと定数を定義
#define SERVO_PIN 2
#define BUTTON_PIN 3
#define ROTATION_INTERVAL (90 * 60 * 1000UL) // 90分 (unsigned long)
#define LEFT_ANGLE 45
#define RIGHT_ANGLE 180
#define LCD_RS 4
#define LCD_EN 5
#define LCD_D4 6
#define LCD_D5 7
#define LCD_D6 8
#define LCD_D7  9

Servo sg90;
int currentAngle = LEFT_ANGLE; // 現在のサーボの角度 (0 or 180)
unsigned long lastRotationTime = 0; // 最後にサーボが回転した時刻

// Creates an LCD object. Parameters: (rs, enable, d4, d5, d6, d7)
LiquidCrystal lcd(LCD_RS, LCD_EN, LCD_D4, LCD_D5, LCD_D6, LCD_D7);

// initialize sensor
float ahtValue;                               //to store T/RH result

AHTxx aht20(AHTXX_ADDRESS_X38, AHT2x_SENSOR); //sensor address, sensor type

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

  // LCDを初期化
  initLCD();

  // センサーを初期化
  initSensor();
}

void loop() {
  // 90分経過したかチェック
  if (millis() - lastRotationTime >= ROTATION_INTERVAL) {
    rotateServo();
  }

  // ボタンが押されたかチェック (INPUT_PULLUPなのでLOWが押された状態)
  if (digitalRead(BUTTON_PIN) == LOW) {
    Serial.println("Button might be pressed. Rotating servo.");

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

  readSensor();
  
  refreshLCD();
}

// サーボの角度を0度と180度の間で切り替える関数
void rotateServo() {
  currentAngle = (currentAngle == LEFT_ANGLE)
    ? RIGHT_ANGLE
    : LEFT_ANGLE;

  sg90.write(currentAngle);
  Serial.print("Servo moved to: ");
  Serial.println(currentAngle);

  // 最後の回転時刻を更新
  lastRotationTime = millis();
}

void initLCD() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Clears the LCD screen
  lcd.clear();
}

void refreshLCD() {
  // Print a message to the LCD.
  lcd.print(" Hello world!");

  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0, 1);
  // Print a message to the LCD.
  lcd.print(" LCD Tutorial");
}

void initSensor() {
  while (aht20.begin() != true)
  {
    Serial.println(F("AHT2x not connected or fail to load calibration coefficient")); //(F()) save string to flash & keeps dynamic memory free

    delay(5000);
  }

  Serial.println(F("AHT20 OK"));
}

void readSensor() {
    ahtValue = aht20.readTemperature(); //read 6-bytes via I2C, takes 80 milliseconds

  Serial.print(F("Temperature...: "));
  
  if (ahtValue != AHTXX_ERROR) //AHTXX_ERROR = 255, library returns 255 if error occurs
  {
    Serial.print(ahtValue);
    Serial.println(F(" +-0.3C"));
  }
  /* else
  {
    printStatus(); //print temperature command status

    if   (aht20.softReset() == true) Serial.println(F("reset success")); //as the last chance to make it alive
    else                             Serial.println(F("reset failed"));
  }*/

  delay(2000); //measurement with high frequency leads to heating of the sensor, see NOTE

  ahtValue = aht20.readHumidity(); //read another 6-bytes via I2C, takes 80 milliseconds

  Serial.print(F("Humidity......: "));
  
  if (ahtValue != AHTXX_ERROR) //AHTXX_ERROR = 255, library returns 255 if error occurs
  {
    Serial.print(ahtValue);
    Serial.println(F(" +-2%"));
  }
  /*else
  {
    printStatus(); //print humidity command status
  }*/
  delay(10000); //recomended polling frequency 8sec..30sec
}
