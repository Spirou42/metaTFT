/**
* implementation of the views
*/


#include "Arduino.h"
#include "UI_Views.hpp"
#include "UI_UserEvent.hpp"

#define DEBUG_LAYOUT_COLOR_BACKGROUND_INNER ILI9341_DARKGREY
#define DEBUG_LAYOUT_COLOR_BACKGROUND_MIDDLE ILI9341_LIGHTGREY
#define DEBUG_LAYOUT_COLOR_BACKGROUND_OUTER ILI9341_NAVY
#define DEBUG_LAYOUT_LINECOLOR ILI9341_WHITE

/**********************************************
 *									metaView									*
 **********************************************/

void metaView::initView(TFTDisplay* tft, GCRect frame){
	initGraphicsContext(tft);
	_frame = frame;
	_needsRedraw = true;
	_needsLayout = true;
	_respondsToEvents = 0;
	_visible = true;
}

GCPoint metaView::getScreenOrigin(){
  GCPoint result;
  result += getOrigin();
  metaView *superView = getSuperview();
  while(superView){
    result += superView->getOrigin();
    superView = superView->getSuperview();
  }
  return result;
}

void metaView::initView(TFTDisplay* tft, GCPoint origin, GCSize size){
	initView(tft,GCRect(origin,size));
}

void metaView::initView(TFTDisplay* tft, int16_t x, int16_t y, int16_t w, int16_t h){
	initView(tft,GCRect(x,y,w,h));
}

void metaView::setLayout(ViewLayout l){
	_outlineColor = l.outlineColor;
	_backgroundColor = l.backgroundColor;
	_cornerRadius = l.cornerRadius;
	_opaque = l.opaque;
	_drawsOutline = l.drawsOutline;
	_visualizeState = l.visualizeState;
	_state = l.state;
	setNeedsRedraw();
}

const vector<metaView*>::iterator metaView::findSubview(metaView* subView){
	vector<metaView*>::iterator start = _subViews.begin();
	while(start != _subViews.end()){
		if(*start == subView){
			return start;
		}
		start ++;
	}
	return start;
}

void metaView::removeFromSuperview(){
	if(_superView){
		_superView->removeSubview(this);
	}
}

void metaView::removeSubview(metaView* subView){
	const vector<metaView*>::iterator subIter = findSubview(subView);
	if(_subViews.end() != subIter){
		_subViews.erase(subIter);
		subView->_superView = NULL;
	}
}

void metaView::addSubview(metaView* ptr){
	_subViews.push_back(ptr);
	ptr->_superView = this;
}

void metaView::sizeToFit(){
	return;
}

bool metaView::childNeedsLayout(){
	vector<metaView*>::iterator iter = _subViews.begin();
	while(iter != _subViews.end()){
		if((*iter++)->_needsLayout){
			return true;
		}
	}
	return false;
}

void metaView::allignInRect(uint8_t allignmentMask, GCRect rect){
	if(!allignmentMask)
		return;
	GCPoint origin = _frame.origin;
	if(allignmentMask & HALLIGN_LEFT){
		origin.x = rect.origin.x;
	}else if(allignmentMask & HALLIGN_RIGHT){
		origin.x = (rect.size.w - _frame.size.w) +rect.origin.x;
	}else if(allignmentMask & HALLIGN_CENTER){
		origin.x = (rect.size.w/2 - _frame.size.w/2) + rect.origin.x;
	}

	if(allignmentMask & VALLIGN_TOP){
		origin.y = rect.origin.y;
	}else if(allignmentMask & VALLIGN_BOTTOM){
		origin.y = (rect.size.h - _frame.size.h) + rect.origin.y;
	}else if(allignmentMask & VALLIGN_CENTER){
		origin.y = (rect.size.h/2 - _frame.size.h/2)+rect.origin.y;
	}
	setOrigin(origin);
	setNeedsLayout();

}

void metaView::allignInSuperView(uint8_t allignmentMask){
	if((allignmentMask == 0))
		return;
	GCRect p;
	if(_superView){
		p.size = _superView->getSize();
	}else{
		p.size = GraphicsContext::displaySize();
		//Serial << "Allign: "<<p<<endl;
	}
	allignInRect(allignmentMask, p);
}

void metaView::redrawChildren(bool forceRedraw){
	if(_subViews.size() ==0)
		return;
		//	Serial << ">>>>>>>redrawChildren "<< forceRedraw<<endl;
	vector<metaView*>::iterator redrawIter = _subViews.begin();
	while(redrawIter!=_subViews.end()){
		//Serial << "RedrawChild: "<<endl;
		if(forceRedraw){
			(*redrawIter)->_needsRedraw = true;
			//			Serial << "Force:"<<endl;
		}
		if((*redrawIter)->_needsRedraw && (*redrawIter)->_visible)
			(*redrawIter)->redraw();
		(*redrawIter)->resetFlags();
		redrawIter++;
	}
	//	Serial << "<<<<<<<redrawChildren"<<endl<<endl;;
}

