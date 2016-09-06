/**
* definition of predifined encoders
*/

#ifndef __ENCODERS_H__
#define __ENCODERS_H__
#include "Input_Encoder.hpp"

typedef void(*EncoderIRQHandler)();

extern Encoder encoders[];
extern size_t numberOfEncoders;
extern UserEventQueue eventQueue;

extern EncoderIRQHandler encoderHandlers[];

void enableEncoders();
void disableEncoders();
#endif
