/**
 This file contains IR codes from received from the default 21 Button chinese RGB LED-Lamps

 Layout 3 x 7
      ON         LMP         OFF
     Flash      Clock1      Clock2
    smooth      Dim up     Dim down
      Red       green        Blue
      C1         C2          C3
      C4         C5          C6
      C7         C8         White
 */

 #ifndef __IRCODES_LEDLAMP1_H__
 #define __IRCODES_LEDLAMP1_H__

 #include <Arduino.h>
 #include <vector>
TFTUI_NAMESPACE_BEGIN

 typedef enum _irCommand {
  IRCommand_None = 0,      // this is sysnonyme for no or not known IRCommand
  IRCommand_On,           //ON
  IRCommand_Off,          //OFF
  IRCommand_Dimer,        //Dim up
  IRCommand_Brighter,     //Dim down
  IRCommand_Faster,       //Clock1
  IRCommand_Slower,       //Clock2

  IRCommand_Effect1,
  IRCommand_Effect2,
  IRCommand_Effect3,
 }IRCommand_t;

 typedef std::pair<uint64_t,IRCommand_t> IRCommandCodePair;
 typedef std::vector<IRCommandCodePair> IRCodes;

TFTUI_NAMESPACE_END

#define IRC_00  0xFFA25D
#define IRC_01  0xFF629D
#define IRC_02  0xFFE21D

#define IRC_10  0xFF22DD
#define IRC_11  0xFF02FD
#define IRC_12  0xFFC23D

#define IRC_20  0xFFE01F
#define IRC_21  0xFFA857
#define IRC_22  0xFF906F

#define IRC_30  0xFF6897
#define IRC_31  0xFF9867
#define IRC_32  0xFFB04F

#define IRC_40  0xFF30CF
#define IRC_41  0xFF18E7
#define IRC_42  0xFF7A85

#define IRC_50  0xFF10EF
#define IRC_51  0xFF38C7
#define IRC_52  0xFF5AA5

#define IRC_60  0xFF42BD
#define IRC_61  0xFF4AB5
#define IRC_62  0xFF52AD


 #endif