#if DEBUG_LAYOUT
void metaView::drawDebug(){
	#if DEBUG_LAYOUT
	if (drawDebugRect){
		GCRect dr = debugRect;
		//		dr.origin += _frame.origin;
		GraphicsContext::setStrokeColor(DEBUG_LAYOUT_LINECOLOR);
		GraphicsContext::drawRect(dr);
		Serial << "DebugDraw: "<<debugRect<<endl;
	}
	#endif
}
#endif
void metaView::drawOutline()
{
	GraphicsContext::setStrokeColor(_outlineColor);
	if(_cornerRadius==0){
		GraphicsContext::drawRect(getBounds());
	}else{
		GraphicsContext::drawRoundRect(getBounds(),_cornerRadius);
	}
}
void metaView::redraw(){
	if(!this->_visible)
    return;
	vector<metaView*>::iterator redrawIter = _subViews.begin();
	bool cnl = childNeedsLayout();
	if(_needsRedraw || cnl){
		GCRect p = getBounds();
		p.origin += getScreenOrigin();
			//Serial << "Redraw: "<<_needsRedraw<<" "<<cnl<<"("<<p<<")\t"<<_HEX((unsigned long)this)<<endl;
		GraphicsContext::setFillColor(_backgroundColor);
  	if(_cornerRadius==0){
    	GraphicsContext::fillRect(getBounds());
  	}else{
    	GraphicsContext::fillRoundRect(getBounds(),_cornerRadius);
  	}
  	if(_drawsOutline){
			this->drawOutline();
  	}
		redrawChildren(true);
		resetFlags();
	}else{
		redrawChildren();
	}
	#if DEBUG_LAYOUT
	drawDebug();
	#endif
}

void metaView::prepareForDisplay(){_needsRedraw=true;}

void metaView::removeFromScreen(){

	setFillColor(_backgroundColor);
	fillRect(GCPoint(0,0),_frame.size);
	//return;
  Serial << "Frame: "<< _frame<<endl;
	if(this->_responderStack){
		ResponderStack::iterator iter = this->_responderStack->begin();
    //GraphicsContext::setClipRect(GCRect(0,0,_frame.size.w,_frame.size.h));
		while(iter != this->_responderStack->end()-1){
			metaView *v = *iter;
			Serial << "Got: "<<_HEX((long int)v)<< endl;
      v->setNeedsRedraw();
			v->redraw();
			++iter;
		}
    //GraphicsContext::clearClipRect();
    Serial << "Redraw finished"<<endl;
	}else{
		Serial << "Nothing to redraw"<< endl;
	}
}
/**********************************************
 *									metaLabel 								*
 **********************************************/

void metaLabel::sizeToFit(){
	 GCSize is = intrinsicSize();
	 setSize(is); setNeedsLayout();
}

void metaLabel::setLabel(const String &label){
	_label.remove(0);
	_label.append(label);
	_needsRedraw=true;
}

GCSize metaLabel::intrinsicSize(){
	GCSize s=GCSize();
	GraphicsContext::setFont(_font);
	GraphicsContext::setTextSize(_textSize);
	s = GraphicsContext::stringSize(_label.c_str());
	if(_visualizeState){
		s.w +=_indicatorSpace;
	}
	s.w+=2*_insets.w;
	s.h+=2*_insets.h;
	return s;
}

void metaLabel::setLayout(LabelLayout ll){
	if(NULL != ll.viewLayout){
		metaView::setLayout(*ll.viewLayout);
	}
	setFont(ll.font);
	setTextSize(ll.textSize);
	setAllignmentMask(ll.allignmentMask);
	setTextPosition(ll.textPosition);
	setInsets(ll.insets);
	setIndicatorSize(ll.indicatorSize);
	setIndicatorSpace(ll.indicatorSpace);
}

