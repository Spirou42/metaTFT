/**
 The Demo sketch
 This Demo is intended for the teensy3.2 enviroment together with APA102 or WS2811/12 LED Stripes
 The configuration for PINS number of LEDS is Strip etc. are located in the FastLED_Demo.h file
 **/
#include <TFT_UI.h>
#include "Math.h"

#include "SPI.h"
#include "ILI9341_t3.h"

#include "Queue.h"
#include "Streaming.h"
#include "FastLED.h"

#include "Palettes.h"
#include "font_Arial.h"
#include "font_ArialBold.h"
#include "font_GillSans.h"
#include "font_GillSans_SemiBold.h"

//#include "font_Montserrat_Regular.h"
#include "LEDEffects.h"
#include "FastLED_Demo.h"
#include "FastLED_DemoParameterWrapper.h"

using namespace std;
// the LEDs frame buffer and the display instance
CRGB leds[NUM_LEDS+1];
metaTFT tft = metaTFT(TFT_CS, TFT_DC,TFT_RST,TFT_MOSI,TFT_SCK,TFT_MISO,TFT_LED,3);


PaletteList initializeSystemPalettes(){
  PaletteList tmp;
  tmp.push_back(new PalettePair("Rainbow",RainbowColors_p));
  tmp.push_back(new PalettePair("Clouds",CloudColors_p));
  tmp.push_back(new PalettePair("Ocean",OceanColors_p));
  tmp.push_back(new PalettePair("Forest",ForestColors_p));
  tmp.push_back(new PalettePair("Party",PartyColors_p));
  tmp.push_back(new PalettePair("Rainbow Stripes",RainbowStripeColors_p));
  tmp.push_back(new PalettePair("Lava",LavaColors_p));
  tmp.push_back(new PalettePair("Heat",HeatColors_p));
  tmp.push_back(new PalettePair("Arctic",arctic_gp));
  tmp.push_back(new PalettePair("Temperature",temperature_gp));
  tmp.push_back(new PalettePair("Colombia",colombia_gp));
  tmp.push_back(new PalettePair("Cequal",cequal_gp));
  tmp.push_back(new PalettePair("Sunset",Another_Sunset_gp));
  return tmp;
}

EffectList initializeSystemEffects(){
  EffectList tmp;
  tmp.push_back(new EffectPair("Tine",&tinelon));
  tmp.push_back(new EffectPair("Minelon",&minelon));
  tmp.push_back(new EffectPair("Rainbow",rainbow));
  tmp.push_back(new EffectPair("Rainbow Glitter",&rainbowWithGlitter));
  tmp.push_back(new EffectPair("Confetti",&confetti));
  tmp.push_back(new EffectPair("Sinelon",&sinelon));
  tmp.push_back(new EffectPair("Juggle",&juggle));
  tmp.push_back(new EffectPair("BPM",&bpm));
  return tmp;
}

PaletteList systemPalettes = initializeSystemPalettes();
PaletteList::iterator currentSystemPalette = systemPalettes.begin();

EffectList systemEffects = initializeSystemEffects();
EffectList::iterator currentSystemEffect = systemEffects.begin();

/**         Global UI Elements        **/
metaList  SystemMenu;               ///<< The main Menu of the Application
metaList  ParameterMenu;
metaList  EffectsMenu;               ///<< Selection List for Effeects
metaList  PalettesMenu;             ///<< Selection List for Palettes
metaValue ValueView;                ///<< View used for numerical value changes


int16_t tftBrightness = 0;
TFTBrightnessWrapper TFTBrightness(&tftBrightness);
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

int16_t numberOfBlobs   = 3;
ValueWrapper numberOfBlobsWrapper(&numberOfBlobs,1,10,"Blobs");
ValueEditor blobsAction(&ValueView,&numberOfBlobsWrapper);

int16_t fadeOutAmount   = 5;
ValueWrapper fadeAmountWrapper(&fadeOutAmount,1,128,"Fade");
ValueEditor fadeAction(&ValueView,&fadeAmountWrapper);

