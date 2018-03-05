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
#include "EffectLava.h"
#include "EffectRain.h"
#include "UIHelper.h"


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

/**
  Setup global variables and infrastructure
*/

// queue for simple multitasking
Queue taskQueue;

// LED buffers and the matrix mapping object
CRGB leds[NUM_LEDS+1];
CRGB backBuffer[NUM_LEDS+1];
XYMatrix ledMatrix(MATRIX_WIDTH,MATRIX_HEIGHT,leds,
  #if USE_BACKBUFFER
    backBuffer,
  #else
    NULL,
  #endif
  SERPENTIME,FLIPPED,ROTATED);

// the TFT and IR receiver
TFTDisplay tft = TFTDisplay(TFT_CS,TFT_DC,TFT_RST,TFT_MOSI,TFT_SCK,TFT_MISO,TFT_LED,displayStartScreen,3);
Input_IR IRReciever = Input_IR(IR_IN);

// LED Effects
EffectLineBounce lineBounceEffect = EffectLineBounce();
EffectTorch torchEffect = EffectTorch();
EffectNoise noiseEffect=EffectNoise();
EffectLava lavaEffect=EffectLava();
EffectRain rainEffect=EffectRain();


// The global palettes
PaletteList systemPalettes = initializeSystemPalettes();
PaletteList::iterator currentSystemPalette = systemPalettes.begin();

// The Effects k
EffectList FastLEDAddOns::systemEffectList = initializeSystemEffects();
EffectList::iterator FastLEDAddOns::currentRunningEffect = systemEffectList.begin();

// The IRCodes
TFT_UI::IRCodes TFT_UI::knownCodes = IRReciever.initDefaultCodes();

// Storage for not used list entries
std::vector<metaLabel*> labelList; ///< temorary storage for not used

// the Lists and editors
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

/**
Initialise the UI
*/
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

void initialiseLEDs(){
  FastLED.addLeds<CHIPSET, LED_PIN, CLOCK_PIN, COLOR_ORDER,DATA_RATE_MHZ(1)>(leds, NUM_LEDS).setCorrection(COLOR_CORRECTION);
  FastLED.clear(true);
  FastLED.show();
  FastLED.setBrightness( LED_BRIGHTNESS );
  fill_solid(leds,NUM_LEDS,CRGB::DarkOrange);
  FastLED.show();
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

void setupParameters(){
  //Serial << "Setup ParameterList"<<endl;
  // cleanup the former parameterlist
  vector<metaView*>::iterator iter = EffectParameter._subViews.begin();
  //Serial << "got iter"<<endl;
  vector<metaView*> tmpList;
  //Serial << "got tempList"<<endl;
  while(iter != EffectParameter._subViews.end()){
    //Serial << "get ";
      metaLabel *currentView = (metaLabel*)(*iter++);
    //  Serial << "view ";
      labelList.push_back(currentView);
    //  Serial << "push ";
      tmpList.push_back(currentView);
    //  Serial << " push" ;
    //  Serial << "Remove marked "<<_HEX((unsigned long)currentView)<<endl;
  }
  //Serial << "End of cleanup"<<endl;
  iter = tmpList.begin();
  while(iter != tmpList.end()){
    (*iter++)->removeFromSuperview();
    //Serial << "Removed"<<endl;
  }
  Serial << "End of removal"<<endl;
  // buildUp List
  Effect *p = (*FastLEDAddOns::currentRunningEffect);
  //Serial << "Got effect "<<p->name()<<endl;
  for(size_t k = 0;k<p->numberOfParameters();k++){
    metaLabel *currentView = NULL;
    if(labelList.size()){
      currentView = labelList.back();
      //Serial << "Recycle "<<_HEX((unsigned long)currentView)<<endl;
      labelList.pop_back();
      currentView->setLabel(p->parameterNameAt(k));
      EffectParameter.addSubview(currentView);
      ValueEditor *valEdit = currentView->getAction();
      valEdit->setValue(p->parameterAt(k));
    }else{
    //  Serial << "Add new "<<p->parameterNameAt(k)<<endl;
      currentView = EffectParameter.addEntry(p->parameterNameAt(k));
      ValueEditor * valEdit = new ValueEditor(&ValueView,p->parameterAt(k));
      currentView->setAction(valEdit);
    }
  }
  EffectParameter.layoutList();
  EffectParameter.sizeToFit();
  //Serial << "Finally the final finish"<<endl;
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
    Serial <<"Switch detected"<<endl;
    Effect *p = (*FastLEDAddOns::currentRunningEffect);
    if(p->numberOfParameters()){
      if(!effectParameterLabel->getSuperview()){
        SystemMenu.addSubview(effectParameterLabel);
        SystemMenu.sizeToFit();
        SystemMenu.layoutList();
      }
      setupParameters();
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
  delay(3000);
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