void metaLabel::redraw(){
	//Serial << ">>>>>>Label Redraw "<<_frame<<_font->line_space<<endl;
	//		Serial << "Label: "<<(this->getLabel())<<" visible: "<<(this->_visible?"YES":"NO")<<endl;
	if(!this->_visible){
		return;
	}
  metaView::redraw();
	GCPoint tp=_textPosition;
	tp.x+=_insets.w;
	tp.y+=_insets.h;
	int16_t xOffset = 0;
	if(_visualizeState){
		xOffset = _indicatorSpace;
		tp.x += xOffset;
	}
	if(_allignmentMask){
		GCSize is = intrinsicSize();
		if((_allignmentMask & HALLIGN_LEFT))
			{
				tp.x += 0/*+xOffset*/;
			}
		if((_allignmentMask & HALLIGN_RIGHT))
			{
				tp.x += _frame.size.w - is.w+xOffset;
			}
		if((_allignmentMask & HALLIGN_CENTER))
			{
				tp.x += _frame.size.w/2 - is.w/2 + xOffset;
			}
		if((_allignmentMask & VALLIGN_TOP))
			{
				tp.y = 0;
			}
		if((_allignmentMask & VALLIGN_BOTTOM))
			{
				tp.y += _frame.size.h - is.h;
			}
		if((_allignmentMask & VALLIGN_CENTER))
			{
				tp.y += _frame.size.h/2 - is.h/2;
			}
	}
	if(_visualizeState){
		uint16_t bgColor = ILI9341_BLACK;
		uint16_t olColor = ILI9341_DARKGREY;
		switch(_state){
			case Off: bgColor = ILI9341_BLACK; break;
			case On: bgColor = _outlineColor; break;
			case Mixed: bgColor = ILI9341_NAVY; break;
		}
		GraphicsContext::setFillColor(bgColor);
		GraphicsContext::setStrokeColor(olColor);
		GCRect indicator;
		indicator.origin=GCPoint(0,   (_frame.size.h-2*_insets.h)/2 -_indicatorSize.h/2 -(_font->line_space-_font->cap_height)/2);
		indicator.origin.x += _insets.w;
		indicator.origin.y += _insets.h;
		//indicator.origin = _frame.origin;
		indicator.size =_indicatorSize;
		if(_state != Off){
			GraphicsContext::fillRect(indicator);
		}
		drawRect(indicator);
		//drawRect(_insets.w,_insets.h,_indicatorSpace,_frame.size.h-2*_insets.h);

	}
	//	Serial << *_label<<" TP: "<<tp<<endl;
	GraphicsContext::setCursor(tp);
	GraphicsContext::setFont(_font);
	//Serial << "[Label] ("<<*_label<<") "<<(int)_font<<","<<_textColor<<","<<_backgroundColor<<endl;
	if(_opaque){
		GraphicsContext::setTextColor(_textColor,_backgroundColor);
		//		Serial << "opaque"<<endl;
	}else{
		GraphicsContext::setTextColor(_textColor);
	}
	GraphicsContext::setTextSize(_textSize);
	(*this) << _label<<endl;
	resetFlags();
	//Serial << ">>>>>>>Label Redraw "<<_label<<" done"<<endl;
}

/**********************************************
 *									metaValue 								*
 **********************************************/

void metaValue::initValue(TFTDisplay* tft, GCRect frame){
	metaView::initView(tft, frame);
	setBackgroundColor(ILI9341_BLACK);
	#if DEBUG_LAYOUT_VALUE
	setBackgroundColor(DEBUG_LAYOUT_COLOR_BACKGROUND_MIDDLE);
	setOpaque(true);
	#endif
	setOutlineColor(ILI9341_YELLOW);
	setCornerRadius(_cornerRadius);
	setDrawsOutline(false);


	_labelView.initView(tft,0,0,100,100);

	_labelView.setTextSize(2);
	_labelView.setBackgroundColor(_backgroundColor);
	_labelView.setVisualizeState(false);
	#if DEBUG_LAYOUT_VALUE
	_labelView.setBackgroundColor(DEBUG_LAYOUT_COLOR_BACKGROUND_INNER);
	#endif
	_labelView.setAllignmentMask(VALLIGN_CENTER | HALLIGN_CENTER);
	_labelView.sizeToFit();

	_valueView.initView(tft,0,0,120,120);
	_valueView.setTextSize(3);
	_valueView.setBackgroundColor(_backgroundColor);
	#if DEBUG_LAYOUT_VALUE
	_valueView.setBackgroundColor(DEBUG_LAYOUT_COLOR_BACKGROUND_INNER);
	#endif

	_valueView.sizeToFit();
	_valueView.setAllignmentMask(VALLIGN_CENTER | HALLIGN_CENTER);
	_valueView.setTextPosition(1,1);
	_valueView.setNeedsRedraw();
	_lastValueRect=_valueView._frame;
	//Serial << "ValuePos "<<valOr.x<<", "<<valOr.y<<"("<<s.w<<","<<s.h<<")"<<endl;
	//	value->remove(0);

	addSubview(&_labelView);
	//	Serial << "Label Added"<<endl;
	addSubview(&_valueView);
		//Serial << "Value Added"<<endl;

}

void metaValue::initValue(TFTDisplay* tft, GCRect frame, String label, String value){
	_labelView.setLabel(label);
	_valueView.setLabel(value);
	initValue(tft,frame);
}

void metaValue::setValue(String label){
	#if DEBUG_VALUE_VALUE
	Serial << "[mV] setValue: "<<label<<endl;
	#endif
	bool relayout = false;
	if(_valueView.getLabel().length() != label.length()){
		#if DEBUG_VALUE_VALUE
		Serial << "[mV] Value Changed Length"<<endl;
		#endif
		relayout = true;
	}
	_valueView.setLabel(label);
	GCSize ns = resizeValue(true);
	GCSize cs = _valueView.getSize();
	if((ns.w>cs.w) || (ns.h>cs.h)){
		#if DEBUG_VALUE_VALUE
		Serial << "[mV] Value changed Size"<<endl;
		#endif
		relayout = true;
	}

	if(relayout){
		_valueView.setSize(ns);
		GCRect p = valueLayoutRect();
		#if DEBUG_LAYOUT_VALUESIZE
		this->debugRect =_valueView._frame;
		this->drawDebugRect = true;
		#endif

		_valueView.allignInRect(HALLIGN_CENTER | VALLIGN_CENTER,p);
		//_lastValueRect = _valueView._frame;
	}
	#if DEBUG_VALUE_VALUE
	Serial<<"newValue "<<_valueView.getLabel()<<endl;
	#endif
}

