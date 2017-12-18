/**
  ChristmasBall simple APA102 Demo
**/
#include <TFT_UI.h>
#include <TFT_UI_Highlevel.h>
#include <Queue.h>
#include <Streaming.h>
#include <FastLED.h>
#include <FastLEDAddOns.h>
#include "ChristmasBall.h"
#include "EffectLineBounce.h"
#include "EffectTorch.h"
#include "EffectNoise.h"

#define USE_BACKBUFFER 1
#define FLIPPED false
#define SERPENTIME true
#define ROTATED true
// queue for simple multitasking
Queue taskQueue;

// the buffer and backbuffer for the LEDS
CRGB leds[NUM_LEDS+1];
CRGB backBuffer[NUM_LEDS+1];
XYMatrix ledMatrix(MATRIX_WIDTH,MATRIX_HEIGHT,leds,
  #if USE_BACKBUFFER
    backBuffer,
  #else
    NULL,
  #endif
  SERPENTIME,FLIPPED,ROTATED);

// The TFT
TFTDisplay tft = TFTDisplay(TFT_CS,TFT_DC,TFT_RST,TFT_MOSI,TFT_SCK,TFT_MISO,TFT_LED,3);



EffectLineBounce lineBounceEffect = EffectLineBounce();
EffectTorch torchEffect = EffectTorch();
EffectNoise noiseEffect=EffectNoise();

EffectList initializeSystemEffects(){
  EffectList tmp;
  tmp.push_back(&noiseEffect);
  tmp.push_back(&lineBounceEffect);
  tmp.push_back(&torchEffect);

  return tmp;
}

// A pair of routines to static initialise the Palette and Effects lists of pairs
PaletteList initializeSystemPalettes(){
  PaletteList tmp;
  tmp.push_back(new PalettePair("Rainbow",RainbowColors_p));
  tmp.push_back(new PalettePair("Rainbow Stripes",RainbowStripeColors_p));
  tmp.push_back(new PalettePair("Clouds",CloudColors_p));
  tmp.push_back(new PalettePair("Ocean",OceanColors_p));
  tmp.push_back(new PalettePair("Forest",ForestColors_p));
  tmp.push_back(new PalettePair("Party",PartyColors_p));
  tmp.push_back(new PalettePair("Lava",LavaColors_p));
  tmp.push_back(new PalettePair("Heat",HeatColors_p));
  tmp.push_back(new PalettePair("Arctic",arctic_gp));
  tmp.push_back(new PalettePair("Temperature",temperature_gp));
  tmp.push_back(new PalettePair("Colombia",colombia_gp));
  tmp.push_back(new PalettePair("Cequal",cequal_gp));
  tmp.push_back(new PalettePair("Sunset",Another_Sunset_gp));
  tmp.push_back(new PalettePair("Yellow Sunset",Sunset_Yellow_gp));
  return tmp;
}

PaletteList systemPalettes = initializeSystemPalettes();
PaletteList::iterator currentSystemPalette = systemPalettes.begin();

EffectList FastLEDAddOns::systemEffectList = initializeSystemEffects();
EffectList::iterator FastLEDAddOns::currentRunningEffect = systemEffectList.begin();
// the UI
metaList  SystemMenu;               ///<< The main Menu of the Application
metaList  GlobalParameter;          ///<< all global Parameters
metaList  PalettesMenu;             ///<< Selection List for Palettes
metaList  EffectsMenu;              ///<< our Effects
metaValue ValueView;                ///<< View used for numerical value changes

// a couple of global parameters the action wrappers are defined in TFT_UI
uint8_t globalHue = 0;

int16_t tftBrightness = 0;
TFTBrightnessWrapper TFTBrightness(&tftBrightness,&tft);
ValueEditor tftBrightnessAction(&ValueView,&TFTBrightness);

int16_t ledBrightness = LED_BRIGHTNESS;
LEDBrightnessWrapper ledBrightnessWrapper(&ledBrightness);
ValueEditor ledBrightnessAction(&ValueView,&ledBrightnessWrapper);

