/**
Input_IR.hpp
*/

#ifndef __Input_IR_HPP__
#define __Input_IR_HPP__

#include "IRremote.h"
#include "TFT_UI_Internal.h"
#include "Input_IRCodes_LEDLamp1.hpp"
#include <Streaming.h>

TFTUI_NAMESPACE_BEGIN
extern IRCodes knownCodes;
int decodeIR(unsigned long now, void * userdata);
TFTUI_NAMESPACE_END


class Input_IR{
 public:
  Input_IR(int irPin):inputIR(IRrecv(irPin)){
    inputIR.enableIRIn();
  };

  TFT_UI::IRCodes initDefaultCodes();

  decode_results* decode(){
    if(inputIR.decode(&irResults)){
      inputIR.resume();
      return &irResults;
    }
    return NULL;
  };

 private:
  IRrecv inputIR;
  decode_results irResults;

};
#endif
