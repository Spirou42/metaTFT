#ifndef __EFECT_LINEBOUNCE_H__
#define __EFECT_LINEBOUNCE_H__

#include "MatrixDemo.h"

class EffectLineBounce : public Effect {
public:

  EffectLineBounce():Effect("LineBounce"),line(0),delayedFrame(0),step(1){};

  virtual void startEffect(){
    blendFactor = 6;
  }
  virtual uint16_t frameRate(){return 1000/30;}
  virtual void frame(unsigned long now) {
    //Serial << "Frame: "<<endl;
    ledMatrix.fadeToBlack(0);
    //ledMatrix.fill(CRGB::Black);
    FLPoint s(0,line);
    FLPoint e(12,line);
    CRGB color = ColorFromPalette((*currentSystemPalette)->second,globalHue);
    ledMatrix.line(s,e,color);
    ledMatrix.flush();

    delayedFrame++;
    int16_t upperLine = MATRIX_HEIGHT -1;
    if(delayedFrame > 4){
      delayedFrame = 0;
      line +=step;
      //line = line % 9;
      if (line >=upperLine){
        line = upperLine;
        step = -1;
      }else if (line <=0){
        step = 1;
        line = 0;
      }
    }
  }

protected:
  int line = 0;
  int delayedFrame = 0;
  int step = 1;
};


#endif
