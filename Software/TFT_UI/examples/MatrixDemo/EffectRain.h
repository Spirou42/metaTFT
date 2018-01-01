#ifndef __EFFECT_RAIN_H__
#define __EFFECT_RAIN_H__

#include "MatrixDemo.h"

class EffectRain : public Effect {
 public:
   EffectRain():Effect("Rain"){}
   virtual void startEffect(){
     blendFactor=255;
   }
   virtual uint16_t frameRate(){return 1000/30;}

   virtual void frame(unsigned long now);
};


#endif
