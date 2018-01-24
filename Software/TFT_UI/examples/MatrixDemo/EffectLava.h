#ifndef __EFFECT_LAVA_H__
#define __EFFECT_LAVA_H__

#include "MatrixDemo.h"
typedef struct _lavaBlob{

}lavaBlob;

class EffectLava:public Effect{
  public:
  EffectLava():Effect("Lava"){}
  virtual uint16_t frameRate(){return 1000/60;}
  virtual void startEffect();
  virtual void frame(unsigned long now);

protected:
  void applyHeat();
  void coolDown();
  void mapHeatToBackground();
  void displayTopHeat();
  uint8_t add(uint8_t, uint8_t);
  uint8_t sub(uint8_t, uint8_t);
  uint8_t lavaDensity = 127;
  uint8_t fluidDensity = 60;
  uint8_t tempMap[MATRIX_WIDTH][MATRIX_HEIGHT];
  uint8_t bottomHeat = 100;
  uint8_t heatLosBase = 2;
  };


#endif