int16_t hueStep = 1;
HUEStepWrapper hueStepWrapper(&hueStep);
ValueEditor hueStepAction(&ValueView,&hueStepWrapper);

int16_t hueFrameDelay = 0;
ValueWrapper hueDelayWrapper(&hueFrameDelay,0,24,"Hue Daylay");
ValueEditor hueDelayAction(&ValueView,&hueDelayWrapper);

#if USE_BACKBUFFER
int16_t blendFactor = 30;
ValueWrapper blendFactorWrapper(&blendFactor,1,255,"BlendFactor");
ValueEditor blendFactorAction(&ValueView,&blendFactorWrapper);
#endif

PaletteIndexWrapper paletteIndexWrapper(&systemPalettes,&currentSystemPalette);
ValueEditor paletteAction(&PalettesMenu,&paletteIndexWrapper);


EffectProgramWrapper programIndexWrapper(&FastLEDAddOns::systemEffectList,&FastLEDAddOns::currentRunningEffect);
ValueEditor programAction(&EffectsMenu,&programIndexWrapper);

ValueEditor globalParameterAction(&GlobalParameter,NULL);

void initSystemMenu(){
  // visual them definition for a single list entry

  SystemMenu.initView(&tft,GCRect(30,15,tft.width()/2,tft.height()-4));
  TFT_UI::initDefaultListVisual(SystemMenu);
  SystemMenu.setIsSelectList(false);
  metaLabel* l = SystemMenu.addEntry( String("Parameter"));
  l->setAction(&globalParameterAction);

  l=SystemMenu.addEntry(String("Effect"));
  l->setAction(&programAction);

  l=SystemMenu.addEntry( String("Palette"));
  l->setAction(&paletteAction);
#if USE_BACKBUFFER
  l = SystemMenu.addEntry(String("Blend Factor"));
  l->setAction(&blendFactorAction);
#endif
  // l = SystemMenu.addEntry(String("Parameter"));
  // l->setAction(&parameterAction);

  SystemMenu.sizeToFit();
  SystemMenu.layoutList();

}

void initGlobalParameterMenu(){
  GlobalParameter.initView(&tft,GCRect(30,15,tft.width()/2,tft.height()-4));
  TFT_UI::initDefaultListVisual(GlobalParameter);
  GlobalParameter.setIsSelectList(false);
  metaLabel* l = GlobalParameter.addEntry( String("TFT Brightness"));
  l->setAction(&tftBrightnessAction);

  l = GlobalParameter.addEntry(String("LED Brightness"));
  l->setAction(&ledBrightnessAction);

  l=GlobalParameter.addEntry( String("Hue Speed"));
  l->setAction(&hueStepAction);

  l=GlobalParameter.addEntry(String("Hue Frames"));
  l->setAction(&hueDelayAction);
  GlobalParameter.sizeToFit();
  GlobalParameter.layoutList();

}

void initPalettesMenu(){
  PalettesMenu.initView(&tft,GCRect(30,15,tft.width()/2,tft.height()-4));
  PalettesMenu.setIsSelectList(true);
  TFT_UI::initDefaultListVisual(PalettesMenu);
  PaletteList::iterator iter = systemPalettes.begin();
  while(iter != systemPalettes.end()){
    PalettesMenu.addEntry((*iter)->first);
    iter ++;
  }
  PalettesMenu.sizeToFit();
  PalettesMenu.layoutList();
}

void initEffectsMenu(){
  EffectsMenu.initView(&tft,GCRect(30,15,tft.width()/2,tft.height()-4));
  EffectsMenu.setIsSelectList(true);
  TFT_UI::initDefaultListVisual(EffectsMenu);
  EffectList::iterator iter = FastLEDAddOns::systemEffectList.begin();
  while(iter != FastLEDAddOns::systemEffectList.end()){
    EffectsMenu.addEntry((*iter)->name());
    iter++;
  }
  EffectsMenu.sizeToFit();
  EffectsMenu.layoutList();
}

