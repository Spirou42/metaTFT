/**
EffectTorch.h
*/
#include "MatrixDemo.h"

#define numLeds NUM_LEDS
#define ledsPerLevel MATRIX_WIDTH
#define levels MATRIX_HEIGHT
#define USE_RED_THEME 1
class EffectTorch: public Effect {
public:
  EffectTorch():Effect("Torch"){}
  uint16_t cycle_wait = 0; // 0..255
#ifdef USE_RED_THEME
  byte flame_min = 20; // 0..255
  byte flame_max = 220; // 0..255

  byte random_spark_probability = 5; // 0..100
  byte spark_min = 220; // 0..255
  byte spark_max = 255; // 0..255

  byte spark_tfr = 40; // 0..256 how much energy is transferred up for a spark per cycle
  uint16_t spark_cap = 195; // 0..255: spark cells: how much energy is retained from previous cycle

  uint16_t up_rad = 40; // up radiation
  uint16_t side_rad = 35; // sidewards radiation
  uint16_t heat_cap = 80; // 0..255: passive cells: how much energy is retained from previous cycle
  bool usePalette = false;

  byte red_bg = 0;
  byte green_bg = 0;
  byte blue_bg = 0;

  byte red_bias = 10;
  byte green_bias = 0;
  byte blue_bias = 0;

  int red_energy = 180;
  int green_energy = 20; // 145;
  int blue_energy = 0;
#elif USE_BLUE_THEME
  byte flame_min = 20; // 0..255
  byte flame_max = 220; // 0..255

  byte random_spark_probability = 3; // 0..100
  byte spark_min = 220; // 0..255
  byte spark_max = 255; // 0..255

  byte spark_tfr = 60; // 0..256 how much energy is transferred up for a spark per cycle
  uint16_t spark_cap = 150; // 0..255: spark cells: how much energy is retained from previous cycle

  uint16_t up_rad = 60; // up radiation
  uint16_t side_rad = 35; // sidewards radiation
  uint16_t heat_cap = 120; // 0..255: passive cells: how much energy is retained from previous cycle
  bool usePalette = false;

  byte red_bg = 0;
  byte green_bg = 0;
  byte blue_bg = 0;

  byte red_bias = 10;
  byte green_bias = 20;
  byte blue_bias = 10;

  int red_energy = 50;
  int green_energy = 50; // 145;
  int blue_energy = 180;
#endif
  byte currentEnergy[numLeds+1]; // current energy level
  byte nextEnergy[numLeds+1]; // next energy level
  byte energyMode[numLeds+1]; // mode how energy is calculated for this point

  enum {
    torch_passive = 0, // just environment, glow from nearby radiation
    torch_nop = 1, // no processing
    torch_spark = 2, // slowly looses energy, moves up
    torch_spark_temp = 3, // a spark still getting energy from the level below
  };

  inline void reduce(byte &aByte, byte aAmount, byte aMin = 0)
  {
    int r = aByte-aAmount;
    if (r<aMin)
    aByte = aMin;
    else
    aByte = (byte)r;
  }


  inline void increase(byte &aByte, byte aAmount, byte aMax = 255)
  {
    int r = aByte+aAmount;
    if (r>aMax)
    aByte = aMax;
    else
    aByte = (byte)r;
  }

  uint16_t random2(uint16_t aMinOrMax, uint16_t aMax = 0)
  {
    if (aMax==0) {
      aMax = aMinOrMax;
      aMinOrMax = 0;
    }
    uint32_t r = aMinOrMax;
    aMax = aMax - aMinOrMax + 1;
    r += rand() % aMax;
    return r;
  }

