/** a couple of ValueWrapper descendants handling special cases of parameters as:
program/effect index, LED or TFT Brightness or HUE Step changes */

#include "FastLED_Demo.h"

class HUEStepWrapper : public ValueWrapper{
public:
  HUEStepWrapper(int16_t *val):ValueWrapper(val,-10,10,"Hue Speed"){ _hueStep = 1; _frameDelay = 1;_frameDelayOffset=0;}

  virtual void setValue(int16_t k){
    int16_t old = *_value;
    ValueWrapper::setValue(k);
    if(*_value != old){
      float t = exp((abs(*_value) - 5.0) / 2.0);
      //float t = pow((*_value / 7.0),3);
      Serial << "t: "<<t<<endl;
      if(t<1.0){
        _hueStep = 1;
        _frameDelay = 5-abs(*_value);
        //62-(100.0 * abs(t));
      }else if(t == 1){
        _hueStep = 1;
        _frameDelay = 0;
      }else{
        _frameDelay = 0;
        _hueStep = 1+int(t);
      }
      _frameDelay+=_frameDelayOffset;
      _hueStep *= sgn(*_value);
      Serial << "fd:"<<_frameDelay<<" hS:"<<_hueStep<<endl;
    }
  }
  int16_t hueStep(){return _hueStep;}
  int16_t frameDelay(){return _frameDelay;}
private:
  int16_t _frameDelayOffset;
  int16_t _frameDelay;
  int16_t _hueStep;
};

class TFTBrightnessWrapper : public ValueWrapper{
 public:
	TFTBrightnessWrapper(int16_t *val):ValueWrapper(val,0,20,"TFT Brightness"){}

  virtual void setValue(int16_t k){
		int16_t old = *_value;
		ValueWrapper::setValue(k);
		if(*_value!=old){
			int8_t p = displayFromVal(*_value);
			tft.setLuminance(p);
		}
	}
 protected:
	int8_t displayFromVal(int16_t k){
		int16_t p = mapInto(GCSize(_min,_max),GCSize(6,554),k);
		uint8_t v= exp(p/100.0);
		return 256 -v;
	}
	int16_t valFromDisplay(int16_t k){
		int16_t uValue = log(256-k)*100.0;
		int16_t p= mapInto(GCSize(6,554),GCSize(_min,_max),uValue);
		return p;
	}
};

class LEDBrightnessWrapper : public ValueWrapper{
 public:
	LEDBrightnessWrapper(int16_t * val):ValueWrapper(val,0,255,"LED Brightness"){}
	virtual void setValue(int16_t k){
		int16_t old = *_value;
		ValueWrapper::setValue(k);
		if(old!=*_value){
			FastLED.setBrightness(*_value);
		}
	}
};

class ProgramIndexWrapper: public ValueWrapper{
	public:
		ProgramIndexWrapper(EffectList* list, EffectList::iterator *iter):ValueWrapper(0,0,list->size()-1,"Program"),
											 _effectIter(iter),_effectList(list){}

		virtual void setValue(int16_t k){
			if(k>_max){
				k=_max;
			}else if(k<_min){
				k=_min;
			}
			*_effectIter = _effectList->begin()+k;
		}
		virtual int16_t getValue(){
			return (*_effectIter) - _effectList->begin();
		}
	protected:
		EffectList::iterator *_effectIter;
		EffectList 					 *_effectList;
};

class PaletteIndexWrapper: public ValueWrapper{
	public:
		PaletteIndexWrapper(PaletteList* list, PaletteList::iterator *iter):ValueWrapper(0,0,list->size()-1,"Program"),
											 _paletteIter(iter),_paletteList(list){}

		virtual void setValue(int16_t k){
			if(k>_max){
				k=_max;
			}else if(k<_min){
				k=_min;
			}
			*_paletteIter = _paletteList->begin()+k;
		}
		virtual int16_t getValue(){
			return (*_paletteIter) - _paletteList->begin();
		}

	protected:
		PaletteList::iterator *_paletteIter;
		PaletteList 					*_paletteList;
};