void metaValue::redraw(){
	#if DEBUG_VALUE_REDRAW
	bool cnl= childNeedsLayout();
	Serial << "[mV] redraw "<<_needsRedraw<<", "<<cnl<<endl;
	#endif
	if(_needsRedraw /*|| cnl*/){
		GCRect p=getBounds();
		p.origin.y+=_frameInset;
		p.size.h-=_frameInset;
		#if DEBUG_VALUE_REDRAW
		Serial << "TopBorderOffset: "<<_frameInset<<endl;
		Serial << "Rect: "<<p<<endl;
		Serial<< "Base: "<<_base<<endl;
		#endif
		#if DEBUG_LAYOUT_VALUEBACKGROUND
		GraphicsContext::setStrokeColor(DEBUG_LAYOUT_COLOR_BACKGROUND_OUTER);
		GraphicsContext::drawRoundRect(getBounds(),_cornerRadius);
		#endif
		GraphicsContext::setFillColor(_backgroundColor);
		GraphicsContext::setStrokeColor(_outlineColor);

		GraphicsContext::fillRoundRect(p,_cornerRadius);
		GraphicsContext::drawRoundRect(p,_cornerRadius);
		resetFlags();
		redrawChildren(true);
		_lastValueRect = _valueView._frame;
	}else{
		if(_valueView._needsRedraw){
			setFillColor(_backgroundColor);
			fillRect(_lastValueRect);
			_valueView.redraw();
			_lastValueRect=_valueView._frame;
		}
		if(_labelView._needsRedraw){
			_labelView.redraw();
		}
		//redrawChildren();
	}
	#if DEBUG_LAYOUT
	drawDebug();
	#endif
}

void metaValue::prepareForDisplay(){
	setSize(10,10);
	sizeToFit();
	allignInSuperView(HALLIGN_CENTER | VALLIGN_CENTER);
	// remember the
}
GCSize metaValue::resizeLabel(){
	_labelView.sizeToFit();
	GCSize labelSize = _labelView.getSize();

	labelSize.h = _labelView.getFont()->line_space;
	if(_labelView.getDrawsOutline()){
		labelSize.h+=2*_labelOutlineInset;
	}
	labelSize.w+=4*_labelOutlineInset;
	_labelView.setSize(labelSize);
	return labelSize;
}

GCSize metaValue::resizeValue(bool dontResize){
	_valueView.sizeToFit();
	GCSize valueSize = _valueView.getSize();
	valueSize+=4;
	if(!dontResize){
		_valueView.setSize(valueSize);
	}
	return valueSize;
}

GCRect metaValue::valueLayoutRect(GCSize fSize){
	GCRect p(GCPoint(0,0),fSize);
	p.size.h=(p.size.h - (2*_frameInset))-(2*_verticalValueInset);
	p.size.w=p.size.w - (2*_horizontalValueInset);
	p.origin.x = _horizontalValueInset;
	p.origin.y = 2*_frameInset+_verticalValueInset -calcBaselineCorrection();
	return p;
}

GCRect metaValue::valueLayoutRect(){
	return valueLayoutRect(_frame.size);
}

int16_t metaValue::calcBaselineCorrection(){
	int16_t baslineCorrection = (_labelView.getFont()->line_space - _labelView.getFont()->cap_height-3);
	return baslineCorrection;
}

void metaValue::sizeToFit(){

	GCSize labelSize = resizeLabel();
	GCSize valueSize = resizeValue();
	_frameInset =   labelSize.h/2 ;
	#if DEBUG_LAYOUT_VALUE
	Serial << "valueSize: "<<valueSize<<endl;
	#endif
	#if DEBUG_LAYOUT_VALUE
	int16_t baslineCorrection = calcBaselineCorrection();
	//	baslineCorrection = 0;

	Serial << "labelSize: "<<labelSize;
	Serial << "baseLine Correction "<<baslineCorrection<<endl;
	#endif
	GCSize ownSize = _frame.size;
	#if DEBUG_LAYOUT_VALUE
	Serial << "ownSize: "<<ownSize<<endl;
	#endif
	valueSize.h+=labelSize.h+(2*_verticalValueInset)+(2*_frameInset);//baslineCorrection;
	valueSize.w+=2*_horizontalValueInset;
	//labelSize.h+=2*VALUE_VERTICAL_INSET;
	labelSize.w+=2*_horizontalLabelInset;

	ownSize.w = MAX(ownSize.w,MAX(valueSize.w,labelSize.w));
	ownSize.h = MAX(ownSize.h,valueSize.h);
	// if(( (float)ownSize.w/(float)ownSize.h)>1.8){
	// 	ownSize.h = ownSize.w/1.6180;
	// }
	#if DEBUG_LAYOUT_VALUE
	Serial << "resultSize: "<<ownSize<<endl;
	#endif
	{
		GCPoint lo = GCPoint();
		lo.x = _horizontalLabelInset;
		//lo.y = 20;
		_labelView.setOrigin(lo);
	}
	/*{
		GCPoint valOr = GCPoint();
		valOr.x =((ownSize.w-2*_horizontalValueInset)/2 - valueSize.w/2)+_horizontalValueInset;
		valOr.y = (ownSize.h/2 - valueSize.h/2)+_frameInset/2;
		_valueView.setOrigin(valOr);
	}*/

	// relayout the Value<
	GCRect p = valueLayoutRect(ownSize);
	#if DEBUG_LAYOUT_VALUE
	Serial << "layoutRect: "<<p<<endl;
	#endif
	#if DEBUG_LAYOUT_VALUESIZE
	this->debugRect =p;
	this->drawDebugRect = true;
	#endif
	_valueView.allignInRect(HALLIGN_CENTER | VALLIGN_CENTER,p);

	setSize(ownSize);
}

