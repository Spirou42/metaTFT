#ifndef __EFFECT_WAVE_H__
#define __EFFECT_WAVE_H__

#include "MatrixDemo.h"
class EffectWave:public Effect{
  public:
  EffectWave():Effect("Waves"){}
  virtual uint16_t frameRate(){return 1000/60;}

  virtual void startEffect(){
    blendFactor = 20;
    currentSystemPalette = systemPalettes.begin()+3;
  }
  virtual void frame(unsigned long now){
    static byte rotation = 1;

      static const uint16_t scaleWidth = 65768 / (MATRIX_WIDTH);
      static const uint16_t scaleHeight = 65768 / (MATRIX_HEIGHT);

      static const uint8_t maxX = MATRIX_WIDTH - 1;
      static const uint8_t maxY = MATRIX_HEIGHT - 1;

      static uint8_t hue = 1;
      static uint16_t theta = 0;

     // EVERY_N_SECONDS(10) {
     //   rotation = random(0, 4);
     //   Serial << "Rotation: "<<rotation<<"\twaveCount:"<<waveCount<<endl;
     // }
      ledMatrix.fadeToBlack(255);
      int n = 0;
      CRGB color;
      CRGBPalette16 palette = (*currentSystemPalette)->second.palette;
      switch (rotation) {
        case 0:
          for (int x = 0; x < MATRIX_WIDTH; x++) {
            n = sin16(x * 2 + theta) / scaleHeight;
            color = ColorFromPalette(palette, x + hue);
            ledMatrix.setPixel(x,n,color);
          }
          break;

        case 1:
          for (int y = 0; y < MATRIX_HEIGHT; y++) {
            n = (sin16( (y*scaleHeight/3)+theta)+32768) / scaleWidth;
//            Serial << "N "<<n<<endl;
//            Serial << "Theta: "<<theta<<endl;
            while(n>=MATRIX_WIDTH){
              n-=MATRIX_WIDTH;

            }
            while (n<0){
              n+=MATRIX_WIDTH;
            }
//            Serial <<"ResulN: "<<n<<endl;
            color = ColorFromPalette(palette, y + hue);
            ledMatrix.setPixel(n,y,color);
          }
          break;
      }
      delayedFrame++;
      if(delayedFrame>0){
        delayedFrame=0;
        theta+=(64*1024)/100;
        hue++;
      }
    }
protected:
  int delayedFrame = 0;

  };


#endif