int16_t blobLength    = 13;
ValueWrapper blobLengthWrapper(&blobLength,1,20,"Length");
ValueEditor lengthAction(&ValueView,&blobLengthWrapper);

int16_t startBlobSpeed = 3; ///< in beats/min
ValueWrapper blobSpeedWrapper(&startBlobSpeed,1,60,"Speed");
ValueEditor speedAction(&ValueView,&blobSpeedWrapper);

ProgramIndexWrapper programIndexWrapper(&systemEffects,&currentSystemEffect);
ValueEditor programAction(&EffectsMenu,&programIndexWrapper);

PaletteIndexWrapper paletteIndexWrapper(&systemPalettes,&currentSystemPalette);
ValueEditor paletteAction(&PalettesMenu,&paletteIndexWrapper);

ValueEditor parameterAction(&ParameterMenu,NULL);


Queue taskQueue;
ActionList initializeActionList(){
  ActionList tmp;
  tmp.push_back(&blobsAction);
  tmp.push_back(&fadeAction);
  tmp.push_back(&lengthAction);
  tmp.push_back(&speedAction);
  return tmp;
}

ActionList actionList = initializeActionList();

metaLabel::LabelLayout*  getListLayout(){
  static metaView::ViewLayout viewLayout;
  static metaLabel::LabelLayout labelLayout;
  static bool isInitialized = false;
  if(!isInitialized){
    viewLayout.backgroundColor=ILI9341_BLACK;
    viewLayout.outlineColor=ILI9341_ORANGE;
    viewLayout.opaque=false;
    labelLayout.viewLayout = &viewLayout;
    labelLayout.font = &Arial_14;
    labelLayout.insets=GCSize(5,5);
    labelLayout.indicatorSpace = 14;
    labelLayout.indicatorSize = GCSize(6,6);
    labelLayout.textSize=1;
    labelLayout.textColor=ILI9341_GREEN;
    viewLayout.visualizeState=false;
    isInitialized=true;
  }
  return &labelLayout;
}

metaValue::ValueLayout getValueLayout(){
  static metaValue::ValueLayout valueLayout;
  static bool isInitialized = false;
  if(!isInitialized){
    Serial << "getValueLayout" <<endl;
    valueLayout.labelFont = &Arial_16;
    valueLayout.valueFont = &Arial_40;
    valueLayout.labelOutlineCornerRadius = 5;
    valueLayout.labelOutlineInset = 3;
    valueLayout.labelDrawOutline=false;
    valueLayout.verticalValueInset=0;
    valueLayout.horizontalLabelInset=18;
    valueLayout.horizontalValueInset=0;
    valueLayout.valueColor = ILI9341_DARKGREEN;
    isInitialized = true;
  }
  return valueLayout;
}

void initListVisual(metaList &k){
  k.setBorderInset(GCSize(15,5));
  k.setLabelLayout(getListLayout());
  k.setDrawsOutline(true);
  k.setCornerRadius(5);
  k.setOutlineColor(ILI9341_RED);
  k.setOpaque(false);
  k.setScrollIndicatorInset(10);
  k.setScrollIndicatorWidth(7);
  k.setScrollIndicatorColor(ILI9341_ORANGE/*0xFBE0*/);
}

void initSystemMenu(){
  // visual them definition for a single list entry

  SystemMenu.initView(&tft,GCRect(30,15,tft.width()/2,tft.height()-4));
  initListVisual(SystemMenu);
  SystemMenu.setIsSelectList(false);

  metaLabel* l = SystemMenu.addEntry( String("TFT Brightness"));
  l->setAction(&tftBrightnessAction);

  l = SystemMenu.addEntry(String("LED Brightness"));
  l->setAction(&ledBrightnessAction);

  l=SystemMenu.addEntry( String("Hue Speed"));
  l->setAction(&hueStepAction);

  l=SystemMenu.addEntry(String("Hue Frames"));
  l->setAction(&hueDelayAction);

  l=SystemMenu.addEntry(String("Effect"));
  l->setAction(&programAction);

  l=SystemMenu.addEntry( String("Palette"));
  l->setAction(&paletteAction);

  l = SystemMenu.addEntry(String("Parameter"));
  l->setAction(&parameterAction);

  SystemMenu.sizeToFit();
  SystemMenu.layoutList();

}
void initEffectsMenu(){
  EffectsMenu.initView(&tft,GCRect(30,15,tft.width()/2,tft.height()-4));
  EffectsMenu.setIsSelectList(true);
  initListVisual(EffectsMenu);
  EffectList::iterator iter = systemEffects.begin();
  while(iter != systemEffects.end()){
    EffectsMenu.addEntry((*iter)->first );
    iter ++;
  }
  EffectsMenu.sizeToFit();
  EffectsMenu.layoutList();
}

