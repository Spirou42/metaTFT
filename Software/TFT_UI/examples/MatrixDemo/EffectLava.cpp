/**
EffectLava
*/

#include "EffectLava.h"
void EffectLava::startEffect(){
  blendFactor = 20;
  currentSystemPalette = systemPalettes.begin()+PALETTE_TEMPERATURE;
  for(uint8_t x=0;x<MATRIX_WIDTH;++x){
    for(uint8_t y=0;y<MATRIX_HEIGHT;++y){
      tempMap[x][y] = 0;
    }
  }
}
uint8_t EffectLava::add(uint8_t f, uint8_t s){
  uint16_t k = f+s;
  if(k>250){
    return 250;
  }
  return k&0xff;
}
uint8_t EffectLava::sub(uint8_t f, uint8_t s){
  int16_t k = f-s;
  if(k<0){
    return 0;
  }
  return k&0xff;
}

void EffectLava::applyHeat(){
  for(uint8_t x= 0;x<MATRIX_WIDTH;x++){
    tempMap[x][0] = add(tempMap[x][0],bottomHeat);
  }
  // heat travels upward with a constant rate
  for(uint8_t x=0;x<MATRIX_WIDTH;x++){
    for(uint8_t y=1;y<MATRIX_HEIGHT;y++){
      int16_t q= heatLosBase+(tempMap[x][y-1]-tempMap[x][y])/3;// + (tempMap[x][y-1])/10;
      if (q>tempMap[x][y-1] || q<0){
        q = tempMap[x][y-1];
      }
      // if(y == MATRIX_HEIGHT-1 && x == 0){
      //   Serial << q<<endl;
      // }

      tempMap[x][y]= add(tempMap[x][y],(uint8_t)q);
      tempMap[x][y-1] = sub(tempMap[x][y-1],q);
    }
  }
}

void EffectLava::coolDown(){
  for(uint8_t y=0;y<MATRIX_HEIGHT;y++){
    for(uint8_t x=0;x<MATRIX_WIDTH;x++){
      uint8_t heatLoss = (((y+1)/3) * tempMap[x][y])/512;
      tempMap[x][y] = sub(tempMap[x][y], heatLoss);
    }
  }
  for (uint8_t x = 0; x < MATRIX_WIDTH; x++) {
    uint8_t q= tempMap[x][MATRIX_HEIGHT-1]/5;
    tempMap[x][MATRIX_HEIGHT-1] = sub(tempMap[x][MATRIX_HEIGHT-1],q);
  }
}

void EffectLava::mapHeatToBackground(){
  for(uint8_t x=0;x<MATRIX_WIDTH;x++){
    for(uint8_t y=0;y<MATRIX_HEIGHT;y++){
      CRGB color = ColorFromPalette((*currentSystemPalette)->second.palette,tempMap[x][y]);
      ledMatrix.setPixel(x,y,color);
    }
  }
}

void EffectLava::frame(unsigned long now){
  applyHeat();
  coolDown();
  mapHeatToBackground();
  EVERY_N_SECONDS(2){
    for (uint8_t y = 19; y >= 15; y--) {
      for (uint8_t x = 0; x < MATRIX_WIDTH; x++) {
        Serial << " "<<_HEX(tempMap[x][y]);
      }
      Serial << endl;
    }
    Serial << endl;
  }
}
