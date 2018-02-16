#include "StdAfx.h"

#include "InventoryComponent.h"
#include <FlashUI\FlashUI.h>
#include "InventoryEvents.h"
#include "steam\steam_api.h"

CInventoryEvents inventoryEvents;

void CInventoryComponent::Initialize() {

	pUIInventory = gEnv->pFlashUI->GetUIElement("InventorySystem");
	pInventoryShow = gEnv->pFlashUI->GetUIAction("inventoryshow");
	pInventoryHide = gEnv->pFlashUI->GetUIAction("inventoryhide");
	pInventoryShowCursor = gEnv->pFlashUI->GetUIAction("inventoryshowcursor");
	pInventoryHideCursor = gEnv->pFlashUI->GetUIAction("inventoryhidecursor");
	pManager = gEnv->pFlashUI->GetUIActionManager();
	pUIInventory->CallFunction("Initialize");
	pManager->StartAction(pInventoryShow, "InventorySystem");

}

uint64 CInventoryComponent::GetEventMask() const {
	return uint64();
}

void CInventoryComponent::ProcessEvent(SEntityEvent & event) {}

int CInventoryComponent::GetItemSlot(SItemComponent * pNewItem) {
	
	if(!pNewItem)
	return -1;

	for (int i = 0; i < INVENTORY_CAPACITY; i++) {

		if (pItem[i] == pNewItem) {

			return i;

		}
	}

	return -1;
}

int CInventoryComponent::GetWeaponSlot(CWeaponComponent * pNewItem) {

	if (!pNewItem)
		return -1;

	for (int i = 0; i < WEAPON_CAPACITY; i++) {

		if (pQuickAccess[i] == pNewItem) {

			return i;

		}
	}

	return -1;
}

int CInventoryComponent::GetQuickAccessSlot(CWeaponComponent * pNewItem) {
	
	if (!pNewItem)
		return -1;

	for (int i = 0; i < WEAPON_CAPACITY; i++) {

		if (pQuickAccess[i] == pNewItem) {

			return i;

		}
	}

	return -1;
}

SItemComponent *CInventoryComponent::GetItem(int slot) {
	
	if (slot < 0)
		return nullptr;

	return pItem[slot];

}

CWeaponComponent *CInventoryComponent::GetQuickAccessItem(int slot) {
	
	if (slot < 0)
		return nullptr;

	return pQuickAccess[slot];

}

bool CInventoryComponent::AddItem(SItemComponent * pNewItem) {

	if (!pNewItem)
		return false;

	for (int i = 0; i < INVENTORY_CAPACITY; i++) {

		if (!pItem[i]) {

			if (pNewItem->GetItemType() == 3) {

				pNewItem->GetEntity()->DetachThis();
				AttachBackpack(pNewItem, i);

				pItem[i] = pNewItem;
				SUIArguments args;
				args.AddArgument<int>(i);
				args.AddArgument<string>(pNewItem->GetItemName());
				args.AddArgument<int>(pNewItem->GetEntityId());
				args.AddArgument<int>(pNewItem->GetItemType());
				args.AddArgument<float>(pNewItem->GetItemWeight());
				pUIInventory->CallFunction("AddItem", args);

			}
			else if (pNewItem->GetItemType() == 5) {
					iHealthPackAmount += 1;
			} else {

				pItem[i] = pNewItem;
				SUIArguments args;
				args.AddArgument<int>(i);
				args.AddArgument<string>(pNewItem->GetItemName());
				args.AddArgument<int>(pNewItem->GetEntityId());
				args.AddArgument<int>(pNewItem->GetItemType());
				args.AddArgument<float>(pNewItem->GetItemWeight());
				pUIInventory->CallFunction("AddItem", args);

			}

			return true;
		}

	}

	return false;

}

bool CInventoryComponent::AddItem(int slot, SItemComponent * pNewItem) {

	if (!pNewItem)
		return false;

	if (!pItem[slot]) {

		pItem[slot] = pNewItem;
		SUIArguments args;
		args.AddArgument<int>(slot);
		args.AddArgument<string>(pNewItem->GetItemName());
		args.AddArgument<int>(pNewItem->GetEntityId());
		args.AddArgument<int>(pNewItem->GetItemType());
		args.AddArgument<float>(pNewItem->GetItemWeight());
		pUIInventory->CallFunction("AddItem", args);
		inventoryEvents.player = m_pEntity->GetComponent<CPlayerComponent>();
		return true;

	}

	return false;

}

