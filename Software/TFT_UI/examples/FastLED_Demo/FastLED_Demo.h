/**
  FastLED Demo Configuration
*/

#ifndef __FastLED_Demo_H__
#define __FastLED_Demo_H__

#define FASTLED_INTERNAL

//#define FASTLED_ALLOW_INTERRUPTS 0

#include <stack>
#include <deque>
#include <vector>
#include <tuple>
#include <list>
#include <deque>

#include "FastLED.h"
#include "FastLEDAddOns.h"
#include "TFT_UI.h"

/** TFT Configuration **/
#define TFT_RST   2
#define TFT_DC    9
#define TFT_CS    10
#define TFT_MISO  12
#define TFT_MOSI  11
#define TFT_SCK   13
#define TFT_LED   6


#define SD_CS     21
#define IR_IN     5

/** FastLED **/
/** switchen between WS2812 and APA102 **/
#define USE_APA102 1

#define LED_PIN           8

#if USE_APA102
  #warning APA102
  #define CHIPSET           APA102
  #define COLOR_ORDER       BGR
  #define COLOR_CORRECTION  0xffeeff
  #define CLOCK_PIN         14
  //#define NUM_LEDS          130
  #define NUM_LEDS          60
#else
  #warning WS2812
  #define CHIPSET           WS2812
  #define COLOR_ORDER       GRB
  #define COLOR_CORRECTION  0xffeeff
  #define NUM_LEDS          158
#endif


#define LED_BRIGHTNESS    50

#define DEBUG_STARTUP     false

// ** some stuff other modules has to know about */
extern CRGB leds[];

// typedef void(*effectHandler)(void);

// // some datatype to map Names(Strings) palettes or Effects
// typedef std::pair<const String,CRGBPalette16> PalettePair;
// typedef std::vector<PalettePair*> PaletteList;
//
// typedef std::pair<const String, effectHandler> EffectPair;
// typedef std::vector<EffectPair*> EffectList;
//
// typedef std::vector<ValueEditor*> ActionList;
//
// // after the types we also need some declarations for those
// extern PaletteList systemPalettes;
// extern PaletteList::iterator currentSystemPalette;
//
// extern EffectList systemEffects;
// extern EffectList::iterator currentSystemEffect;
//
// extern ActionList actionList;

extern PaletteList systemPalettes;
extern PaletteList::iterator currentSystemPalette;

extern int16_t numberOfBlobs;
extern int16_t fadeOutAmount;
extern int16_t blobLength;
extern int16_t startBlobSpeed;
int sgn(float v) ;
//extern CHSV rgb2hsv(const CRGB& rgb);

#endif
