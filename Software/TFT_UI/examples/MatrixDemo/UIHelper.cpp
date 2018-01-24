/**
  UIHelper - routines handling the TFT UI
*/
#include "UIHelper.h"

extern Effect rainEffect;
extern Effect noiseEffect;
extern Effect torchEffect;
extern Effect lineBounceEffect;
extern Effect lavaEffect;

extern int16_t tftBrightness, ledBrightness, hueStep, hueFrameDelay;

EffectList initializeSystemEffects(){
  EffectList tmp;
  tmp.push_back(&rainEffect);
  tmp.push_back(&noiseEffect);
  tmp.push_back(&lineBounceEffect);
  tmp.push_back(&torchEffect);
  tmp.push_back(&lavaEffect);
  return tmp;
}

PaletteList initializeSystemPalettes(){
  PaletteList tmp;
  tmp.push_back(new PalettePair("Rainbow",            Palette_t(RainbowColors_p,true)));
  tmp.push_back(new PalettePair("Rainbow Stripes",    Palette_t(RainbowStripeColors_p,false)));
  tmp.push_back(new PalettePair("Clouds",             Palette_t(CloudColors_p,false)));
  tmp.push_back(new PalettePair("Ocean",              Palette_t(OceanColors_p,false)));
  tmp.push_back(new PalettePair("Forest",             Palette_t(ForestColors_p,false)));
  tmp.push_back(new PalettePair("Party",              Palette_t(PartyColors_p,true)));
  tmp.push_back(new PalettePair("Lava",               Palette_t(LavaColors_p,false)));
  tmp.push_back(new PalettePair("Heat",               Palette_t(HeatColors_p,true)));
  tmp.push_back(new PalettePair("Arctic",             Palette_t(arctic_gp,true)));
  tmp.push_back(new PalettePair("Temperature",        Palette_t(temperature_gp,false)));
  tmp.push_back(new PalettePair("Colombia",           Palette_t(colombia_gp,false)));
  tmp.push_back(new PalettePair("Cequal",             Palette_t(cequal_gp,true)));
  tmp.push_back(new PalettePair("Sunset",             Palette_t(Another_Sunset_gp,false)));
  tmp.push_back(new PalettePair("Yellow Sunset",      Palette_t(Sunset_Yellow_gp,false)));
  return tmp;
}
