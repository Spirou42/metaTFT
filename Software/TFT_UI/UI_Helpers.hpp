/**
* a here are the ui elements
*/

#ifndef __UIHELPERS_H__
#define __UIHELPERS_H__

#include <vector>
#include "Arduino.h"
#include "TFTDisplay.hpp"
#include "font_Arial.h"
#include "UI_UserEvent.hpp"
#include "TFT_UI_Internal.h"

#define DEBUG_RESPONDER 0
/** @todo: rework */
/** AllignmentMasks terms*/
#define HALLIGN_LEFT    (1<<1)
#define HALLIGN_CENTER  (1<<2)
#define HALLIGN_RIGHT   (1<<3)
#define VALLIGN_TOP     (1<<4)
#define VALLIGN_CENTER  (1<<5)
#define VALLIGN_BOTTOM  (1<<6)

using namespace std;

class UserEvent;
class metaView;

/** the Responder Stack, actually a std::vector, contains all views, that are on the screen. The topOfStack will get the all user events
  */

TFTUI_NAMESPACE_BEGIN
typedef std::vector<metaView*> ResponderStack;
extern ResponderStack responderStack;
TFTUI_NAMESPACE_END

int sgn(float v);

/** simple wrapper for int16_t values. It gives the value a visual name and handles the basic min/max/value behaviour.
It wrappes a simple int16_t into a UI feasable container. In addition, the setValue method can be used by subclasses to trigger instatanious actions.
For an example have a look at TFTBrightnessWrapper below.
*/
class ValueWrapper{
 public:
  ValueWrapper():_value(NULL),_min(0),_max(0),_name(){};
  ValueWrapper(int16_t* val, int16_t min, int16_t max, String name):_value(val),_min(min),_max(max),_name(name){}
  virtual ~ValueWrapper(){_value = NULL;}
  virtual int16_t getValue(){return *_value;}
  virtual void setValue(int16_t val){
    if(val < _min){
      val = _min;
    }else if(val>_max){
      val = _max;
    }
    *_value = val;
  }

  virtual void setValueStore(int16_t* valStore){_value = valStore;}
  virtual int16_t getMinValue(){return _min;}
  virtual void setMinValue(int16_t newMin){_min = newMin;}
  virtual int16_t getMaxValue(){return _max;}
  virtual void setMaxValue(int16_t newMax){_max = newMax;}
  virtual String& getName(){return _name;}
  virtual void setName(String& newName){_name = newName;}

  int16_t mapInto(GCSize source, GCSize target, int16_t input){
    float inp = (100.0/(source.h-source.w)) * (input -source.w);
    int16_t out = (target.h-target.w)*inp/100.0 + target.w;
    return out;
  }
  friend Print& operator<<(Print& obj,ValueWrapper* v){
    if(v){
      obj << "["<<v->_name<<"]{"<<*(v->_value)<<", "<<v->_min<<", "<<v->_max<<"}";
    }else{
      obj << "[NONE]";
    }
    return obj;
  }

 protected:
   int16_t* _value;
   int16_t _min;
   int16_t _max;
   String _name;
};

/** the editor takes a metaView and a ValueWrapper and links them for user interaction */
class ValueEditor{
 public:
   ValueEditor(){}
   ValueEditor(metaView* editorView, ValueWrapper *value){
   	_editor = editorView;
   	_value = value;
   };
   void operator()(void) const {
   	Serial << "action called with nothing "<<endl;
   	return;
   };
   void operator()(int16_t val) const{
   	Serial <<"action called with "<<val<<endl;
   	if(_value){
   		_value->setValue(val);
   	}
   	return;
   };
   friend Print& operator<<(Print& obj, ValueEditor* a){
     Serial << "[Action]"<<a->_value<<_HEX((unsigned long)a->_editor);
     return obj;
   }
   metaView* getEditor(){return _editor;}
   ValueWrapper* getValue(){return _value;}
   void setValue(ValueWrapper* wrapper){_value = wrapper;}
  protected:
   metaView* _editor;                               /// this is the next responder
   ValueWrapper *_value;                          /// pointer to a concrete int16_t
};

/**
the Responder is the base class for all event processing related stuff
*/
typedef enum _responderResult{
  ChangedValue    = -1,
  ChangedVisual   = -1,                          ///< this result from a processEvent call will remove this responder from the responder stack.
  ChangedState    = -2,
  ChangedNothing  = -3,
  ResponderExit   = -4,
}ResponderResult;

/** baseclass for EventResponding */
class metaResponder{
 public:

  metaResponder():_responderStack(NULL),_respondsToEvents(),_action(NULL){}
  void initResponder(TFT_UI::ResponderStack* rS){
    _responderStack = rS;}

  virtual void setRespondsToEvents(uint16_t m){
    _respondsToEvents = m;}

  virtual uint16_t respondsToEvents(){
    return _respondsToEvents;}

  void setAction(ValueEditor* action){_action = action;}
  ValueEditor* getAction(){return _action;}

  virtual int16_t selectedIndex(){return -1;}                             /// returns the index of the selected (outlined) entry
  virtual void selectIndex(int16_t){}

  virtual int16_t activeIndex(){return -1;}
  virtual void activateIndex(int16_t){}
  virtual metaView* activeElement(){return NULL;};

  virtual int16_t processEvent(UserEvent *k){return ChangedNothing;};

  virtual void setValueWrapper(ValueWrapper* val){
    _ValueWrapper = val;
  }
  ValueWrapper* getValueWrapper(){return _ValueWrapper;}

  virtual int16_t getNumericValue(){
    if(_ValueWrapper){
      return _ValueWrapper->getValue();
    }
    return 0;
  }

  virtual void setNumericValue(int16_t k){
    if(_ValueWrapper){
      _ValueWrapper->setValue(k);
    }
  }
 protected:
    TFT_UI::ResponderStack * _responderStack;
    uint16_t _respondsToEvents;
    ValueEditor* _action;
    ValueWrapper* _ValueWrapper;
};
TFTUI_NAMESPACE_BEGIN
typedef void(*IREventHandler)(UserEvent*);
/* this function processes the User events and passes them to the top most view in the responder stack */
int processUserEvents(unsigned long now, void * userdata);
TFTUI_NAMESPACE_END
#endif
