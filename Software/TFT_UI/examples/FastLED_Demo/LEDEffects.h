/** Collection of simple LED-Effects for LED stripes */

#ifndef __LEDEFFECTS_H__
#define __LEDEFFECTS_H__

#include "FastLED.h"
#include "FastLED_Demo.h"

extern uint8_t gHue;

void rainbow();
void rainbowWithGlitter();
void confetti();
void sinelon();
void minelon();
void tinelon();
void bpm();
void juggle();
void nextPattern();
void nextPalette();

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))
#define FRAMES_PER_SECOND 120

#endif
