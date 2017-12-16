#ifndef __EFECT_LINEBOUNCE_H__
#define __EFECT_LINEBOUNCE_H__

#include "ChristmasBall.h"

class EffectLineBounce : public Effect {
public:

  EffectLineBounce():Effect("LineBounce"),line(0),delayedFrame(0),step(1){};

  //virtual void startEffect();
  virtual void frame(unsigned long now) {
    Serial << "Frame: "<<endl;
    ledMatrix.clear();
    FLPoint s(0,line);
    FLPoint e(12,line);
    ledMatrix.line(s,e,CRGB::White);
    ledMatrix.flush();

    delayedFrame++;
    if(delayedFrame > 10){
      delayedFrame = 0;
      line +=step;
      if (line >=9){
        line = 9;
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
