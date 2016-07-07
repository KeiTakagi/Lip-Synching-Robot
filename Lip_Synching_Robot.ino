/*

   @file Lip_Synching_Robot.ino
   @brief 8 x 8 LED Matrix animation using Arduino.
   @author Kei Takagi
   @date 2016.7.7

   8 x 8 Led Matrix ... 2-17 pin
   MIC Input        ... A4 pin

   Copyright (c) 2016 Kei dTakagi
   Revfleased under the MIT license
   http://opensource.org/licenses/mit-license.php

*/

#include <MsTimer2.h>
#define FallVelocity 40

// 8 x 8 matrix LED
// ROW side cathode , COLOMN side of the anode
byte pins[17] = { -1, 9, 8, 7, 6, 5, 4, 3, 2, 17, 16, 15, 14, 13, 12, 11, 10};
const static byte cols[8] = {pins[13], pins[ 3], pins[ 4], pins[10], pins[ 6], pins[11], pins[15], pins[16]};
const static byte rows[8] = {pins[ 9], pins[14], pins[ 8], pins[12], pins[ 1], pins[ 7], pins[ 2], pins[ 5]};


int anime_frame = 0;
int cat = 0;
int ano = 0;
int val = 0;
int max = 0;
int min = 1024;
int i = 0;

unsigned long time;
byte layer[8];

byte face[12][8] = {
  {
    0b01100110,
    0b01100110,
    0b00000000,
    0b00000000,
    0b00000000,
    0b01111110,
    0b00000000,
    0b00000000,
  }, {
    0b01100110,
    0b01100110,
    0b00000000,
    0b00000000,
    0b00011000,
    0b00100100,
    0b00011000,
    0b00000000,
  }, {
    0b01100110,
    0b01100110,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00011000,
    0b00000000,
    0b00000000,
  }, {
    0b01100110,
    0b01100110,
    0b00000000,
    0b00000000,
    0b01111110,
    0b10000001,
    0b01111110,
    0b00000000,
  }, {
    0b01100110,
    0b01100110,
    0b00000000,
    0b00000000,
    0b00111100,
    0b01000010,
    0b00111100,
    0b00000000,
  }, {
    0b01100110,
    0b01100110,
    0b00000000,
    0b00111100,
    0b01000010,
    0b01000010,
    0b01000010,
    0b00111100,
  }, {
    0b01100110,
    0b01100110,
    0b00000000,
    0b00011000,
    0b00100100,
    0b00100100,
    0b00100100,
    0b00011000,
  }, {
    0b01100110,
    0b01100110,
    0b00000000,
    0b01111110,
    0b10000001,
    0b10000001,
    0b01111110,
    0b00000000,
  }, {
    0b01100110,
    0b01100110,
    0b00000000,
    0b00111100,
    0b01000010,
    0b01000010,
    0b01000010,
    0b00111100,
  }, {
    0b01100110,
    0b01100110,
    0b01111110,
    0b10000001,
    0b10000001,
    0b10000001,
    0b10000001,
    0b01111110,
  }, {
    0b01100110,
    0b01100110,
    0b01111110,
    0b10000001,
    0b10000001,
    0b10000001,
    0b10000001,
    0b01111110,
  }, {
    0b01100110,
    0b00000000,
    0b01111110,
    0b10000001,
    0b10000001,
    0b10000001,
    0b10000001,
    0b01111110,
  }
};

void setup()
{
  for ( cat = 0; cat < 8; cat++ ) {
    pinMode( rows[cat], OUTPUT );
    digitalWrite( rows[cat], HIGH );
  }
  for ( ano = 0; ano < 8; ano++ ) {
    pinMode( cols[ano], OUTPUT );
    digitalWrite( cols[ano], LOW );
  }

  time = millis() +  FallVelocity;
  MsTimer2::set(1, display);
  MsTimer2::start();

  randomSeed(analogRead(4));
}

void loop()
{
  val = 0.7 * val + 0.3 * analogRead(A4) ;
  if (max < val)max = val;
  if (min > val)min = val;
  if (time < millis()) {
    time = millis() +  FallVelocity;

    anime_frame = map(max, min, 1023, 0, 12);
    for ( i = 0; i < 8; i++) {
      *(layer + i) = *(face[anime_frame] + i);
    }
    switch (random(0, 50) ) {
      case 1:
        *(layer + 0) = 0b01000100;
        *(layer + 1) = 0b01000100;
        break;
      case 2:
        *(layer + 0) = 0b00100010;
        *(layer + 1) = 0b00100010;
        break;
      case 3:
        *(layer + 0) = 0b00000000;
        break;
      case 4:
        *(layer + 1) = 0b00000000;
        break;
    }
    min = max;
    max = 0;
  }
}

void display()
{
  cat < 7 ? cat++ :  cat = 0;
  digitalWrite( *(rows + cat), LOW );
  for ( ano = 0; ano < 8; ano++ ) {
    digitalWrite( *(cols + ano) , (*(layer + ano) >> cat ) & 0x01 );
    delayMicroseconds(50);
    digitalWrite( *(cols + ano), LOW );
  }
  digitalWrite( *(rows + cat), HIGH );
}

