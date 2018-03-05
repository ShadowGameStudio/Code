#include "StdAfx.h"

#include "InventoryComponent.h"
#include <FlashUI\FlashUI.h>
#include "InventoryEvents.h"

void CInventoryComponent::Initialize() {

	//Gets the different UI elements for the inventory
	pUIInventory = gEnv->pFlashUI->GetUIElement("InventorySystem");
	pInventoryShow = gEnv->pFlashUI->GetUIAction("inventoryshow");
	pInventoryHide = gEnv->pFlashUI->GetUIAction("inventoryhide");
	pInventoryShowCursor = gEnv->pFlashUI->GetUIAction("inventoryshowcursor");
	pInventoryHideCursor = gEnv->pFlashUI->GetUIAction("inventoryhidecursor");
	pManager = gEnv->pFlashUI->GetUIActionManager();
	//Starts the initialization of the actual UI
	pUIInventory->CallFunction("Initialize");
	//Makes sure the UI is hidden on start
	pManager->StartAction(pInventoryHide, "InventorySystem");
}

uint64 CInventoryComponent::GetEventMask() const {
	return uint64();
}

void CInventoryComponent::ProcessEvent(SEntityEvent& event) {}

//Returns the slot that the passed in weapon is located in
int CInventoryComponent::GetWeaponSlot(SItemComponent * pNewWeapon) {

	//If is not weapon, return
	if (!pNewWeapon)
		return -1;

	//Check every slot if the weapon is located there
	for (int i = 0; i < WEAPON_CAPACITY; i++) {
		if (pWeapon[i] == pNewWeapon) {
			return i;
		}
	}

	return -1;
}

//Adds an item to the inventory
bool CInventoryComponent::AddItem(SItemComponent *pNewItem) {

	//If not the item you are looking at, return
	if (!pNewItem)
		return false;

	//If you have carry weight left continue
	if (fCurrentWeight < fInventoryCapKilo) {

		float fCapOver = fInventoryCapKilo - fCurrentWeight;
		//If there is carrying weight enough over continue
		if (fCapOver > pNewItem->GetItemWeight()) {
			//If it is some sort of weapon(meele of non)
			if (pNewItem->GetItemType() == 2 || 4) {
				//Set the arguments for the UI function call
				SUIArguments args;
				for (int i = 0; i < WEAPON_CAPACITY; i++) {
					if (!pWeapon)
						//Sets a slot to the weapon
						args.AddArgument<int>(i);
				}
				args.AddArgument<string>(pNewItem->GetItemName());
				args.AddArgument<int>(pNewItem->GetEntityId());
				args.AddArgument<int>(pNewItem->GetItemType());
				args.AddArgument<float>(pNewItem->GetItemWeight());
				//Calls the UI function
				pUIInventory->CallFunction("AddWeapon", args);
			}
			//If it is a backpack do this
			else if(pNewItem->GetItemType() == 3){
				//TODO: Attach backpack here
			}
			//If it's not weapon or backpack, just add it normally
			else {
				SUIArguments args;
				args.AddArgument<string>(pNewItem->GetItemName());
				args.AddArgument<int>(pNewItem->GetEntityId());
				args.AddArgument<int>(pNewItem->GetItemType());
				args.AddArgument<float>(pNewItem->GetItemWeight());
				pUIInventory->CallFunction("AddItem", args);
			}
			return true;
		}
		//If there isn't enough carrying weight
		else {
			//TODO: Show message that you can't carry anymore here!
		}

	}
	return false;
}

//Removes an item from the inventory
void CInventoryComponent::RemoveItem(SItemComponent *pNewItem) {

	if (!pNewItem)
		return;

	//Remove the item weight from the current weight carried
	fCurrentWeight -= pNewItem->GetItemWeight();

}

//Removes a weapon from the inventory
void CInventoryComponent::RemoveWeapon(SItemComponent *pNewItem) {

	if (!pNewItem)
		return;
	
	//Remove the weapon weight from the current weigth carried
	fCurrentWeight -= pNewItem->GetItemWeight();
	int index = GetWeaponSlot(pNewItem);
	//Check so that the slot isn't -1(invalid)
	if (index > -1) {
		pWeapon[index] = nullptr;
		//Detach the weapon from the back when you remove it
		DetachFromBack(index);
	}

}

void CInventoryComponent::AttachToBack(SItemComponent * pWeaponToAttach, int slotId) {}

void CInventoryComponent::DetachFromBack(int slotId) {}

void CInventoryComponent::Attach(SItemComponent * pWeaponToAttach) {}

void CInventoryComponent::AttachToHand(SItemComponent * pWeaponToAttach) {}

void CInventoryComponent::DetachFromHand() {}

void CInventoryComponent::SelectSlot(int slotId) {}

void CInventoryComponent::SetItemSlot(SItemComponent * pWeaponToSelect, int slotId) {}

void CInventoryComponent::SelectItem(SItemComponent * pWeaponToSelect) {}

void CInventoryComponent::DeselectItem() {}

