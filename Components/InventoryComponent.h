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

#define WEAPON_CAPACITY 2
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

	int GetWeaponSlot(SItemComponent *pNewWeapon);

	bool AddItem(SItemComponent *pNewItem);

	void RemoveItem(SItemComponent *pNewItem);
	void RemoveWeapon(SItemComponent *pNewItem);

	void AttachToBack(SItemComponent *pWeaponToAttach, int slotId);
	void DetachFromBack(int slotId);
	void Attach(SItemComponent *pWeaponToAttach);

	void AttachToHand(SItemComponent *pWeaponToAttach);
	void DetachFromHand();

	void SelectSlot(int slotId);
	SItemComponent *GetSelectedWeapon() { return pSelectedWeapon; }

	void Show();

protected:

	void SelectWeapon(SItemComponent *pWeaponToSelect);
	void DeselectWeapon();

private:

	float fInventoryCapKilo = 100.f;
	float fCurrentWeight = 0.f;

	SItemComponent *pWeapon[WEAPON_CAPACITY] = { nullptr };

	//UI Stuff

	IUIElement *pUIInventory;
	IUIAction *pInventoryShow;
	IUIAction *pInventoryHide;
	IUIActionManager *pManager;
	IUIAction *pInventoryShowCursor;
	IUIAction *pInventoryHideCursor;

	//Item selection

	SItemComponent *pSelectedWeapon = nullptr;

public:

	bool bIsInventoryOpened = false;
	int iHealthPackAmount = 0;
};