#pragma once
#include "FlashUI\FlashUI.h"
#include "Player.h"

class CInventoryEvents : public IUIElementEventListener {

public:

	virtual void OnUIEvent(IUIElement *pSender, const SUIEventDesc& event, const SUIArguments &args) override {
	
		string eventName = event.sDisplayName;

		if (eventName == "") {
		}

	}

	CPlayerComponent *player = nullptr;

};