/**
Input_IR.cpp
*/

#include <Input_IR.hpp>
#include "IRremote.h"
#include "IRTest.h"
#include "Streaming.h"
TFTUI_NAMESPACE_BEGIN

/** The IR decoder */
int decodeIR(unsigned long now, void * userdata){
  //Serial << "decodeIR called with "<<(unsigned long)userdata <<endl;
  Input_IR *irRec = (Input_IR*)userdata;
  decode_results* dr = irRec->decode();
  if (dr){
     dumpInfo(dr);
   }
  return 0;
}
TFTUI_NAMESPACE_END