void metaValue::setLayout(ValueLayout definition){
	setLabelFont(definition.labelFont);
	setValueFont(definition.valueFont);
	setLabelColor(definition.labelColor);
	setValueColor(definition.valueColor);
	setDrawLabelOutline(definition.labelDrawOutline);
	setLabelOutlineInset(definition.labelOutlineInset);
	setLabelOulineCornerRadius(definition.labelOutlineCornerRadius);
	setCornerRadius(definition.cornerRadius);
	setHorizontalLabelInset(definition.horizontalLabelInset);
	setHorizontalValueInset(definition.horizontalValueInset);
	setVerticalValueInset(definition.verticalValueInset);
}

uint16_t metaValue::respondsToEvents(){
	if(_processEvents){
		uint16_t result = EventMask::ButtonEvents | EventMask::EncoderEvents |			// we want button and encoder events
			EventMask::ButtonEvent_AllButtons |																				// we want to know about all Buttons
			EventMask::ButtonState_Up | EventMask::ButtonState_Down; 									// and if they are UP or Down
		return result;
	}
	return 0;
}

int16_t metaValue::processEvent(UserEvent *evnt){
	int16_t result = ResponderResult::ChangedNothing;
	#if DEBUG_VALUE_EVENTS
	Serial << "metaValue::processEvent "<<evnt<<endl;
	#endif
	if(!_ValueWrapper){
		return ResponderResult::ChangedNothing;
	}else{
		int16_t val = getNumericValue();
		#if DEBUG_VALUE_EVENTS
		Serial <<"we got a Value named "<<_ValueWrapper->getName()<<" ("<<val<<")"<<endl;
		#endif
		switch(evnt->getType()){
			case EventType::EventTypeEncoder: {
				int8_t steps = evnt->getAbsEncoderSteps();
				float k = evnt->getEncoderSpeed();
				#if DEBUG_VALUE_EVENTS
				Serial << "Encoder with "<< steps<<" with Speed: "<<k<<" "<<log(k*10)<<endl;
				#endif
				if(k>10){
					float d = (_ValueWrapper->getMaxValue()-_ValueWrapper->getMinValue())/25.0;
					if (d==0.0){
						d=0.001;
					}
					float p =log(k*(exp(d)/4));
					#if 1
					Serial << "d "<<d<<", "<<p<<endl;
					#endif
					steps *=p;
				}
				val += steps;
				#if DEBUG_VALUE_EVENTS
				Serial << "Setting: "<<val<<endl;
				#endif
				setNumericValue(val);
				val = getNumericValue();
				#if DEBUG_VALUE_EVENTS
				Serial << "and got "<<val<<endl;
				#endif
				setValue(String(val));
				//_valueView.setNeedsRedraw();
				result = ResponderResult::ChangedVisual;
			}
			break;
			case EventType::EventTypeButton:{
				switch(evnt->getButtonID()){
					case ButtonID::UpButton: break;
					case ButtonID::DownButton: break;
					case ButtonID::RightButton: break;

					case ButtonID::LeftButton:
					case ButtonID::CenterButton:
						if(evnt->getButtonState() == ButtonState::ButtonUp){
							result = ResponderResult::ResponderExit;
						}
						break;
					default: break;
				}
			}
			break;
		}
	}

	return result;
}
/**********************************************
 *									metaList 									*
 **********************************************/

void metaList::initView(TFTDisplay* tft, GCRect frame ){
	metaView::initView(tft,frame);
	this->_maxVisibleEntries = 6;
	this->_visibleStart = 0;
}
uint16_t metaList::respondsToEvents(){
	uint16_t result = EventMask::EncoderEvents | EventMask::ButtonEvents |
										EventMask::ButtonEvent_AllButtons |
										EventMask::ButtonState_All;
	return result;
}
void metaList::addSubview(metaView* aView){
	metaView::addSubview(aView);
	aView->sizeToFit();
	//GCSize l = aView->getSize();
	//Serial << " View: "<<l<<endl;
	_maxElementSize.w = MAX(aView->getSize().w,_maxElementSize.w);
	_maxElementSize.h = MAX(aView->getSize().h,_maxElementSize.h);
}

metaLabel* metaList::addEntry(const String b){
	metaLabel *k = new metaLabel(b);

	k->initView(_display,GCRect());
	if(_ll){
		k->setLayout(*_ll);
	}
	if(_isSelectList){
		k->setVisualizeState(true);
	}else{
		k->setVisualizeState(false);
	}
	addSubview(k);
	return k;
	//Serial << "added: "<<_HEX((long int)k)<<" "<<_subViews.size()<<endl;
}

