#include "StdAfx.h"

#include "InventoryComponent.h"
#include <FlashUI\FlashUI.h>
#include "InventoryEvents.h"
#include "ItemProperties.h"

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
int CInventoryComponent::GetWeaponSlot(SItemComponent *pNewWeapon) {

	//If is not weapon, return
	if (!pNewWeapon)
		return -1;

	//Check every slot if the weapon is located there
	for (int i = 0; i < WEAPON_CAPACITY; i++) {
		if (pWeapon[i] == pNewWeapon) 
			return i;
	}

	return -1;
}

//Adds an item to the inventory
bool CInventoryComponent::AddItem(SItemComponent *pNewItem) {

	//If not the item you are looking at, return
	if (!pNewItem)
		return false;

	//If you have carry weight left continue
	if (m_fCurrentWeight < m_fInventoryCapKilo) {

		float fCapOver = m_fInventoryCapKilo - m_fCurrentWeight;
		//If there is carrying weight enough over continue
		if (fCapOver > pNewItem->GetItemWeight()) {
			//If it is some sort of weapon(meele of non)
			if (pNewItem->GetItemType() == EItemType::MeeleWeapon || EItemType::Weapon) {
				//Set the arguments for the UI function call
				SUIArguments args;
				for (int i = 0; i < WEAPON_CAPACITY; i++) {
					//If there is no weapon in the slot, continue
					if (!pWeapon[i]) {
						//Sets a slot to the weapon
						pWeapon[i] = pNewItem;
						args.AddArgument<int>(i);
						args.AddArgument<string>(pNewItem->GetItemName());
						args.AddArgument<int>(pNewItem->GetEntityId());
						args.AddArgument<int>(pNewItem->GetItemType());
						args.AddArgument<float>(pNewItem->GetItemWeight());
						//Calls the UI function
						pUIInventory->CallFunction("AddWeapon", args);
						return true;

					}

				}
			}
			//If it is a backpack do this
			else if(pNewItem->GetItemType() == EItemType::Backpack){
				
				SUIArguments args;
				if (!pBackpack) {
					//Adds all the arguments for flash
					//Assigns the backpack
					pBackpack = pNewItem;
					args.AddArgument<string>(pNewItem->GetItemName());
					args.AddArgument<int>(pNewItem->GetEntityId());
					args.AddArgument<int>(pNewItem->GetItemType());
					args.AddArgument<float>(pNewItem->GetItemWeight());
					//Calls the UI function
					pUIInventory->CallFunction("AddBackpack", args);
					return true;

				}
				else {
					//TODO: show message saying: You can only carry one backpack at a time
				}
			}
			else if (pNewItem->GetItemType() == EItemType::Gasmask) {

				SUIArguments args;
				for (int i = 0; i < GASMASK_CAPACITY; i++) {
					if (!pGasmask[i]) {
						//Adds all the arguments for flash
						//Sets a slot to the gasmask
						pGasmask[i] = pNewItem;
						args.AddArgument<string>(pNewItem->GetItemName());
						args.AddArgument<int>(pNewItem->GetEntityId());
						args.AddArgument<int>(pNewItem->GetItemType());
						args.AddArgument<float>(pNewItem->GetItemWeight());
						//Calls the UI function
						pUIInventory->CallFunction("AddGasmask", args);
						//Sets the current gasmask
						pCurrentGasmask = pNewItem;
						return true;
					}

				}
			}
			//If it's not weapon, gasmask or backpack, just add it normally
			else {
				//Adds all of the arguments for flash
				SUIArguments args;
				args.AddArgument<string>(pNewItem->GetItemName());
				args.AddArgument<int>(pNewItem->GetEntityId());
				args.AddArgument<int>(pNewItem->GetItemType());
				args.AddArgument<float>(pNewItem->GetItemWeight());
				pUIInventory->CallFunction("AddItem", args);
				
				return true;
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
	m_fCurrentWeight -= pNewItem->GetItemWeight();

}

//Removes a weapon from the inventory
void CInventoryComponent::RemoveWeapon(SItemComponent *pNewItem) {

	if (!pNewItem)
		return;
	
	//Remove the weapon weight from the current weigth carried
	m_fCurrentWeight -= pNewItem->GetItemWeight();
	int index = GetWeaponSlot(pNewItem);
	//Check so that the slot isn't -1(invalid)
	if (index > -1) {
		pWeapon[index] = nullptr;
		//Detach the weapon from the back when you remove it
		DetachFromBack(index);
	}

}

//Attaches a weapon to players back
void CInventoryComponent::AttachToBack(SItemComponent *pItemToAttach, int slotId) {

	//If it's not the right weapon or if the slot is less than zero, return
	if (!pItemToAttach || slotId < 0)
		return;

	//Create a string from the slotId int
	string sSlotString = ToString(slotId);
	//Get the players PlayerComponent
	CPlayerComponent *pPlayer = m_pEntity->GetComponent<CPlayerComponent>();
	//Create attachment entity for the weapon
	CEntityAttachment *pAttachmentItem = new CEntityAttachment();
	//Set the above entitys ID to the weapons
	pAttachmentItem->SetEntityId(pItemToAttach->GetEntityId());

	//If it can get the players character, continue
	if (ICharacterInstance *pCharacter = pPlayer->GetAnimations()->GetCharacter()) {
		//If it can get the characters attachments, continue
		if(IAttachmentManager *pAttMan = pCharacter->GetIAttachmentManager()){
			//Create string to get the correct character attachment
			string sAttName;

			//If the item type is Backpack, continue
			if (pItemToAttach->GetItemType() == EItemType::Backpack) {
				//Set the attachment to the backpack attachment
				sAttName = "backpack_att_00";
				//Set that the player is wearing a backpack
				pPlayer->bPlayerHasBackpack = true;
				ActivateBackpack(pItemToAttach);
			}
			//Else, continue
			else {
				//Set the attachment to the back attachment
				sAttName = "back_att_0" + sSlotString;
			}

			//If it can get the specified attachment, continue
			if (IAttachment *pAttachment = pAttMan->GetInterfaceByName(sAttName)) {
				//Attach the weapon
				pAttachment->AddBinding(pAttachmentItem);
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
void CInventoryComponent::Attach(SItemComponent *pItemToAttach) {

	//If it's a weapon, continue
	if (pItemToAttach->GetItemType() == EItemType::Weapon || EItemType::MeeleWeapon) {
		//Check every slot if it's the specified weapon
		for (int i = 0; i < WEAPON_CAPACITY; i++) {
			//If it is the specified weapon, continue
			if (pWeapon[i] == pItemToAttach) {
				//Detach the weapon from player
				pItemToAttach->GetEntity()->DetachThis();
				//Attach it to the back
				AttachToBack(pItemToAttach, i);
			}

		}

	}
	//If it's a gasmask, continue
	else if (pItemToAttach->GetItemType() == EItemType::Gasmask) {
		//Check every slot for the specified gasmask
		for (int i = 0; i < GASMASK_CAPACITY; i++) {
			//If it's the specified gasmask, continue
			if (pGasmask[i] == pItemToAttach) {
				//Detach the gasmask from the player
				pItemToAttach->GetEntity()->DetachThis();
				//Attach it to the face
				AttachToFace(pItemToAttach);
			}

		}

	}
	//Else if it's a backpack, continue
	else if (pItemToAttach->GetItemType() == EItemType::Backpack) {
		//If the backpack in the inventory is the passed in one, continue
		if (pBackpack == pItemToAttach) {
			//Detach the backpack from the player
			pItemToAttach->GetEntity()->DetachThis();
			//Attach it to the back
			AttachToBack(pItemToAttach, 0);

		}

	}
	//If it's none of the above, continue
	else {
		return;
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

//Attach a mask to the face
void CInventoryComponent::AttachToFace(SItemComponent *pItemToAttach) {

	if (!pItemToAttach)
		return;

	//Get the players PlayerComponent
	CPlayerComponent *pPlayer = m_pEntity->GetComponent<CPlayerComponent>();
	//Create attachment entity for the mask
	CEntityAttachment *pAttachmentItem = new CEntityAttachment();
	//Assigns the item to the attachment
	pAttachmentItem->SetEntityId(pItemToAttach->GetEntityId());

	//If it can get the players character, continue
	if (ICharacterInstance *pCharacter = pPlayer->GetAnimations()->GetCharacter()) {
		//If it can get the characters attachments, continue
		if (IAttachmentManager *pAttMan = pCharacter->GetIAttachmentManager()) {
		
			//The attachment string
			string sAttName = "mask";
			//If it can get the attachment
			if (IAttachment *pAttachment = pAttMan->GetInterfaceByName(sAttName)) {
				//Attach the item
				pAttachment->AddBinding(pAttachmentItem);
				//If it can get the items gasmask component, continue
				if (CGasmaskComponent *pGasmask = pItemToAttach->GetEntity()->GetComponent<CGasmaskComponent>()) {
					//Sets that the player is wearing a gasmask
					pPlayer->bPlayerIsWearingGasmask = true;
				}
				//It it can't, continue
				else {
					//Sets that the player isn't wearing any gasmask
					pPlayer->bPlayerIsWearingGasmask = false;
				}

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
	pLastSelectedWeapon = pSelectedWeapon;
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

bool CInventoryComponent::ActivateBackpack(SItemComponent *pBackpack) {

	//If the item actually is a backpack, continue
	if (pBackpack->GetItemType() == EItemType::Backpack) {

		//If the backpacks level is one, continue
		if (pBackpack->GetItemLevel() == 1) {
			//Add 25 to the max weight
			m_fInventoryCapKilo += 25;
			return true;

		}
		//Else if the level is two, continue
		else if (pBackpack->GetItemLevel() == 2) {
			//Add 50 to the max weight
			m_fInventoryCapKilo += 50;
			return true;

		}
		//Else if the level is three, continue
		else if (pBackpack->GetItemLevel() == 3) {
			//Add 75 to the max weight
			m_fInventoryCapKilo += 75;
			return true;

		}
		//Else, return false
		else {
			return false;
		}

	}

	return false;

}

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
