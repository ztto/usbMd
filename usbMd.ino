/*
 * Md To USB
 * (Arduino Pro Micro)
 *
 *DSUB9P(Md)  --  pro micro  gpio
 *  1         --   d4(D0)     d4
 *  2         --   d5(D1)     c6
 *  3         --   d6(D2)     d7
 *  4         --   d7(D3)     e6
 *  5         --   Vcc(+5V)
 *  6         --   d2 (D4)    d1(int1)
 *  7 (9)     --   d3 (D5)    d0(int0)
 *  8 (7)     --   d8 (SEL)   b4
 *  9 (8)     --   GND
 */
#include <Joystick.h>

Joystick_ Joystick = Joystick_(
  0x03,                    // reportid
  JOYSTICK_TYPE_GAMEPAD,  // type
  10,                      // button count
  0,                       // hat switch count
  true,                    // x axis enable
  true,                    // y axis enable
  false,                   // z axis enable
  false,                   // right x axis enable
  false,                   // right y axis enable
  false,                   // right z axis enable
  false,                   // rudder enable
  false,                   // throttle enable
  false,                   // accelerator enable
  false,                   // brake enable
  false                    // steering enable
);

// gpio
#define D0      4
#define D1      5
#define D2      6
#define D3      7
#define D4      2
#define D5      3
#define SEL     8

int PIN[10];
#define LOOP_INTERVAL 16  // 入力周期(msec)

// セットアップ
void setup() {
  
  // シリアル初期化
//  Serial.begin(115200);
//  while (!Serial);

  // pin初期設定
  pinMode(D0,INPUT_PULLUP);
  pinMode(D1,INPUT_PULLUP);
  pinMode(D2,INPUT_PULLUP);
  pinMode(D3,INPUT_PULLUP);
  pinMode(D4,INPUT_PULLUP);
  pinMode(D5,INPUT_PULLUP) ;
  pinMode(SEL,OUTPUT) ;
  // d8(SEL) HIGH
  PORTB |= 0b00010000 ;

  // ジョイスティック開始
  Joystick.begin();

  // ジョイスティック初期設定
  Joystick.setXAxisRange(0, 2);
  Joystick.setYAxisRange(0, 2);
}

uint8_t btn6;
// メインループ
void loop() {
  
  // d8(SEL) LOW
  PORTB &= 0b11101111;
  delayMicroseconds(20);

  // a start
  PIN[0] = PIND;

  // d8(SEL) HIGH
  PORTB |= 0b00010000 ;
  delayMicroseconds(20);

  // 上 下 左 右 b c
  PIN[1] = PINC;
  PIN[2] = PIND;
  PIN[3] = PINE;

  // d8(SEL) LOW
  PORTB &= 0b11101111;
  delayMicroseconds(20);
  // d8(SEL) HIGH
  PORTB |= 0b00010000 ;
  delayMicroseconds(20);
  // d8(SEL) LOW
  PORTB &= 0b11101111;
  delayMicroseconds(20);

  if((digitalRead(D0) == LOW) &&
     (digitalRead(D1) == LOW) &&
     (digitalRead(D2) == LOW) &&
     (digitalRead(D3) == LOW)) {
    btn6 = 1;
//    Serial.println("btn6");
  } else {
    btn6 = 0;
//    Serial.println("btn3");
  }
    
  // d8(SEL) HIGH
  PORTB |= 0b00010000 ;
  delayMicroseconds(20);

  // X Y Z mode
  if (btn6 == 1) {
    PIN[4] = PINC;
    PIN[5] = PIND;
    PIN[6] = PINE;
  }

  // d8(SEL) LOW
  PORTB &= 0b11101111;
  delayMicroseconds(20);
  // d8(SEL) HIGH
  PORTB |= 0b00010000 ;
//  delayMicroseconds(20);

  // ボタン入力 ------------
  // a
  if (!(PIN[0] & (1 << 1))) {
    Joystick.setButton(2, 1);
  } else {
    Joystick.setButton(2, 0);
  }
  // start
  if (!(PIN[0] & (1 << 0))) {
    Joystick.setButton(9, 1);
  } else {
    Joystick.setButton(9, 0);
  }

  // ボタン入力 ------------
  // 上
  if (!(PIN[2] & (1 << 4))) {
    Joystick.setYAxis(0);
  }
  // 下
  if (!(PIN[1] & (1 << 6))) {
    Joystick.setYAxis(2);
  }
  if (((PIN[2] & (1 << 4))) &&
      ((PIN[1] & (1 << 6)))) {
    Joystick.setYAxis(1);
  }
  // 左
  if (!(PIN[2] & (1 << 7))) {
    Joystick.setXAxis(0);
  }
  // 右
  if (!(PIN[3] & (1 << 6))) {
    Joystick.setXAxis(2);
  }
  if (((PIN[2] & (1 << 7))) &&
      ((PIN[3] & (1 << 6)))) {
    Joystick.setXAxis(1);
  }
  // b
  if (!(PIN[2] & (1 << 1))) {
    Joystick.setButton(1, 1);
  } else {
    Joystick.setButton(1, 0);
  }
  // c
  if (!(PIN[2] & (1 << 0))) {
    Joystick.setButton(5, 1);
  } else {
    Joystick.setButton(5, 0);
  }

  // ボタン入力 ------------
  // X Y Z mode
  if (btn6 == 1) {
    // z
    if (!(PIN[5] & (1 << 4))) {
      Joystick.setButton(4, 1);
    } else {
      Joystick.setButton(4, 0);
    }
    // y
    if (!(PIN[4] & (1 << 6))) {
      Joystick.setButton(0, 1);
    } else {
      Joystick.setButton(0, 0);
    }
    // x
    if (!(PIN[5] & (1 << 7))) {
      Joystick.setButton(3, 1);
    } else {
      Joystick.setButton(3, 0);
    }
    // mode
    if (!(PIN[6] & (1 << 6))) {
      Joystick.setButton(8, 1);
    } else {
      Joystick.setButton(8, 0);
    }
  }

//  //-------------------------
//  for (int i = 0; i < 7; i++) {
//    Serial.print(i);
//    Serial.print(":");
//    Serial.print(PIN[i]);
//    Serial.print(",");
//  }
//  Serial.println("");
//  //-------------------------

  // 処理待ち
//  delay(LOOP_INTERVAL);

  // HID デバイスへの出力に反映させる
  Joystick.sendState();
}
