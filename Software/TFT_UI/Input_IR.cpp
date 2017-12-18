/**
Input_IR.cpp
*/

#include <Input_IR.hpp>
#include "IRremote.h"
TFTUI_NAMESPACE_BEGIN

int decodeIR(unsigned long now, void * userdata){
  Input_IR &tmp = (Input_IR&)userdata;
  return 0;
}
TFTUI_NAMESPACE_END