  void calcNextEnergy()
  {
    int i = 0;
    for (int y=0; y<levels; y++) {
      for (int x=0; x<ledsPerLevel; x++) {
        i = ledMatrix.XY(x,y);
        byte e = currentEnergy[i];
        byte m = energyMode[i];
        switch (m) {
          case torch_spark: {
            // loose transfer up energy as long as the is any
            reduce(e, spark_tfr);
            // cell above is temp spark, sucking up energy from this cell until empty
            if (y<(levels-1)) {
              energyMode[ledMatrix.XY(x,y+1)] = torch_spark_temp;
            }
            break;
          }
          case torch_spark_temp: {
            // just getting some energy from below
            byte e2 = currentEnergy[ledMatrix.XY(x,y-1)];
            if (e2<spark_tfr) {
              // cell below is exhausted, becomes passive
              energyMode[ledMatrix.XY(x,y-1)] = torch_passive;
              // gobble up rest of energy
              increase(e, e2);
              // loose some overall energy
              e = ((int)e*spark_cap)>>8;
              // this cell becomes active spark
              energyMode[i] = torch_spark;
            } else {
              increase(e, spark_tfr);
            }
            break;
          }
          case torch_passive: {
            e = ((int)e*heat_cap)>>8;
            increase(e, ((((int)currentEnergy[ledMatrix.XY(x-1,y)]+
            (int)currentEnergy[ledMatrix.XY(x+1,y)])*side_rad)>>9) +
            (((int)currentEnergy[ledMatrix.XY(x,y-1)]*up_rad)>>8));
          }
          default:
          break;
        }
        nextEnergy[i] = e;
      }
    }
  }

  const uint8_t energymap[32] = {0, 64, 96, 112, 128, 144, 152, 160, 168, 176, 184, 184, 192, 200, 200, 208, 208, 216, 216, 224, 224, 224, 232, 232, 232, 240, 240, 240, 240, 248, 248, 248};

  void calcNextColors(){
    for(int x=0;x<MATRIX_WIDTH;x++){
      for(int y=0;y<MATRIX_HEIGHT;y++){
        uint16_t p = ledMatrix.XY(x,y);
        uint16_t e = nextEnergy[p];
        currentEnergy[p] = e;
        CRGB color;
        if(usePalette){
          color = ColorFromPalette((*currentSystemPalette)->second,e+globalHue);
        }else{
          if (e>250){
            color = CRGB(170, 170, e); // blueish extra-bright spark
          } else if (e>0) {
            //energy to brightness is non-linear
            byte eb = energymap[e>>3];
            byte r = red_bias;
            byte g = green_bias;
            byte b = blue_bias;
            increase(r, (eb*red_energy)>>8);
            increase(g, (eb*green_energy)>>8);
            increase(b, (eb*blue_energy)>>8);
            color = CRGB(r, g, b);
          } else {
            // background, no energy
            color = CRGB::Black;//(red_bg, green_bg, blue_bg);
          }
        }
        ledMatrix.setPixel(x,y,color);
      }
    }
  }


  void injectRandom(){
    // random flame energy at bottom row
    for (int i=0; i<ledsPerLevel; i++) {
      int p = ledMatrix.XY(i,0);
      currentEnergy[p] = random2(flame_min, flame_max);
      energyMode[p] = torch_nop;
    }
    // random sparks at second row
    for (int i=0; i<ledsPerLevel; i++) {
      int p = ledMatrix.XY(i,1);
      if (energyMode[p]!=torch_spark && random2(100)<random_spark_probability) {
        currentEnergy[p] = random2(spark_min, spark_max);
        energyMode[p] = torch_spark;
      }
    }
  }

  void startEffect()
  {
    currentSystemPalette = systemPalettes.begin()+8;
    blendFactor = 50;

    for (int i=0; i<numLeds; i++) {
      currentEnergy[i] = 0;
      nextEnergy[i] = 0;
      energyMode[i] = torch_passive;
    }
    // #if USE_RED_THEME
    // ledMatrix.setFlipped(false);
    // #elif USE_BLUE_THEME
    // ledMatrix.setFlipped(false);
    // #endif
  }
  virtual void frame(unsigned long now) {
    injectRandom();
    calcNextEnergy();
    calcNextColors();
    ledMatrix.flush();
  }
  virtual uint16_t frameRate(){return 1000/30;}
};
