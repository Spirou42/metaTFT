/**
  ChristmasBall simple APA102 Demo
**/
#define FASTLED_INTERNAL
#include <TFT_UI.h>
#include <TFT_UI_Highlevel.h>
#include <Queue.h>
#include <Streaming.h>
#include <FastLED.h>
#include <FastLEDAddOns.h>
#include <IRTest.h>
#include "MatrixDemo.h"
#include "EffectLineBounce.h"
#include "EffectTorch.h"
#include "EffectNoise.h"
//#include "EffectLava.h"
#include "EffectRain.h"

#define USE_BACKBUFFER 1

#if USE_CYLINDERLAMP
#warning CYLINDER
  #define FLIPPED false
  #define SERPENTIME false
  #define ROTATED false
#elif USE_CHRISTMASBALL
#warning CHRISTMASBALL
  #define FLIPPED     true
  #define SERPENTIME  true
  #define ROTATED     true
#endif
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

void displayStartScreen(TFTDisplay* tft);

// The TFT
TFTDisplay tft = TFTDisplay(TFT_CS,TFT_DC,TFT_RST,TFT_MOSI,TFT_SCK,TFT_MISO,TFT_LED,displayStartScreen,3);
Input_IR IRReciever = Input_IR(IR_IN);


EffectLineBounce lineBounceEffect = EffectLineBounce();
EffectTorch torchEffect = EffectTorch();
EffectNoise noiseEffect=EffectNoise();
//EffectLava lavaEffect=EffectLava();
EffectRain rainEffect=EffectRain();

EffectList initializeSystemEffects(){
  EffectList tmp;
  //tmp.push_back(&lavaEffect);
  tmp.push_back(&rainEffect);
  tmp.push_back(&noiseEffect);
  tmp.push_back(&lineBounceEffect);
  tmp.push_back(&torchEffect);
  return tmp;
}

// A pair of routines to static initialise the Palette and Effects lists of pairs
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

PaletteList systemPalettes = initializeSystemPalettes();
PaletteList::iterator currentSystemPalette = systemPalettes.begin();

EffectList FastLEDAddOns::systemEffectList = initializeSystemEffects();
EffectList::iterator FastLEDAddOns::currentRunningEffect = systemEffectList.begin();
TFT_UI::IRCodes TFT_UI::knownCodes = IRReciever.initDefaultCodes();

TFT_UI::ResponderStack labelList; ///< temorary storage for not used

// the UI
metaList  SystemMenu;               ///<< The main Menu of the Application
metaList  GlobalParameter;          ///<< all global Parameters
metaList  PalettesMenu;             ///<< Selection List for Palettes
metaList  EffectsMenu;              ///<< our Effects
metaList  EffectParameter;          ///< the public parameters of the current running effect
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

ValueEditor effectParameterAction(&EffectParameter,NULL);

metaLabel* effectParameterLabel;

void initSystemMenu(){
  // visual them definition for a single list entry
  SystemMenu.initView(&tft,GCRect(30,15,tft.width()/2,tft.height()-4));
  TFT_UI::initDefaultListVisual(SystemMenu);
  SystemMenu.setIsSelectList(false);
  metaLabel* l = SystemMenu.addEntry( String("System"));
  l->setAction(&globalParameterAction);

  l = SystemMenu.addEntry(String("Brightness"));
  l->setAction(&ledBrightnessAction);

  l=SystemMenu.addEntry(String("Effect"));
  l->setAction(&programAction);

  l=SystemMenu.addEntry( String("Palette"));
  l->setAction(&paletteAction);

  effectParameterLabel = SystemMenu.addEntry(String("Effect Para"));
  effectParameterLabel->setAction(&effectParameterAction);

  SystemMenu.sizeToFit();
  SystemMenu.layoutList();

  EffectParameter.initView(&tft,GCRect(30,15,tft.width()/2,tft.height()-4));
  TFT_UI::initDefaultListVisual(EffectParameter);
}

void initGlobalParameterMenu(){
  GlobalParameter.initView(&tft,GCRect(30,15,tft.width()/2,tft.height()-4));
  TFT_UI::initDefaultListVisual(GlobalParameter);
  GlobalParameter.setIsSelectList(false);
  metaLabel* l = GlobalParameter.addEntry( String("TFT Brightness"));
  l->setAction(&tftBrightnessAction);

  l=GlobalParameter.addEntry( String("Hue Speed"));
  l->setAction(&hueStepAction);

  l=GlobalParameter.addEntry(String("Hue Frames"));
  l->setAction(&hueDelayAction);

  #if USE_BACKBUFFER
    l = GlobalParameter.addEntry(String("Blend Factor"));
    l->setAction(&blendFactorAction);
  #endif

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
  FastLED.addLeds<CHIPSET, LED_PIN, CLOCK_PIN, COLOR_ORDER,DATA_RATE_MHZ(3)>(leds, NUM_LEDS).setCorrection(COLOR_CORRECTION);
  FastLED.clear(true);
  FastLED.show();
  FastLED.setBrightness( LED_BRIGHTNESS );
  fill_solid(leds,NUM_LEDS,CRGB::DarkOrange);
  FastLED.show();
}


