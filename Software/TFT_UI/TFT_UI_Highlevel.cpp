/**
TFT_UI_Highlevel.cpp
**/
// a couple of global parameters

#include <TFT_UI_Highlevel.h>
TFTUI_NAMESPACE_BEGIN
metaLabel::LabelLayout*  defaultListLabelLayout(){
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

metaValue::ValueLayout defaultValueLayout(){
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

void initDefaultListVisual(metaList &k){
  k.setBorderInset(GCSize(15,5));
  k.setLabelLayout(defaultListLabelLayout());
  k.setDrawsOutline(true);
  k.setCornerRadius(5);
  k.setOutlineColor(ILI9341_RED);
  k.setOpaque(false);
  k.setScrollIndicatorInset(10);
  k.setScrollIndicatorWidth(7);
  k.setScrollIndicatorColor(ILI9341_ORANGE/*0xFBE0*/);
}
TFTUI_NAMESPACE_END
