/**
  ChristmasBall Configuration
*/
#ifndef __ChristmasBall_H__
#define __ChristmasBall_H__

#define FASTLED_INTERNAL

//#define FASTLED_ALLOW_INTERRUPTS 0

#include <stack>
#include <deque>
#include <vector>
#include <tuple>
#include <list>
#include <deque>

#include "FastLED.h"
#include "TFT_UI.h"
#include "TFT_UI_Highlevel.h"

#define USE_CHRISTMASBALL 0
#define USE_CYLINDERLAMP 1

/** TFT Configuration **/
#define TFT_RST   2
#define TFT_DC    9
#define TFT_CS    10
#define TFT_MISO  12
#define TFT_MOSI  11
#define TFT_SCK   13
#define TFT_LED   6
#define IR_IN     5

/** FastLED Configuration **/
#define CHIPSET           APA102
#define COLOR_ORDER       BGR
#define COLOR_CORRECTION  0xffffff
#define LED_PIN           8
#define CLOCK_PIN         14

#define LED_BRIGHTNESS    80

#if USE_CHRISTMASBALL
  #define MATRIX_WIDTH      13
  #define MATRIX_HEIGHT     10
#elif USE_CYLINDERLAMP
  #define MATRIX_WIDTH      10
  #define MATRIX_HEIGHT     20
#endif

#define NUM_LEDS          (MATRIX_WIDTH*MATRIX_HEIGHT)

extern XYMatrix ledMatrix;
extern PaletteList systemPalettes;
extern PaletteList::iterator currentSystemPalette;

extern EffectList systemEffectList;
extern EffectList::iterator currentRunningEffect;

extern int16_t blendFactor;
extern uint8_t globalHue;
extern CRGB leds[];

#ifndef MATRIX_HEIGHT
#error You have to declare either USE_CHRISTMASBALL or USE_CYLINDERLAMP
#endif
#endif