void displayStartScreen(TFTDisplay* tft){
  //tft->setFont(Arial_48_Bold);
  tft->setTextSize(7);
  tft->setTextColor(ILI9341_GREEN);
  tft->setCursor(25,15);
  #if USE_CYLINDERLAMP
  *tft<<" deep"<<endl<<"    NET"<<endl;
  #elif USE_CHRISTMASBALL
  tft->setTextSize(5);
  *tft<<"CHRISTMAS"<<endl<<"    BALL"<<endl;
  #endif
  tft->drawFastHLine(tft->getCursorX(),tft->getCursorY()+10,tft->width(),ILI9341_GREEN);
  tft->setTextColor(ILI9341_YELLOW);
  tft->setTextSize(2);
  *tft<<endl;

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

void dumpParameters(Effect* currentEffect)
{
  Serial<<"Name: "<<currentEffect->name()<<endl;
  for(size_t k = 0;k<currentEffect->numberOfParameters();k++){
    Serial<<"Parameter: "<<currentEffect->parameterNameAt(k)<<"\t";
    ValueWrapper* vw = currentEffect->parameterAt(k);
    if(vw){
        Serial << vw->getValue()<<"\t("<<vw->getMinValue()<<", "<<vw->getMaxValue()<<")"<<endl;
    }else{
      Serial << endl;
    }
  }
  Serial << "----"<<endl;
}

int16_t currentFrameRate;
void postFrameCallback(unsigned long now){
  static int hueDelay = 0;
  static EffectList::iterator lastEffect;

  int16_t hFD = hueStepWrapper.frameDelay() + hueFrameDelay*10;
  int16_t hueStep = hueStepWrapper.hueStep();
  if(hueDelay>=hFD) {
    globalHue+=hueStep;
    hueDelay = 0;
  }else{
    hueDelay ++;
  }
  if(lastEffect != FastLEDAddOns::currentRunningEffect){
    Effect *p = (*FastLEDAddOns::currentRunningEffect);
    if(p->numberOfParameters()){
      if(!effectParameterLabel->getSuperview()){
        SystemMenu.addSubview(effectParameterLabel);
        SystemMenu.sizeToFit();
        SystemMenu.layoutList();
      }
    }else{
      effectParameterLabel->removeFromSuperview();
      SystemMenu.sizeToFit();
      tft.fillScreen(0);
      SystemMenu.layoutList();
      TFT_UI::ResponderStack::iterator iter = TFT_UI::responderStack.begin();
      while(iter !=TFT_UI::responderStack.end() ){
        Serial << "Redraw"<<_HEX((unsigned long)*iter)<<endl;
        (*iter)->setNeedsRedraw();
        (*iter)->redraw();
        iter++;
      }
    }
    lastEffect = FastLEDAddOns::currentRunningEffect;
    dumpParameters(*FastLEDAddOns::currentRunningEffect);
  }
}

void setup(){
  Serial.begin(115200);
  //delay(5000);
  initialiseLEDs();

  //init LED backlight
  pinMode(TFT_LED,OUTPUT);
  digitalWriteFast(TFT_LED,1);

  initialiseTFT();
  //delay(3000);
  // enable UI
  enableSwitches();
  enableEncoders();


  initUI();
  tft.fillScreen(ILI9341_BLACK);

  /** process our userevents */
  taskQueue.scheduleFunction(TFT_UI::processUserEvents,NULL,"USER",0,100);
  // startup the IR receiver
  taskQueue.scheduleFunction(TFT_UI::decodeIR,(void*)&IRReciever,"IRIP",0,100 );


  // startup the renderer
  currentFrameRate = 1000/60;
  taskQueue.scheduleFunction(FastLEDAddOns::effectRunner,(void*)postFrameCallback,"EFCT",0, 1000/60);
  #if USE_BACKBUFFER
  taskQueue.scheduleFunction(backbufferBlender,NULL,"BBLD",0,1000/120);
  #endif
  Serial << TFT_UI::knownCodes.size()<<" IRCommands installed"<<endl;

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
