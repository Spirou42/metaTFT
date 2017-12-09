/**
* Palettes.h
* definitions of some Palettes
*/


#ifndef __PALETTES_H__
#define __PALETTES_H__

DEFINE_GRADIENT_PALETTE( arctic_gp ) {
    0,   2,  8, 23,
    2,   2,  8, 24,
    5,   3,  9, 26,
    8,   3, 10, 28,
   11,   4, 10, 29,
   14,   4, 10, 30,
   17,   4, 11, 31,
   19,   5, 12, 33,
   22,   5, 13, 35,
   25,   6, 14, 37,
   28,   7, 15, 38,
   31,   7, 16, 40,
   33,   8, 17, 42,
   36,   8, 18, 43,
   39,   9, 20, 46,
   42,  10, 22, 49,
   45,  11, 22, 50,
   48,  12, 24, 52,
   51,  13, 25, 54,
   53,  15, 28, 59,
   56,  16, 29, 61,
   59,  17, 31, 63,
   62,  18, 33, 65,
   65,  19, 35, 68,
   68,  21, 37, 72,
   70,  23, 40, 75,
   73,  25, 43, 79,
   76,  26, 45, 82,
   79,  30, 49, 85,
   82,  33, 53, 88,
   84,  39, 58, 92,
   87,  44, 62, 96,
   90,  47, 67, 99,
   93,  52, 71,102,
   96,  56, 75,106,
   99,  61, 81,108,
  102,  69, 88,114,
  104,  74, 92,117,
  107,  79, 97,120,
  110,  83,101,123,
  113,  88,107,127,
  116,  95,112,130,
  119, 103,119,133,
  121, 106,122,137,
  124, 110,127,138,
  127, 113,130,142,
  130, 118,135,144,
  133, 123,138,145,
  135, 125,141,147,
  138, 128,144,149,
  141, 130,144,149,
  141,  64,166, 48,
  142,  69,178, 49,
  142,  73,180, 50,
  143,  78,184, 51,
  143,  82,184, 51,
  144,  86,186, 51,
  145,  88,187, 52,
  145,  94,189, 52,
  146,  98,191, 52,
  146, 104,195, 52,
  147, 109,197, 53,
  147, 112,197, 53,
  148, 117,199, 53,
  149, 120,201, 54,
  149, 126,203, 54,
  150, 133,205, 55,
  150, 139,207, 55,
  151, 144,209, 56,
  151, 148,211, 57,
  152, 153,213, 57,
  153, 161,215, 59,
  153, 169,217, 59,
  154, 173,219, 60,
  154, 177,221, 60,
  155, 182,219, 59,
  155, 184,213, 55,
  155, 182,217, 57,
  158, 184,213, 55,
  161, 186,209, 53,
  164, 188,201, 49,
  167, 190,197, 47,
  170, 192,193, 44,
  172, 192,189, 42,
  175, 194,186, 41,
  178, 197,182, 38,
  181, 199,178, 37,
  184, 199,173, 35,
  186, 203,166, 32,
  189, 206,162, 31,
  192, 208,159, 29,
  195, 208,156, 27,
  198, 210,152, 27,
  201, 213,149, 25,
  204, 215,146, 24,
  206, 215,142, 23,
  209, 217,138, 21,
  212, 220,131, 19,
  215, 217,128, 18,
  218, 206,125, 18,
  221, 197,122, 18,
  223, 186,119, 18,
  226, 177,117, 18,
  229, 169,114, 18,
  232, 153,108, 18,
  235, 142,105, 18,
  237, 135,103, 18,
  240, 128,100, 18,
  243, 120, 97, 18,
  246, 113, 95, 18,
  249, 107, 93, 18,
  252, 101, 91, 18,
  255,  94, 88, 18};

// Gradient palette "temperature_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/arendal/tn/temperature.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 144 bytes of program space.
DEFINE_GRADIENT_PALETTE( temperature_gp ) {
    0,   1, 27,105,
   14,   1, 27,105,
   14,   1, 40,127,
   28,   1, 40,127,
   28,   1, 70,168,
   42,   1, 70,168,
   42,   1, 92,197,
   56,   1, 92,197,
   56,   1,119,221,
   70,   1,119,221,
   70,   3,130,151,
   84,   3,130,151,
   84,  23,156,149,
   99,  23,156,149,
   99,  67,182,112,
  113,  67,182,112,
  113, 121,201, 52,
  127, 121,201, 52,
  127, 142,203, 11,
  141, 142,203, 11,
  141, 224,223,  1,
  155, 224,223,  1,
  155, 252,187,  2,
  170, 252,187,  2,
  170, 247,147,  1,
  184, 247,147,  1,
  184, 237, 87,  1,
  198, 237, 87,  1,
  198, 229, 43,  1,
  212, 229, 43,  1,
  212, 220, 15,  1,
  226, 220, 15,  1,
  226, 171,  2,  2,
  240, 171,  2,  2,
  240,  80,  3,  3,
  255,  80,  3,  3};
// Gradient palette "colombia_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/wkp/shadowxfox/tn/colombia.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 112 bytes of program space.
DEFINE_GRADIENT_PALETTE( colombia_gp ) {
    0,   0,  2, 14,
   28,   0,  2, 14,
   28,   0,  7, 25,
   56,   0,  7, 25,
   56,   0, 33, 71,
   84,   0, 33, 71,
   84,   0, 82,147,
   99,   0, 82,147,
   99,  22,149,255,
  107,  22,149,255,
  107, 132,215,255,
  113, 132,215,255,
  113,  61,105, 49,
  116,  61,105, 49,
  116,  91,135, 55,
  119,  91,135, 55,
  119, 117,156, 67,
  127, 117,156, 67,
  127, 190,189, 99,
  141, 190,189, 99,
  141, 194,152, 65,
  170, 194,152, 65,
  170, 144,105, 48,
  198, 144,105, 48,
  198, 104, 81, 45,
  226, 104, 81, 45,
  226,  69, 51, 23,
  255,  69, 51, 23};

// Gradient palette "cequal_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/imagej/tn/cequal.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 1860 bytes of program space.
DEFINE_GRADIENT_PALETTE( cequal_gp ) {
    0,   1,  1,  1,
   36,   1,  3,  1,
   76,   1, 41,123,
  112,  59,  1,252,
  127, 117,  1,180,
  172, 239, 81,  1,
  195, 252,180,  1,
  222, 252,250,147,
  255, 252,252,252};
// Gradient palette "Another_Sunset_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/nd/atmospheric/tn/Another_Sunset.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 32 bytes of program space.
DEFINE_GRADIENT_PALETTE( Another_Sunset_gp ) {
  0, 110, 49, 11,
  29,  55, 34, 10,
  68,  22, 22,  9,
  68, 239,124,  8,
  97, 220,156, 27,
  124, 203,193, 61,
  178,  33, 53, 56,
  255,   0,  1, 52};


#endif