void metaList::sizeToFit(){
	GCSize resultSize ;
	if(!_subViews.size()){
		return;
	}
	int p = _subViews.size();
	if(p>_maxVisibleEntries){
		p=_maxVisibleEntries;
	}
	GCSize tS = _maxElementSize;
	tS.h=_maxElementSize.h*p;
	if(p>1){
		tS.h+=_cellInset.h*(p-1);
	}
	tS+=_borderInset*2;

	resultSize += tS;
	//Serial << "Size: "<< resultSize<<endl;
	setSize(tS);
}
void metaList::scrollList(){
	GCPoint currentLine;
	currentLine.x = _borderInset.w;
	currentLine.y = _borderInset.h;
	vector<metaView*>::iterator subIter = _subViews.begin();

	while(subIter != _subViews.end()){
		uint8_t currentEntry = subIter - _subViews.begin();

		if ( 	(currentEntry >= this->_visibleStart) &&
					(currentEntry <  (this->_visibleStart + this->_maxVisibleEntries))){
			(*subIter)->_visible = true;
			(*subIter)->setSize(_maxElementSize);
			(*subIter)->setOrigin(currentLine);
			currentLine.y+=_maxElementSize.h+_cellInset.h;
		}else{
			(*subIter)->_visible = false;
		}
		subIter ++;
	}
}


// this method is called to intal layout the list,
// determine the current visibility state of the list entries and position
void metaList::layoutList(){
	GCPoint currentLine;
	currentLine.x = _borderInset.w;
	currentLine.y = _borderInset.h;
	vector<metaView*>::iterator subIter = _subViews.begin();

	while(subIter != _subViews.end()){
		uint8_t currentEntry = subIter - _subViews.begin();
		// set initial visibility
		if ( 	(currentEntry >= this->_visibleStart) &&
					(currentEntry <  (this->_visibleStart + this->_maxVisibleEntries))){
			(*subIter)->_visible = true;
		}else{
			(*subIter)->_visible = false;
		}
		(*subIter)->setSize(_maxElementSize);
		(*subIter)->setOrigin(currentLine);
		if(subIter != _subViews.begin()){
			(*subIter)->setDrawsOutline(false);
			(*subIter)->setState(metaView::State::Off);
		}else{
			(*subIter)->setState(metaView::State::Off);
			(*subIter)->setDrawsOutline(true);
			_lastSelectedView = NULL;
		}
		currentLine.y+=_maxElementSize.h+_cellInset.h;

//		Serial << "Entry: "<<" visible:"<< ((*subIter)->_visible ? "YES":"NO")<<endl;
		//(*subIter)->setVisualizeState(true);

		subIter++;
	}

}

metaView* metaList::selectedSubview(){
	vector<metaView*>::iterator subIter = _subViews.begin();
	while(subIter!=_subViews.end()){
		if((*subIter)->getDrawsOutline()){
			return *subIter;
		}
		subIter++;
	}
	return NULL;
}

void metaList::drawConnectionFor(metaView* v, uint16_t lineColor){
	if(v){
		GCPoint vo = v->getOrigin();
		GCSize  vs = v->getSize();

		vo.x+=vs.w;
		vo.y+=vs.h/2-1;
		GraphicsContext::setStrokeColor(lineColor);
		GraphicsContext::drawLine(vo,GCPoint(_frame.size.w-2,vo.y));

		vo.y++;
		GraphicsContext::setStrokeColor(_backgroundColor);
		GraphicsContext::drawLine(vo,GCPoint(_frame.size.w-2,vo.y));

		vo.y++;
		GraphicsContext::setStrokeColor(lineColor);
		GraphicsContext::drawLine(vo,GCPoint(_frame.size.w-2,vo.y));
	}
}
void metaList::drawScrollIndicator(){
	// draw an indicator
	#if DEBUG_LIST_REDRAW
	Serial << "Indicator needed in list"<<endl;
	#endif
	int16_t indicatorInset = _scrollIndicatorInset+_cornerRadius;
	int16_t indicatorTrackHeight = _frame.size.h- 2*indicatorInset;
	GCPoint indicatorOrigin = GCPoint(_frame.size.w-1-(_scrollIndicatorWidth/2),indicatorInset);
	GCRect indicatorTrack = GCRect(indicatorOrigin,GCSize(_scrollIndicatorWidth,indicatorTrackHeight) );
	setFillColor(_backgroundColor);
	fillRect(indicatorTrack);
	setStrokeColor(_outlineColor);
	drawLine(indicatorTrack.origin.x,indicatorTrack.origin.y,indicatorTrack.origin.x-1+_scrollIndicatorWidth,indicatorTrack.origin.y);
	drawLine(indicatorTrack.origin.x,_frame.size.h-indicatorTrack.origin.y,indicatorTrack.origin.x-1+_scrollIndicatorWidth,_frame.size.h-indicatorTrack.origin.y);

	indicatorTrackHeight -=2;// remove the horizontal borders from the track
	indicatorOrigin.y+=1;
	float heightForEntry = indicatorTrackHeight / float(_subViews.size());
	//int16_t fractHeightForEntry = indicatorTrackHeight % (_subViews.size());
	int16_t heightForWindow = heightForEntry * _maxVisibleEntries;// + fractHeightForEntry*_maxVisibleEntries;
	indicatorOrigin.y+= _visibleStart*(heightForEntry);// + fractHeightForEntry);
	GCRect indicatorRect = GCRect(indicatorOrigin,GCSize(_scrollIndicatorWidth,heightForWindow));
	setFillColor(_scrollIndicatorColor);
	fillRoundRect(indicatorRect,_scrollIndicatorWidth/2);
	drawRect(indicatorTrack);
	#if DEBUG_LIST_REDRAW
	Serial << "HFE:"<<heightForEntry<<" HFW:"<<heightForWindow<<" ITH:"<<indicatorTrackHeight<<" IIL:"<<_subViews.size()<<" MVE:"<<_maxVisibleEntries<<endl;
	#endif
}