bool CInventoryComponent::AddItemQuickAccess(CWeaponComponent * pNewItem) {

	if (!pNewItem)
		return false;

	for (int i = 0; i < WEAPON_CAPACITY; i++) {

		if (!pQuickAccess[i]) {

			pQuickAccess[i] = pNewItem;
			SUIArguments args;
			args.AddArgument<int>(i);
			args.AddArgument<string>(pNewItem->GetItemName());
			args.AddArgument<int>(pNewItem->GetEntityId());
			args.AddArgument<int>(pNewItem->GetItemType());
			args.AddArgument<float>(pNewItem->GetItemWeight());
			pUIInventory->CallFunction("AddWeapon", args);

			return true;
		}

	}

	return false;

}

bool CInventoryComponent::AddItemQuickAccess(int slot, CWeaponComponent * pNewItem) {
	
	if (!pNewItem)
		return false;

		if (!pQuickAccess[slot]) {

			pQuickAccess[slot] = pNewItem;
			SUIArguments args;
			args.AddArgument<int>(slot);
			args.AddArgument<string>(pNewItem->GetItemName());
			args.AddArgument<int>(pNewItem->GetEntityId());
			args.AddArgument<int>(pNewItem->GetItemType());
			args.AddArgument<float>(pNewItem->GetItemWeight());
			pUIInventory->CallFunction("AddWeapon", args);

			return true;
		}

	return false;

}

void CInventoryComponent::RemoveItem(SItemComponent * pNewItem) {

	if (!pNewItem)
		return;

	int index = GetItemSlot(pNewItem);

	if (index > -1) {
	
		pItem[index] = nullptr;

	}

}

void CInventoryComponent::RemoveItem(int slot) {

	if (slot > -1) {

		pItem[slot] = nullptr;

	}

}

void CInventoryComponent::RemoveItemQuickAccess(CWeaponComponent * pNewItem) {

	if (!pNewItem)
		return;

	int index = GetQuickAccessSlot(pNewItem);

	if (index > -1) {

		pQuickAccess[index] = nullptr;

			DetachFromBack(index);

	}

}

void CInventoryComponent::RemoveItemQuickAccess(int slot) {

	if (slot > -1) {

		pQuickAccess[slot] = nullptr;

	}

}

//Attaches a weapon to the back
void CInventoryComponent::AttachToBack(CWeaponComponent *pWeaponToAttach, int slotID) {

	if (!pWeaponToAttach || slotID < 0)
		return;

	string slotString = ToString(slotID);
	CPlayerComponent *player = m_pEntity->GetComponent<CPlayerComponent>();

	CEntityAttachment *attachmentItem = new CEntityAttachment();
	attachmentItem->SetEntityId(pWeaponToAttach->GetEntityId());

	if (ICharacterInstance *pCharacter = player->GetAnimations()->GetCharacter()) {
	
		if (IAttachmentManager *pAttMan = pCharacter->GetIAttachmentManager()) {

			string attName = "back_att_0" + slotString;
			if (IAttachment *pAttachment = pAttMan->GetInterfaceByName(attName)) {

				pAttachment->AddBinding(attachmentItem);

			}

		}

	}

}

//Detaches an attached weapon from back
void CInventoryComponent::DetachFromBack(int slotID) {

	if (slotID < 0)
		return;

	string slotString = ToString(slotID);
	CPlayerComponent *player = m_pEntity->GetComponent<CPlayerComponent>();

	if (ICharacterInstance *pCharacter = player->GetAnimations()->GetCharacter()) {

		if (IAttachmentManager *pAttMan = pCharacter->GetIAttachmentManager()) {

			string attName = "back_att_0" + slotString;
			if (IAttachment *pAttachment = pAttMan->GetInterfaceByName(attName)) {

				pAttachment->ClearBinding();

			}

		}

	}

}

void CInventoryComponent::Attach(CWeaponComponent *pWeaponToAttach) {

	for (int i = 0; i < WEAPON_CAPACITY; i++) {

			pWeaponToAttach->GetEntity()->DetachThis();
			AttachToBack(pWeaponToAttach, i);

	}

}

