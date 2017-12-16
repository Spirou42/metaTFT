/**
Input_IR.hpp
*/

#ifndef __Input_IR_HPP__
#define __Input_IR_HPP__

#include "IRremote.h"


class Input_IR{
public:
  Input_IR(int irPin):inputIR(IRrecv(irPin)){
    inputIR.enableIRIn();
  };
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
