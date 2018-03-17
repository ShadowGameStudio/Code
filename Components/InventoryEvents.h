#pragma once
#include "FlashUI\FlashUI.h"
#include "Player.h"
#include "DrugComponent.h"

class CInventoryEvents : public IUIElementEventListener {

public:

	virtual void OnUIEvent(IUIElement *pSender, const SUIEventDesc& event, const SUIArguments &args) override {
	
		string eventName = event.sDisplayName;

		if (eventName == "ConsumeDrug") {

			//Create in to store the EntityId
			int Id;
			//Get the EntityId
			args.GetArg<int>(0, Id);

			//If it can get the entity from the EntityId, continue
			if (IEntity *pEntityToConsume = gEnv->pEntitySystem->GetEntity(Id)) {
				//If it can get the DrugComponent, continue
				if (CDrugComponent *pItemToConsume = pEntityToConsume->GetComponent<CDrugComponent>()) {
					//Consume the actual food
					pItemToConsume->Consume(pItemToConsume);

				}

			}

		}
		//Else if the event name is DropItem, continue
		else if (eventName == "DropItem") {

			//Create int to store the EntityId
			int Id;
			//Get the EntityId
			args.GetArg<int>(0, Id);

			//If it can get an entity from the Id, continue
			if (IEntity *pEntityToDrop = gEnv->pEntitySystem->GetEntity(Id)) {
				//If it can get the items ItemComponent, continue
				if(SItemComponent *pItemToDrop = pEntityToDrop->GetComponent<SItemComponent>()) {
					//Drop the actual item
					pItemToDrop->Drop(pItemToDrop);

				}


			}

		}

	}

};