void initPalettesMenu(){
  PalettesMenu.initView(&tft,GCRect(30,15,tft.width()/2,tft.height()-4));
  PalettesMenu.setIsSelectList(true);
  initListVisual(PalettesMenu);
  PaletteList::iterator iter = systemPalettes.begin();
  while(iter != systemPalettes.end()){
    PalettesMenu.addEntry((*iter)->first);
    iter ++;
  }
  PalettesMenu.sizeToFit();
  PalettesMenu.layoutList();
}

void initParameterMenu(){
  ParameterMenu.initView(&tft,GCRect(30,15,tft.width()/2,tft.height()-4));
  ParameterMenu.setIsSelectList(false);
  initListVisual(ParameterMenu);
  ActionList::iterator iter = actionList.begin();
  while(iter != actionList.end()){
    metaLabel* l =ParameterMenu.addEntry((*iter)->getValue()->getName());
    l->setAction(*iter);
    iter ++;
  }
  ParameterMenu.sizeToFit();
  ParameterMenu.layoutList();
}


void initValueView(){
  Serial <<"initValueView"<<endl;
  ValueView.initValue(&tft,GCRect(130,00,13,8));

  metaValue::ValueLayout k = getValueLayout();

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
  initEffectsMenu();
  initParameterMenu();
  initValueView();
}

void initializeTFT(){
  tft.start();
  TFTBrightness.setValue(13);
}

void initializeLEDs(){
  #if USE_APA102
  FastLED.addLeds<CHIPSET, LED_PIN, CLOCK_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(COLOR_CORRECTION);
  #else
   FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(COLOR_CORRECTION);
  #endif
  FastLED.clear(true);
  FastLED.show();
  FastLED.setBrightness( LED_BRIGHTNESS );
  fill_solid(leds,NUM_LEDS,CRGB::Red);
  FastLED.show();
}

int processLEDEffects(unsigned long now,void* data){
  static int hueDelay = 0;
  EffectPair *l = *currentSystemEffect;
  effectHandler h = l->second;
  h();
  FastLED.show();
  int16_t hFD = hueStepWrapper.frameDelay() + hueFrameDelay*10;
  int16_t hueStep = hueStepWrapper.hueStep();
  if(hueDelay>=hFD) {
    gHue+=hueStep;
    hueDelay = 0;
  }else{
    hueDelay ++;
  }
  return 0;
}

