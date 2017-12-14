/** a couple of ValueWrapper descendants handling special cases of parameters as:
program/effect index, LED or TFT Brightness or HUE Step changes */

#ifndef __FastLED_DemoParameterWrapper__
#define __FastLED_DemoParameterWrapper__

#include "FastLED_Demo.h"

// class ProgramIndexWrapper: public ValueWrapper{
// 	public:
// 		ProgramIndexWrapper(EffectList* list, EffectList::iterator *iter):ValueWrapper(0,0,list->size()-1,"Program"),
// 											 _effectIter(iter),_effectList(list){}
//
// 		virtual void setValue(int16_t k){
// 			if(k>_max){
// 				k=_max;
// 			}else if(k<_min){
// 				k=_min;
// 			}
// 			*_effectIter = _effectList->begin()+k;
// 		}
// 		virtual int16_t getValue(){
// 			return (*_effectIter) - _effectList->begin();
// 		}
// 	protected:
// 		EffectList::iterator *_effectIter;
// 		EffectList 					 *_effectList;
// };

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
#endif
