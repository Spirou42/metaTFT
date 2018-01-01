/**
EffectNoise.h
**/

#include "MatrixDemo.h"
#define MAX_DIMENSION ((MATRIX_WIDTH>MATRIX_HEIGHT) ? MATRIX_WIDTH : MATRIX_HEIGHT)
class EffectNoise: public Effect{
public:
  EffectNoise():Effect("Clouds"){}
  virtual uint16_t frameRate(){return 1000/10;}

  virtual void startEffect(){
    blendFactor = 20;
    currentSystemPalette = systemPalettes.begin()+2;
    x = random16();
    y = random16();
    z = random16();

  }

  virtual void frame(unsigned long now);
protected:
  // The 16 bit version of our coordinates
 uint16_t x;
 uint16_t y;
 uint16_t z;
 uint16_t speed = 4; // speed is set dynamically once we've started up
 uint16_t scale = 40; // scale is set dynamically once we've started up
 uint8_t noise[MAX_DIMENSION][MAX_DIMENSION];

 void fillnoise8();
};
