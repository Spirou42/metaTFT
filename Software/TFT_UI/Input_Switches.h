/**
* definition of the predefined switches
*/

#ifndef __SWITCHES_H__
#define __SWITCHES_H__
#include "Input_Switch.hpp"

typedef void(*SwitchIRQHandler)();

extern Switch switches[];
extern size_t numberOfSwitches;

extern UserEventQueue eventQueue;

extern SwitchIRQHandler switchHandlers[];

void enableSwitches();
void disableSwitches();
#endif