void CInventoryComponent::AttachBackpack(SItemComponent * pItemToAttach, int slotID) {

	if (!pItemToAttach || slotID < 0)
		return;

	string slotString = ToString(slotID);
	CPlayerComponent *player = m_pEntity->GetComponent<CPlayerComponent>();

	CEntityAttachment *attachmentItem = new CEntityAttachment();
	attachmentItem->SetEntityId(pItemToAttach->GetEntityId());

	if (ICharacterInstance *pCharacter = player->GetAnimations()->GetCharacter()) {

		if (IAttachmentManager *pAttMan = pCharacter->GetIAttachmentManager()) {

			string attName = "backpackAttachment0" + slotString;
			if (IAttachment *pAttachment = pAttMan->GetInterfaceByName(attName)) {

				pAttachment->AddBinding(attachmentItem);
				pItemToAttach->GetEntity()->EnablePhysics(false);
			}

		}

	}


}

void CInventoryComponent::DetachBackpackBack(int slotID) {

	if (slotID < 0)
		return;

	string slotString = ToString(slotID);
	CPlayerComponent *player = m_pEntity->GetComponent<CPlayerComponent>();

	if (ICharacterInstance *pCharacter = player->GetAnimations()->GetCharacter()) {

		if (IAttachmentManager *pAttMan = pCharacter->GetIAttachmentManager()) {

			string attName = "backpackAttachment0" + slotString;
			if (IAttachment *pAttachment = pAttMan->GetInterfaceByName(attName)) {

				pAttachment->ClearBinding();

			}

		}

	}

}

void CInventoryComponent::AttachToHand(CWeaponComponent * pItemToAttach) {

	if (!pItemToAttach)
		return;

	CPlayerComponent *player = m_pEntity->GetComponent<CPlayerComponent>();
	CEntityAttachment *attachmentItem = new CEntityAttachment();
	attachmentItem->SetEntityId(pItemToAttach->GetEntityId());

	if (ICharacterInstance *pCharacter = player->GetAnimations()->GetCharacter()) {

		if (IAttachmentManager *pAttMan = pCharacter->GetIAttachmentManager()) {

				string attName = "meeleWeapon";
				if (IAttachment *pAttachment = pAttMan->GetInterfaceByName(attName)) {

					pAttachment->AddBinding(attachmentItem);

				}

		}

	}

}

void CInventoryComponent::DetachFromHand() {

	CPlayerComponent *player = m_pEntity->GetComponent<CPlayerComponent>();
	if (ICharacterInstance *pCharacter = player->GetAnimations()->GetCharacter()) {

		if (IAttachmentManager *pAttMan = pCharacter->GetIAttachmentManager()) {

			string attName = "meeleWeapon";
			if (IAttachment *pAttachment = pAttMan->GetInterfaceByName(attName)) {

				pAttachment->ClearBinding();

			}


		}

	}

}

void CInventoryComponent::SelectSlot(int slotId) {

	if (slotId < 0)
		return;

	CWeaponComponent *pSavedSelectedItem = pSelectedWeapon;
	DeselectItem();

	if (pSavedSelectedItem != pQuickAccess[slotId])
		SelectItem(pQuickAccess[slotId]);

}

void CInventoryComponent::SetQuickAccessItemSlot(CWeaponComponent * pItemToMove, int slotId) {}

void CInventoryComponent::SelectItem(CWeaponComponent * pItemToSelect) {

	if (!pItemToSelect)
		return;

	DetachFromBack(GetQuickAccessSlot(pItemToSelect));

	pSelectedWeapon = pItemToSelect;

	AttachToHand(pItemToSelect);

}

void CInventoryComponent::DeselectItem() {

	if (!pSelectedWeapon)
		return;

	DetachFromHand(); 

	AttachToBack(pSelectedWeapon, GetWeaponSlot(pSelectedWeapon));

	pSelectedWeapon = nullptr;

}

//UI Inventory

void CInventoryComponent::Show() {

//	string m_LocalUsername = SteamFriends()->GetPersonaName();

	m_pEntity->GetComponent<CPlayerComponent>()->FreezePlayer(bIsInventoryOpened);
	SUIArguments args;
	args.AddArgument<bool>(bIsInventoryOpened);
	pUIInventory->CallFunction("Show", args);

	//args.AddArgument<string>(m_LocalUsername);
	//pUIInventory->CallFunction("SetPlayerName", args);
	if (bIsInventoryOpened) {
		pUIInventory->AddEventListener(&inventoryEvents, "InventorySystem");
		pManager->StartAction(pInventoryShowCursor, "InventorySystem");
	} else {
		pUIInventory->RemoveEventListener(&inventoryEvents);
		pManager->StartAction(pInventoryShow, "InventorySystem");
		pManager->StartAction(pInventoryHideCursor, "InventorySystem");
	}
}
