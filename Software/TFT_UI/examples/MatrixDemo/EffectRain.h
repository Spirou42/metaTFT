#ifndef __EFFECT_RAIN_H__
#define __EFFECT_RAIN_H__

#include "MatrixDemo.h"
#define MAX_DIMENSION ((MATRIX_WIDTH>MATRIX_HEIGHT) ? MATRIX_WIDTH : MATRIX_HEIGHT)

class EffectRain : public Effect {
 public:
   EffectRain():Effect("Rain"){}
   virtual void startEffect();
   virtual uint16_t frameRate(){return 1000/30;}

   virtual void frame(unsigned long now);
 protected:
   uint16_t x;
   uint16_t y;
   uint16_t z;
   uint16_t speed = 40;
   uint16_t scale = 100;
   uint8_t noise[MAX_DIMENSION][MAX_DIMENSION];
   void fillnoise8();
};


#endif
