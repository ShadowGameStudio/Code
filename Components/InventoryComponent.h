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
#include "WeaponComponent.h"

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
	int GetWeaponSlot(CWeaponComponent *pNewItem);
	int GetQuickAccessSlot(CWeaponComponent *pNewItem);
	SItemComponent *GetItem(int slot);
	CWeaponComponent *GetQuickAccessItem(int slot);

	bool AddItem(SItemComponent *pNewItem);
	bool AddItem(int slot, SItemComponent *pNewItem);
	bool AddItemQuickAccess(CWeaponComponent *pNewItem);
	bool AddItemQuickAccess(int slot, CWeaponComponent *pNewItem);

	void RemoveItem(SItemComponent *pNewItem);
	void RemoveItem(int slot);
	void RemoveItemQuickAccess(CWeaponComponent *pNewItem);
	void RemoveItemQuickAccess(int slot);

	void AttachToBack(CWeaponComponent *pWeaponToAttach, int slotID);
	void DetachFromBack(int slotID);
	void Attach(CWeaponComponent *pWeaponToAttach);

	void AttachBackpack(SItemComponent *pItemToAttach, int slotID);
	void DetachBackpackBack(int slotID);
	void AttachToHand(CWeaponComponent *pItemToAttach);
	void DetachFromHand();

	void SelectSlot(int slotId);
	void SetQuickAccessItemSlot(CWeaponComponent *pItemToMove, int slotId);

	CWeaponComponent *GetSelectedItem() { return pSelectedWeapon; }

	void Show();

protected:

	void SelectItem(CWeaponComponent *pItemToSelect);
	void DeselectItem();

private:

	SItemComponent *pItem[INVENTORY_CAPACITY] = { nullptr };
	CWeaponComponent *pQuickAccess[WEAPON_CAPACITY] = { nullptr };

	//UI Stuff

	IUIElement *pUIInventory;
	IUIAction *pInventoryShow;
	IUIAction *pInventoryHide;
	IUIActionManager *pManager;
	IUIAction *pInventoryShowCursor;
	IUIAction *pInventoryHideCursor;

	//Item selection

	CWeaponComponent *pSelectedWeapon = nullptr;

public:

	bool bIsInventoryOpened = false;
	int iHealthPackAmount = 0;
};