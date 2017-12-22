/**
Input_IR.cpp
*/

#include <Input_IR.hpp>
#include "IRremote.h"
#include "IRTest.h"
#include "Streaming.h"
TFTUI_NAMESPACE_USE

TFTUI_NAMESPACE_BEGIN

//IRCodes knownCodes;

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

TFT_UI::IRCodes Input_IR::initDefaultCodes(){
  IRCodes tmp;
  tmp.push_back(IRCommandCodePair(IRC_00,IRCommand_On));
  tmp.push_back(IRCommandCodePair(IRC_02,IRCommand_Off));

  tmp.push_back(IRCommandCodePair(IRC_21,IRCommand_Brighter));
  tmp.push_back(IRCommandCodePair(IRC_22,IRCommand_Dimer));

  tmp.push_back(IRCommandCodePair(IRC_11,IRCommand_Faster));
  tmp.push_back(IRCommandCodePair(IRC_12,IRCommand_Slower));

  tmp.push_back(IRCommandCodePair(IRC_21,IRCommand_Effect1));
  tmp.push_back(IRCommandCodePair(IRC_22,IRCommand_Effect2));
  tmp.push_back(IRCommandCodePair(IRC_22,IRCommand_Effect3));
  Serial << tmp.size()<<" IRCommands installed today"<<endl;
  return tmp;
}
