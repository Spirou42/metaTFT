/**
* predefined Switches and IRQ handlers
*/

#include "Input_Switches.h"


Switch switches[]={
  Switch(ButtonID::RightButton,&TFT_UI::eventQueue,20,0),
  Switch(ButtonID::LeftButton,&TFT_UI::eventQueue,19,1),
  Switch(ButtonID::UpButton,&TFT_UI::eventQueue,18,2),
  Switch(ButtonID::DownButton,&TFT_UI::eventQueue,17,3),
  Switch(ButtonID::CenterButton,&TFT_UI::eventQueue,16,4),
};

size_t numberOfSwitches = sizeof(switches) / sizeof(Switch);
void switch0Handler();
void switch1Handler();
void switch2Handler();
void switch3Handler();
void switch4Handler();

SwitchIRQHandler switchHandlers[] = {
  switch0Handler,switch1Handler,switch2Handler,switch3Handler,switch4Handler
};


void enableSwitches(){
  for(size_t i = 0;i<numberOfSwitches;++i){
    switches[i].enable();
  }
}

void disableSwitches(){
  for(size_t i=0;i<numberOfSwitches;++i){
    switches[i].disable();
  }
}
void switchHandler(size_t idx){
  bool pinState = digitalReadFast(switches[idx].getPin());
  if(pinState){
    switches[idx].doSwitchDown();
  }else{
    switches[idx].doSwitchUp();
  }
}

void switch0Handler(){
  __disable_irq();
  switchHandler(0);
  __enable_irq();
}

void switch1Handler(){
  __disable_irq();
  switchHandler(1);
  __enable_irq();
}

void switch2Handler(){
  __disable_irq();
  switchHandler(2);
  __enable_irq();
}
void switch3Handler(){
  __disable_irq();
  switchHandler(3);
  __enable_irq();
}
void switch4Handler(){
  __disable_irq();
  switchHandler(4);
  __enable_irq();
}
