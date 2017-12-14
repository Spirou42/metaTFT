/**
* a here are the ui elements
*/

#ifndef __UIHELPERS_H__
#define __UIHELPERS_H__

#include <vector>
#include "Arduino.h"
#include "TFTDisplay.hpp"
#include "font_Arial.h"

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

/** the Responder Stack, actually a std::vector, contains all views, that are on the screen.

  */
typedef std::vector<metaView*> ResponderStack;
extern ResponderStack responderStack;
int sgn(float v);

/** simple wrapper for int16_t values. It gives the value a name and handles the basic min/max/value behaviour.
It wrappes a simpleint16_t into a UI feasable container. In addition, the setValue method can be used by subclasses to trigger instatanious actions.
*/
class ValueWrapper{
 public:
  ValueWrapper(int16_t* val, int16_t min, int16_t max, String name):_value(val),_min(min),_max(max),_name(name){}

  virtual int16_t getValue(){return *_value;}
  virtual void setValue(int16_t val){
    if(val < _min){
      val = _min;
    }else if(val>_max){
      val = _max;
    }
    *_value = val;
  }

  virtual int16_t getMinValue(){return _min;}
  virtual int16_t getMaxValue(){return _max;}
  virtual String& getName(){return _name;}
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

/** the editor takes a metaView and a ValueWrapper  and links them for user interaction*/
class ValueEditor{
 public:
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
  void initResponder(ResponderStack* rS){
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
    ResponderStack * _responderStack;
    uint16_t _respondsToEvents;
    ValueEditor* _action;
    ValueWrapper* _ValueWrapper;
};


int processUserEvents(unsigned long now, void * userdata);

#endif
