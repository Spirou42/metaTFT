/**
__EFFECT_RAIN_H__*/

#include "EffectRain.h"

void EffectRain::startEffect(){
  blendFactor=20;
  _tempWrapper = new ValueWrapper();
  //Fill background
}

void EffectRain::stopEffect(){
  if(_tempWrapper){
    delete(_tempWrapper);
    _tempWrapper = NULL;
  }
}
void EffectRain::fillnoise8() {
  // If we're runing at a low "speed", some 8-bit artifacts become visible
  // from frame-to-frame.  In order to reduce this, we can do some fast data-smoothing.
  // The amount of data smoothing we're doing depends on "speed".
  uint8_t dataSmoothing = 0;

  for(int i = 0; i < MAX_DIMENSION; i++) {
    int ioffset = scale * i;
    for(int j = 0; j < MAX_DIMENSION; j++) {
      int joffset = scale * j;

      uint8_t data = inoise8(x + ioffset,y + joffset,z);

      noise[i][j] = data;
    }
  }

//  z += speed;

  // apply slow drift to X and Y, just for visual variation.
  x += speed/2;
  y += speed;
  //y -= speed / 16;
}

void EffectRain::frame(unsigned long now){
  fillnoise8();
  for(int i = 0; i < ledMatrix.width(); i++) {
    for(int j = 0; j < ledMatrix.height(); j++) {
      // We use the value at the (i,j) coordinate in the noise
      // array for our brightness, and the flipped value from (j,i)
      // for our pixel's index into the color palette.

      //uint8_t index = noise[j][i];
      uint8_t bri =   noise[i][j];
      bri = quadwave8(bri/3);
      CHSV hcolor = CHSV(160,255,bri);
      //CRGB color = ColorFromPalette( (*currentSystemPalette)->second.palette, index, bri);
      ledMatrix.setPixel(i,j,hcolor);
    }
  }
}

String EffectRain::parameterNameAt(size_t idx){
  static String result;
  switch(idx){
    case 0:
    result = "Speed";
    break;
    case 1:
    result = "Scale";
    break;
    default:
    break;
  }
  return result;
}

ValueWrapper* EffectRain::parameterAt(size_t idx){
  switch(idx){
    case 0:
    _tempWrapper->setValueStore(&speed);
    _tempWrapper->setMinValue(0);
    _tempWrapper->setMaxValue(100);
    break;
    case 1:
    _tempWrapper->setValueStore(&scale);
    _tempWrapper->setMinValue(1);
    _tempWrapper->setMaxValue(300);
    break;
    default:
    return NULL;
  }
  return _tempWrapper;
}
