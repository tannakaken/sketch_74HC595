// 以下を参考にした
// 配線も以下のサイトを参考にするとできる。
// https://iot.keicode.com/arduino/arduino-shift-register-sn74hc595.php

/**
 * シリアルを送信するピン
 */
const int PIN_SERIAL = 8;
/**
 * ラッチ（情報をアップデートするサイン）を送信するピン
 */
const int PIN_LATCH = 9;
/**
 * クロックを送信するピン
 */
const int PIN_CLOCK = 10;
/**
 * ボタン入力のピン
 */
const int PIN_BUTTON = 7;

/**
 * LEDの光るパターン
 */
byte patterns[] = {
  B00000000,
  B10000000,
  B01000000,
  B00100000,
  B00010000,
  B00001000,
  B00000100,
  B00000010,
  B00000001,
  B00000011,
  B00000110,
  B00001100,
  B00011000,
  B00110000,
  B01100000,
  B11000000,
  B11100000,
  B01110000,
  B00111000,
  B00011100,
  B00001110,
  B00000111,
  B00001111,
  B00011110,
  B00111100,
  B01111000,
  B11110000,
  B11111000,
  B01111100,
  B00111110,
  B00011111,
  B00111111,
  B01111110,
  B11111100,
  B11111110,
  B01111111,
  B11111111,
  B01111111,
  B11111110,
  B11111100,
  B01111110,
  B00111111,
  B00011111,
  B00111110,
  B01111100,
  B11111000,
  B11110000,
  B01111000,
  B00111100,
  B00011110,
  B00001111,
  B00000111,
  B00001110,
  B00011100,
  B00111000,
  B01110000,
  B11100000,
  B11000000,
  B01100000,
  B00110000,
  B00011000,
  B00001100,
  B00000110,
  B00000011,
  B00000001,
  B00000010,
  B00000100,
  B00001000,
  B00010000,
  B00100000,
  B01000000,
  B10000000,
};

void setup() {
  Serial.begin(9600);
  pinMode(PIN_SERIAL, OUTPUT);
  pinMode(PIN_LATCH, OUTPUT);
  pinMode(PIN_CLOCK, OUTPUT);
}

bool started = false;
uint8_t i = 0;
void loop() {
  String command = Serial.readStringUntil("\n");
  command.trim();
  if (command.length() > 0) {
    Serial.println(command);
  }
  if (command == String("switch") || digitalRead(PIN_BUTTON) == HIGH) {
    started = !started;
    if (!started) {
      i = 0;
      digitalWrite(PIN_LATCH, LOW); // 入力を受け付けるモードにする
      shiftOut(PIN_SERIAL, PIN_CLOCK, LSBFIRST, B00000000); // この情報がクロックが0から1になるたびに、チップの出力ピンに設定されていく。
      digitalWrite(PIN_LATCH, HIGH); // 入力を終わり、チップの出力ピンの電圧が変わる。
    }
    Serial.println("ok");
  }
  if (started) {
    digitalWrite(PIN_LATCH, LOW);
    shiftOut(PIN_SERIAL, PIN_CLOCK, LSBFIRST, patterns[i]);
    digitalWrite(PIN_LATCH, HIGH);
    i = (i + 1) % (sizeof(patterns)/sizeof(byte));
    delay(50);
  }
}