void metaList::drawOutline()
{
#if 0
	metaView::drawOutline();
#else
	GraphicsContext::setStrokeColor(_outlineColor);
	strokeRoundOutline();
#endif
}

// Draw a rounded rectangle
void metaList::strokeRoundOutline() {
  GCRect rect = getBounds();
  rect.origin += getScreenOrigin();
  // smarter version
  _display->drawFastHLine(rect.origin.x+_cornerRadius, rect.origin.y              , rect.size.w-2*_cornerRadius, _strokeColor); // Top
  _display->drawFastHLine(rect.origin.x+_cornerRadius, rect.origin.y+rect.size.h-1, rect.size.w-2*_cornerRadius, _strokeColor); // Bottom
  _display->drawFastVLine(rect.origin.x              , rect.origin.y+_cornerRadius, rect.size.h-2*_cornerRadius, _strokeColor); // Left
	if(isScrollList()){
		_display->drawFastVLine(rect.origin.x+rect.size.w-1,rect.size.h-2*_cornerRadius+_scrollIndicatorInset,_scrollIndicatorInset,_strokeColor);
		_display->drawFastVLine(rect.origin.x+rect.size.w-1, rect.origin.y+_cornerRadius, _scrollIndicatorInset,_strokeColor);
	}else{
  	_display->drawFastVLine(rect.origin.x+rect.size.w-1, rect.origin.y+_cornerRadius, rect.size.h-2*_cornerRadius, _strokeColor); // Right
	}
  // draw four corners
  _display->drawCircleHelper(rect.origin.x+_cornerRadius              , rect.origin.y+_cornerRadius              , _cornerRadius, 1, _strokeColor);
  _display->drawCircleHelper(rect.origin.x+rect.size.w-_cornerRadius-1, rect.origin.y+_cornerRadius              , _cornerRadius, 2, _strokeColor);
  _display->drawCircleHelper(rect.origin.x+rect.size.w-_cornerRadius-1, rect.origin.y+rect.size.h-_cornerRadius-1, _cornerRadius, 4, _strokeColor);
  _display->drawCircleHelper(rect.origin.x+_cornerRadius              , rect.origin.y+rect.size.h-_cornerRadius-1, _cornerRadius, 8, _strokeColor);
}

void metaList::redraw(){
	//	Serial << endl<<">>>>>metaList"<<endl;

	metaView::redraw();
	metaView *sv = selectedSubview();
	if(sv != _lastSelectedView){
		_lastSelectedView = sv;
	}
	if(isScrollList() ){
		drawScrollIndicator();
	}

//	Serial << "<<<<<metaList"<<endl;
}

void metaList::removeFromScreen()
{
	metaView::removeFromScreen();
	if(_maxVisibleEntries < _subViews.size() ){
		int16_t indicatorInset = _scrollIndicatorInset+_cornerRadius;
		int16_t indicatorTrackHeight = _frame.size.h- 2*indicatorInset+1;
		GCPoint indicatorOrigin = GCPoint(_frame.size.w-1-(_scrollIndicatorWidth/2),indicatorInset);
		GCRect indicatorTrack = GCRect(indicatorOrigin,GCSize(_scrollIndicatorWidth,indicatorTrackHeight) );
		setFillColor(_backgroundColor);
		fillRect(indicatorTrack);
	}
}

vector<metaView*>::iterator metaList::onIterator(){
	vector<metaView*>::iterator result = _subViews.begin();
	while(result != _subViews.end()){
		if((*result)->getState()==metaView::State::On){
			break;
		}
		result ++;
	}
	return result;
}

vector<metaView*>::iterator metaList::selectedIterator(){
	vector<metaView*>::iterator result = _subViews.begin();
	while(result != _subViews.end()){
		if((*result)->getDrawsOutline()){
			break;
		}
		result ++;
	}
	return result;
}

int16_t metaList::selectedIndex(){
	int16_t result = -1;
	vector<metaView*>::iterator l = selectedIterator();
	if(l!=_subViews.end()){
		result = l-_subViews.begin();
	}
	return result;
}

