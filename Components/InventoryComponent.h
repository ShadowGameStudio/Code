/*-------------------------------------------------------------------------------

Author : Ivar Jönsson
Project : BR
Purpose : Inventory System will allow the player to store items.

-------------------------------------------------------------------------------*/

#pragma once

#include "StdAfx.h"
#include <CryEntitySystem/IEntityComponent.h>
#include "steam\steam_api.h"
#include "FlashUI\FlashUI.h"

#define INVENTORY_CAPACITY 20
#define WEAPON_CAPACITY 2
#define INVENTORY_CAPACITY_KILOS 100
#define HEALTH_PACK_CAPACITY 5

struct SItemComponent;
struct IUIElement;
struct IUIAction;
struct IUIActionManager;

class CInventoryComponent : public IEntityComponent {
public:
	CInventoryComponent() = default;
	//CInventoryComponent::~CInventoryComponent();

	virtual void Initialize() override;
	virtual uint64 GetEventMask() const override;
	virtual void ProcessEvent(SEntityEvent& event) override;

	//Inventory System

	int GetItemSlot(SItemComponent *pNewItem);
	int GetQuickAccessSlot(SItemComponent *pNewItem);
	SItemComponent *GetItem(int slot);
	SItemComponent *GetQuickAccessItem(int slot);

	bool AddItem(SItemComponent *pNewItem);
	bool AddItem(int slot, SItemComponent *pNewItem);
	bool AddItemQuickAccess(SItemComponent *pNewItem);
	bool AddItemQuickAccess(int slot, SItemComponent *pNewItem);

	void RemoveItem(SItemComponent *pNewItem);
	void RemoveItem(int slot);
	void RemoveItemQuickAccess(SItemComponent *pNewItem);
	void RemoveItemQuickAccess(int slot);

	void AttachToBack(SItemComponent *pItemToAttach, int slotID);
	void DetachFromBack(int slotID);
	void Attach(SItemComponent *pWeaponToAttach);

	void AttachBackpack(SItemComponent *pItemToAttach, int slotID);
	void DetachBackpackBack(int slotID);
	void AttachToHand(SItemComponent *pItemToAttach);
	void DetachFromHand();

	void SelectSlot(int slotId);
	void SetItemSlot(SItemComponent *pItemToMove, int slotId);
	void SetQuickAccessItemSlot(SItemComponent *pItemToMove, int slotId);

	SItemComponent *GetSelectedItem() { 
			return pSelectedItem; 
	}

	void Show();

protected:

	void SelectItem(SItemComponent *pItemToSelect);
	void DeselectItem();

private:

	SItemComponent *pItem[INVENTORY_CAPACITY] = { nullptr };
	SItemComponent *pQuickAccess[WEAPON_CAPACITY] = { nullptr };

	//UI Stuff

	IUIElement *pUIInventory;
	IUIAction *pInventoryShow;
	IUIAction *pInventoryHide;
	IUIActionManager *pManager;
	IUIAction *pInventoryShowCursor;
	IUIAction *pInventoryHideCursor;

	//Item selection

	SItemComponent *pSelectedItem = nullptr;

public:

	bool bIsInventoryOpened = false;
	int iHealthPackAmount = 0;
};