void initValueView(){
  Serial <<"initValueView"<<endl;
  ValueView.initValue(&tft,GCRect(130,00,13,8));

  metaValue::ValueLayout k = TFT_UI::defaultValueLayout();

  ValueView.setLayout(k);
  ValueView.setValueWrapper(&TFTBrightness);
  ValueView.sizeToFit();
  ValueView.setProcessEvents(true);
  ValueView.setValue(TFTBrightness.getValue());

  ValueView.allignInSuperView(HALLIGN_CENTER | VALLIGN_CENTER);
  // valueStr.remove(0);
  // valueStr+=String("10");
  // ValueView.setValue(valueStr);

}

void initUI(){
  initSystemMenu();
  initPalettesMenu();
  initValueView();
  initEffectsMenu();
  initGlobalParameterMenu();
}


void initialiseLEDs(){
  FastLED.addLeds<CHIPSET, LED_PIN, CLOCK_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(COLOR_CORRECTION);
  FastLED.clear(true);
  FastLED.show();
  FastLED.setBrightness( LED_BRIGHTNESS );
  fill_solid(leds,NUM_LEDS,CRGB::Red);
  FastLED.show();
}

void initialiseTFT(){
  tft.start();
  TFTBrightness.setValue(13);
}

#if USE_BACKBUFFER
int backbufferBlender(unsigned long now, void *data){
  uint8_t frac = blendFactor;
  static uint8_t lastFrac =0;
  if(frac != lastFrac){
  	//		Serial << "frac"<<frac<<endl;
  	lastFrac = frac;
  }

  #if DEBUG_EFFECTS
  Serial <<".";
  #endif
  // if(frac < 4){
  // 	#if DEBUG_EFFECTS
  // 	Serial << "Frac cliped to 4, was "<<frac<<endl;
  // 	#endif
  // 	frac = 4;
  // }
  for(uint16_t i=0;i<NUM_LEDS;i++){
  	leds[i]=nblend(leds[i],backBuffer[i],frac);
  }
  // if( millis() < 5000 ) {
  // 	FastLED.setBrightness( scale8( Brightness.currentValue(), (millis() * 256) / 5000));
  // } else {
  // 	FastLED.setBrightness(Brightness.currentValue());
  // }
  FastLED.show();
  return 0;
}
#endif
int16_t currentFrameRate;
void postFrameCallback(unsigned long now){
  static int hueDelay = 0;
  int16_t hFD = hueStepWrapper.frameDelay() + hueFrameDelay*10;
  int16_t hueStep = hueStepWrapper.hueStep();
  if(hueDelay>=hFD) {
    globalHue+=hueStep;
    hueDelay = 0;
  }else{
    hueDelay ++;
  }
}

void setup(){
  Serial.begin(115200);
  delay(5000);
  initialiseLEDs();

  //init LED backlight
  pinMode(TFT_LED,OUTPUT);
  digitalWriteFast(TFT_LED,1);

  initialiseTFT();
  // enable UI
  enableSwitches();
  enableEncoders();


  initUI();
  tft.fillScreen(ILI9341_BLACK);

  taskQueue.scheduleFunction(TFT_UI::processUserEvents,NULL,"USER",0,100);
  taskQueue.scheduleFunction(FastLEDAddOns::effectRunner,(void*)postFrameCallback,"EFCT",0, 1000/60);
  currentFrameRate = 1000/60;
  #if USE_BACKBUFFER
  taskQueue.scheduleFunction(backbufferBlender,NULL,"BBLD",0,1000/120);
  #endif
}

void loop (){
  if(TFT_UI::responderStack.empty()){
    TFT_UI::responderStack.push_back(&SystemMenu);
    SystemMenu.prepareForDisplay();
    SystemMenu.redraw();
    Serial << "Instanciate the System Menu"<<endl;
  }
  if(currentFrameRate != FastLEDAddOns::requestedFrameRate){
    Serial << "Change FrameRate to "<<FastLEDAddOns::requestedFrameRate<<endl;
    if(FastLEDAddOns::requestedFrameRate == 0)
      FastLEDAddOns::requestedFrameRate = 1;
    currentFrameRate = FastLEDAddOns::requestedFrameRate;
    taskQueue.scheduleChangeFunction("EFCT", millis() , currentFrameRate);

  }

  taskQueue.Run(millis());
}