void metaList::selectIndex(int16_t idx){
	if(idx <0){
		return;
	}
	#if DEBUG_LIST_VALUE
	Serial << "Select "<<idx<<endl;
	#endif
	vector<metaView*>::iterator subIter = _subViews.begin();
	if(selectedIndex() != idx){
		if(_lastSelectedView){
			_lastSelectedView->setDrawsOutline(false);
		}
		subIter += idx;
		if(idx > (this->_visibleStart + this->_maxVisibleEntries-1)){
			this->_visibleStart = idx - (this->_maxVisibleEntries-1);
			#if DEBUG_LIST_VALUE
			Serial << "Scroll up "<<_visibleStart<<endl;
			#endif
			scrollList();
		}else if(idx<this->_visibleStart){
			this->_visibleStart = idx;
			#if DEBUG_LIST_VALUE
			Serial << "Scroll down "<<_visibleStart<<endl;
			#endif
			scrollList();
		}
		if(subIter != _subViews.end()){
			(*subIter)->setDrawsOutline(true);
		}
	}
}

void metaList::clearAllStates(){
	vector<metaView*>::iterator l = _subViews.begin();
	while(l!=_subViews.end()){
		(*l)->setState(metaView::State::Off);
		l++;
	}
}

int16_t metaList::activeIndex(){
	int16_t result = -1;
	vector<metaView*>::iterator l = onIterator();
	if(l!=_subViews.end()){
		result = l-_subViews.begin();
	}
	return result;
}

void metaList::activateIndex(int16_t idx){
	if(idx<0){
		return;
	}
	Serial << "activate "<<endl;
	vector<metaView*>::iterator l = _subViews.begin();
	l +=idx;
	if(l != _subViews.end()){
		clearAllStates();
		(*l)->setState(metaView::State::On);
	}
}

metaView* metaList::activeElement(){
	vector<metaView*>::iterator l = onIterator();
	if(l==_subViews.end()){
		return NULL;
	}
	return (*l);
}


bool metaList::switchSelectedOn(){
	vector<metaView*>::iterator oldIter = onIterator();
	vector<metaView*>::iterator newIter = selectedIterator();
	bool k = false;
	if(oldIter == newIter){
		return true;
	}
	if (oldIter != _subViews.end() ){
		#if DEBUG_LIST_VALUE
		Serial << "Old: "<<(oldIter-_subViews.begin())<<endl;
		#endif
		(*oldIter)->setState(metaView::State::Off);
		k=true;
	}
	if (newIter != _subViews.end()) {
		#if DEBUG_LIST_VALUE
		Serial << "New: "<<(newIter-_subViews.begin())<<endl;
		#endif
		(*newIter)->setState(metaView::State::On);
		k=true;
	}
	return k;
}

int16_t metaList::processEvent(UserEvent* k){
	int16_t result = ResponderResult::ChangedNothing;
	if(_isSelectList &&!_ValueWrapper){
		return ResponderResult::ChangedNothing;
	}
	if(k->getType() == EventType::EventTypeButton){
		ButtonData bData = k->getData().buttonData;
		// Serial << k<<endl;
		int8_t step = 0;
		switch(bData.id){
			case ButtonID::DownButton:
			if(bData.state == ButtonState::ButtonDown){step = 1;} break;

			case ButtonID::UpButton:
			if(bData.state == ButtonState::ButtonDown){step = -1;} break;

			case ButtonID::RightButton:
			Serial << "RightButtonInList"<<endl;
			case ButtonID::CenterButton:
			if(bData.state == ButtonState::ButtonDown){
				if(switchSelectedOn()){
					result = ResponderResult::ChangedVisual;
				}
			}else if (bData.state == ButtonState::ButtonUp){
				if(switchSelectedOn()){
					if(_isSelectList && _ValueWrapper){
						_ValueWrapper->setValue(activeIndex());
					}
					result = ResponderResult::ChangedState;
				}
			} break;

			case ButtonID::LeftButton:
			if(k->getButtonState()==ButtonState::ButtonUp){result = ResponderResult::ResponderExit;}
			break;

			default: break;
		}
		if(step){
			vector<metaView*>::iterator subIter = selectedIterator();
			vector<metaView*>::iterator k = subIter+step;
			if( (k>=_subViews.begin()) && (k<_subViews.end()) ) {
					result = k-_subViews.begin();
				selectIndex(result);
				//(*subIter)->setDrawsOutline(false);
				//(*k)->setDrawsOutline(true);

			}
		}
	}else if(k->getType() == EventType::EventTypeEncoder){
		vector<metaView*>::iterator subIter = selectedIterator();
		EncoderData eData =k->getData().encoderData;
		int8_t offSet = eData.absSteps;
		vector<metaView*>::iterator k = subIter+offSet;
		if( (k>=_subViews.begin()) && (k<_subViews.end()) ) {
			result = k-_subViews.begin();
			selectIndex(result);
			// (*subIter)->setDrawsOutline(false);
			// (*k)->setDrawsOutline(true);
			//
		}
	}
	return result;
}

void metaList::prepareForDisplay()
{
	metaView::prepareForDisplay();
	if(_isSelectList){
		uint16_t index = _ValueWrapper->getValue();
		if(index < _subViews.size()-1){
			_subViews[index]->setState(State::On);
		}
	}
}
