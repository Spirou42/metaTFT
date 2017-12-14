/**
  ChristmasBall simple APA102 Demo
**/
#include <TFT_UI.h>
#include <Queue.h>
#include <Streaming.h>
#include <FastLED.h>
#include <FastLEDAddOns.h>
#include "ChristmasBall.h"

// queue for simple multitasking
Queue taskQueue;

// the buffer and backbuffer for the LEDS
CRGB leds[NUM_LEDS+1];
CRGB backBuffer[NUM_LEDS+1];
XYMatrix ledMatrix(13,10,leds,backBuffer,true,true,true);

// The TFT
TFTDisplay tft = TFTDisplay(TFT_CS,TFT_DC,TFT_RST,TFT_MOSI,TFT_SCK,TFT_MISO,TFT_LED,3);

// the UI
metaList  SystemMenu;               ///<< The main Menu of the Application
metaList  PalettesMenu;             ///<< Selection List for Palettes
metaValue ValueView;                ///<< View used for numerical value changes

// a couple of global parameters

int16_t tftBrightness = 0;
TFTBrightnessWrapper TFTBrightness(&tftBrightness,&tft);
ValueEditor tftBrightnessAction(&ValueView,&TFTBrightness);

int16_t ledBrightness = LED_BRIGHTNESS;
LEDBrightnessWrapper ledBrightnessWrapper(&ledBrightness);
ValueEditor ledBrightnessAction(&ValueView,&ledBrightnessWrapper);


void initialiseLEDs(){
  FastLED.addLeds<CHIPSET, LED_PIN, CLOCK_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(COLOR_CORRECTION);
  FastLED.clear(true);
  FastLED.show();
  FastLED.setBrightness( LED_BRIGHTNESS );
  fill_solid(leds,NUM_LEDS,CRGB::Red);
  FastLED.show();
}

void initialiseTFT(){
  tft.start();
  TFTBrightness.setValue(13);
}

void setup(){
  Serial.begin(115200);
  initialiseLEDs();

  //init LED backlight
  pinMode(TFT_LED,OUTPUT);
  digitalWriteFast(TFT_LED,1);

  initialiseTFT();

}

void loop (){
  taskQueue.Run(millis());
}