void adjustBrightness()
{
  int8_t uValue = log(256-tft.getLuminance())*10;
  tft.fillScreen(ILI9341_BLACK);

  String labelStr = String("Brightness");
  String valueStr = String("-55 ");
  String blubberStr = String ("-UU ");
  metaValue bla = metaValue();
  metaValue::ValueLayout brightnessTheme;
  brightnessTheme.labelFont = &Arial_16;
  brightnessTheme.valueFont = &Arial_40;
  brightnessTheme.labelOutlineCornerRadius = 10;
  brightnessTheme.labelOutlineInset = 8;
  brightnessTheme.labelDrawOutline=false;
  brightnessTheme.verticalValueInset=20;
  brightnessTheme.horizontalLabelInset=10;
  brightnessTheme.horizontalValueInset=30;
  brightnessTheme.valueColor = ILI9341_DARKGREEN;

  bla.setLayout(brightnessTheme);
  bla.initValue(&tft,GCRect(100,00,13,8), labelStr, valueStr);
  bla.sizeToFit();
  Serial << "Allign Now"<<endl;
  bla.allignInSuperView(HALLIGN_CENTER | VALLIGN_CENTER);
  valueStr.remove(0);
  valueStr+=String()+uValue;
  bla.setValue(valueStr);
  bla.redraw();

  static elapsedMillis lastAdjust = elapsedMillis(0);

  lastAdjust =0;

  do{
    taskQueue.Run(millis());
    if(eventQueue.length()){
      int8_t kValue = uValue;
      UserEvent *evnt = eventQueue.popEvent();
      if(evnt->getType()==EventType::EventTypeButton){
        ButtonData data = evnt->getData().buttonData;
        if(data.id==ButtonID::CenterButton &&
          data.state == ButtonState::ButtonClick){
          break;
        }
        if(data.id == ButtonID::UpButton &&
          (data.state == ButtonState::ButtonClick ||
            data.state == ButtonState::ButtonLongClick)){
          uValue = 55;
          lastAdjust=0;
        }
        if(data.id == ButtonID::DownButton &&
          (data.state == ButtonState::ButtonClick ||
            data.state == ButtonState::ButtonLongClick)){
          uValue = 0;
          lastAdjust=0;
        }

      }else if(evnt->getType() ==EventType::EventTypeEncoder){
        EncoderData data = evnt->getData().encoderData;
        int8_t steps = data.absSteps;
        uValue +=steps;
        lastAdjust=0;
      }
      if(uValue > 55){
        uValue = 55;
      }else if(uValue <0){
        uValue = 0;
      }
      if(uValue != kValue){
        uint8_t k= exp(uValue/10.0);
        k = 256 -k;
        tft.setLuminance(k);
        valueStr.remove(0);
        valueStr += String(uValue);
        Serial << "ValueStr: "<<valueStr<<endl;
        bla.setValue(valueStr);
        bla.valueUpdate();
        bla.redraw();
      //  tft.setCursor(20,20);
      //  tft.fillRect(20,80,16*6*tft.getTextSize(),7*tft.getTextSize(),ILI9341_BLUE);
      //  tft << "Brightness: "<<tft.getLuminance();
        //Serial <<"Brightness: "<<tft.getLuminance()<<"    "<<endl;
      }
    }
  }while(true/*lastg<5000*/);
}



void setup() {
  Serial.begin(115200);
  #if DEBUG_STARTUP
  while(!Serial){}
  #endif
  Serial << "Start"<<endl;
  Serial << "Effects: "<<systemEffects.size()<<endl;
  Serial << "Palettes: "<<systemPalettes.size()<<endl;
  initializeLEDs();

  // init LED Backlight
  pinMode(TFT_LED,OUTPUT);
  digitalWriteFast(TFT_LED,1);

  Serial << "Init TFT"<<endl;
  initializeTFT();

  // enable UI
  enableSwitches();
  enableEncoders();

  // draw mask
  initUI();
  programIndexWrapper.setValue(2);
  paletteIndexWrapper.setValue(0);
  // initialize tasks
  taskQueue.scheduleFunction(processLEDEffects,NULL,"EFFC",0,1000/FRAMES_PER_SECOND);
  taskQueue.scheduleFunction(processUserEvents,NULL,"USER",0,100);
}


elapsedMillis markerTime = 0;
void loop() {
  static bool skipMask = false;
  bool p = false;
  if(markerTime > 1000){
    markerTime = 0;
    p=true;
//    Serial << "k"<<endl;
  }
  if(!skipMask){
    tft.fillScreen(ILI9341_BLACK);
    responderStack.push_back(&SystemMenu);
    responderStack.back()->redraw();
    Serial << "Draw"<<endl;
    Serial.flush();
    skipMask = true;
  }
  if(responderStack.empty()){ // this is not good
    responderStack.push_back(&SystemMenu);
    SystemMenu.prepareForDisplay();
    SystemMenu.redraw();
  }



  /** run all sequence tasks */
  taskQueue.Run(millis());
  if(p){
    p = false;
//    Serial << "."<<endl;
  }
}
