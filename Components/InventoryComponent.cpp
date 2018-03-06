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
					//If there is no weapon in the slot, continue
					if (!pWeapon[i]) {
						//Sets a slot to the weapon
						args.AddArgument<int>(i);

						//Attaches the weapon to the back
						pNewItem->GetEntity()->DetachThis();
						AttachToBack(pNewItem, i);

					}

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

//Attaches a weapon to players back
void CInventoryComponent::AttachToBack(SItemComponent *pWeaponToAttach, int slotId) {

	//If it's not the right weapon or if the slot is less than zero, return
	if (!pWeaponToAttach || slotId < 0)
		return;

	//Create a string from the slotId int
	string sSlotString = ToString(slotId);
	//Get the players PlayerComponent
	CPlayerComponent *pPlayer = m_pEntity->GetComponent<CPlayerComponent>();
	//Create attachment entity for the weapon
	CEntityAttachment *pAttachmentWeapon = new CEntityAttachment();
	//Set the above entitys ID to the weapons
	pAttachmentWeapon->SetEntityId(pWeaponToAttach->GetEntityId());

	//If it can get the players character, continue
	if (ICharacterInstance *pCharacter = pPlayer->GetAnimations()->GetCharacter()) {
		//If it can get the characters attachments, continue
		if(IAttachmentManager *pAttMan = pCharacter->GetIAttachmentManager()){
			
			//Create string to get the correct character attachment
			string sAttName = "back_att_0" + sSlotString;
			//If it can get the specified attachment, continue
			if (IAttachment *pAttachment = pAttMan->GetInterfaceByName(sAttName)) {
				//Attach the weapon
				pAttachment->AddBinding(pAttachmentWeapon);
			}

		}
	}
}

//Detaches a weapon from the players back
void CInventoryComponent::DetachFromBack(int slotId) {

	//If slotId is less than zero, return
	if (slotId < 0)
		return;

	//Create a string from the slotId int
	string sSlotString = ToString(slotId);
	//Get the players PlayerComponent
	CPlayerComponent *pPlayer = m_pEntity->GetComponent<CPlayerComponent>();

	//If it can get the players character, continue
	if (ICharacterInstance *pCharacter = pPlayer->GetAnimations()->GetCharacter()) {
		//If it can get characters attachments, continue
		if (IAttachmentManager *pAttMan = pCharacter->GetIAttachmentManager()) {
			
			//Create string to get the correct character attachment
			string sAttName = "back_att_0" + sSlotString;
			//If it can get the correct attachment, continue
			if (IAttachment *pAttachment = pAttMan->GetInterfaceByName(sAttName)) {
				//Detach the weapon
				pAttachment->ClearBinding();
			}
		}
	}
}

//Gets which slot to attach the weapon to
void CInventoryComponent::Attach(SItemComponent *pWeaponToAttach) {

	//Check every slot if it's the specified weapon
	for (int i = 0; i < WEAPON_CAPACITY; i++) {
		//If it is the specified weapon, continue
		if (pWeapon[i] == pWeaponToAttach) {
			//Detach the weapon from player
			pWeaponToAttach->GetEntity()->DetachThis();
			//Attach it to the back
			AttachToBack(pWeaponToAttach, i);
		}

	}

}

//Attach a weapon to the hand
void CInventoryComponent::AttachToHand(SItemComponent *pWeaponToAttach) {

	//If it's not the correct weapon, return
	if (!pWeaponToAttach)
		return;

	//Get the players PlayerComponent
	CPlayerComponent *pPlayer = m_pEntity->GetComponent<CPlayerComponent>();
	//Create attachment entity for the weapon
	CEntityAttachment *pAttachmentWeapon = new CEntityAttachment();
	//Sets the above entity id to the weapons entity id
	pAttachmentWeapon->SetEntityId(pWeaponToAttach->GetEntityId());

	//If it can get the players character, continue
	if (ICharacterInstance *pCharacter = pPlayer->GetAnimations()->GetCharacter()) {
		//It it can get the characters attachments
		if (IAttachmentManager *pAttMan = pCharacter->GetIAttachmentManager()) {

			//The attachment string
			string sAttName = "meeleWeapon";
			//If it can get the attachment
			if (IAttachment *pAttachment = pAttMan->GetInterfaceByName(sAttName)) {
				//Attach the weapon
				pAttachment->AddBinding(pAttachmentWeapon);
			}
		}
	}

}

//Detach weapon from the hand
void CInventoryComponent::DetachFromHand() {

	//Get the players PlayerComponent
	CPlayerComponent *pPlayer = m_pEntity->GetComponent<CPlayerComponent>();
	//If it can get the players charactes, continue
	if (ICharacterInstance *pCharacter = pPlayer->GetAnimations()->GetCharacter()) {
		//If it can get the characters attachments, continue
		if (IAttachmentManager *pAttMan = pCharacter->GetIAttachmentManager()) {
			
			//String for attachment
			string sAttName = "meeleWeapon";
			//If it can get the attachment, continue
			if (IAttachment *pAttachment = pAttMan->GetInterfaceByName(sAttName)) {
				//Detach the weapon
				pAttachment->ClearBinding();
			}
		}
	}

}

//Selects the weapon slot that you want to use
void CInventoryComponent::SelectSlot(int slotId) {

	//If slotId is less than zero, return
	if (slotId < 0)
		return;

	//Sets the last selected weapon to the currently selected weapon
	SItemComponent *pLastSelectedWeapon = pSelectedWeapon;
	//Deselect the weapon
	DeselectWeapon();

	//If last selected weapon is not equal to the weapon you want to equip, continue
	if (pLastSelectedWeapon != pWeapon[slotId])
		//Select the weapon in the specified slot
		SelectWeapon(pWeapon[slotId]);

}

//Selects the weapon you want to use
void CInventoryComponent::SelectWeapon(SItemComponent *pWeaponToSelect) {

	//If it's not the correct weapon, return
	if (!pWeaponToSelect)
		return;

	//Detach the weapon from the back
	DetachFromBack(GetWeaponSlot(pWeaponToSelect));
	//Sets the selected weapon to the weapon you want to select
	pSelectedWeapon = pWeaponToSelect;
	//Attaches the weapon to the hand
	AttachToHand(pWeaponToSelect);

}

//Deselects the currently selected weapon
void CInventoryComponent::DeselectWeapon() {

	//If it's not the correct weapon, return
	if (!pSelectedWeapon)
		return;

	//Detach the weapon from the hand
	DetachFromHand();
	//Attach the selected weapon to the back
	AttachToBack(pSelectedWeapon, GetWeaponSlot(pSelectedWeapon));
	//Sets the selected weapon to nothing
	pSelectedWeapon = nullptr;

}

//Inventory UI

//Shows/Hides the inventory
void CInventoryComponent::Show() {

	//TODO: Add Steam name

	//Freeze the player when the inventory is open
	m_pEntity->GetComponent<CPlayerComponent>()->FreezePlayer(bIsInventoryOpened);
	//The UI arguments
	SUIArguments args;
	args.AddArgument<bool>(bIsInventoryOpened);
	//Call flash show function
	pUIInventory->CallFunction("Show", args);
	
	//If inventory is open, continue
	if (bIsInventoryOpened) {
		//Show the cursor
		pManager->StartAction(pInventoryShowCursor, "InventorySystem");
	}
	//If inventory isn't open, continue
	else {
		//Hide the inventory
		pManager->StartAction(pInventoryShow, "InventorySystem");
		//Hide the cursor
		pManager->StartAction(pInventoryHideCursor, "InventorySystem");
	}

}
