#include <Arduino.h>
#include <IRremote.h>

void  ircode (decode_results *results);
void  encoding (decode_results *results);
void  dumpInfo (decode_results *results);
void  dumpRaw (decode_results *results);
void  dumpCode (decode_results *results);
