/**
* implementation of the UIHelper classes
*/

#include "UIHelpers.hpp"
#include "UI_Views.hpp"
#include "UserEvent.hpp"

ResponderStack responderStack;

int processUserEvents(unsigned long now, void * userdata){
	if(responderStack.size()==0){
		#if DEBUG_RESPONDER
		Serial << "There is no top responder"<<endl;
		#endif
		return 0;
	}
	metaView *resp = responderStack.back();
	while(eventQueue.length()){
		UserEvent *evnt = eventQueue.popEvent();
		//uint16_t l = evnt->eventMask();
		uint16_t k = resp->respondsToEvents();
		//int16_t oldSelectedElement =resp->selectedIndex();
		if((evnt->matchesMask(k)) ){
			int16_t result = resp->processEvent(evnt);
			if(result > ResponderResult::ChangedValue){
				#if DEBUG_RESPONDER
				Serial << "Responder changed Value x = "<<result<<endl;
				ValueEditor *a = resp->getAction();
				if(a){
					Serial << "Responder got a value action"<<endl;
				}else{
					Serial << "Responder did not have a value action"<<endl;
				}
				#endif
				resp->redraw();
			}else{
				switch(result){
					case ResponderResult::ChangedNothing:
					#if DEBUG_RESPONDER
					Serial << "Responder did not change"<< endl;
					#endif
					break;

					case ResponderResult::ChangedVisual:
					#if DEBUG_RESPONDER
					Serial << "Responder changed visualy"<< endl;
					#endif
					resp->redraw();
					break;

					case ResponderResult::ChangedState:			/// this only is send if there was a list select
					{
						#if DEBUG_RESPONDER
						int16_t idx =resp->activeIndex();
						Serial << "Responder changed state selected "<<idx<<endl;
						#endif
						ValueEditor *a = resp->getAction();
						if(a){
							#if DEBUG_RESPONDER
							Serial << "Responder has a action for this"<< endl;
							#endif
						}else{
							// check if the list entry has an ValueEditor added;
							metaView * p = resp->activeElement();
							if(p){
								ValueEditor *a = p->getAction();
								if(a){
									#if DEBUG_RESPONDER
									Serial << "got an action on the active Elements"<<endl;
									Serial << a<<endl;
									#endif
									metaView* aView = a->getEditor();
									ValueWrapper* val = a->getValue();
									if(aView){
										if(val){
											aView->setValueWrapper(val);
										}
										responderStack.push_back(aView);
										aView->prepareForDisplay();
										aView->redraw();
									}
								}else{
									#if DEBUG_RESPONDER
									Serial << "on element does not have a action"<<endl;
									#endif
								}
							}
						}
						resp->redraw();
					}
					break;

					case ResponderResult::ResponderExit:
						#if DEBUG_RESPONDER
						Serial << "Responder has exited"<< endl;
						#endif
						responderStack.pop_back();
						resp->removeFromScreen();
						if(responderStack.size()){
							metaView * k=responderStack.back();
							k->setNeedsRedraw();
							k->redraw();
						}
					break;

				}
			}
		}
		delete evnt;
	}
	return 0;
}
