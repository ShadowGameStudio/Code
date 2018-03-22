/*-------------------------------------------------------------------------------

Author : Ivar J�nsson
Project : BR
Purpose : Inventory System will allow the player to store items.

-------------------------------------------------------------------------------*/

#pragma once

#include "StdAfx.h"
#include <CryEntitySystem/IEntityComponent.h>
#include "FlashUI\FlashUI.h"

#define WEAPON_CAPACITY 2
#define HEALTH_PACK_CAPACITY 5
#define GASMASK_CAPACITY 1

struct SItemComponent;
struct IUIElement;
struct IUIAction;
struct IUIActionManager;

class CInventoryComponent : public IEntityComponent {

	struct SAddItemParams {

		EntityId Id;
		uint16 playerChannelId;

		void SerializeWith(TSerialize ser) {
			ser.Value("EntityId", Id, 'eid');
			ser.Value("PlayerChannelId", playerChannelId, 'ui16');
		}
	};

public:
	CInventoryComponent() = default;
	//CInventoryComponent::~CInventoryComponent();

	virtual void Initialize() override;
	virtual uint64 GetEventMask() const override;
	virtual void ProcessEvent(const SEntityEvent& event) override;

	//Inventory System

	int GetWeaponSlot(SItemComponent *pNewWeapon);

	bool AddItem(SItemComponent *pNewItem);

	void RemoveItem(SItemComponent *pNewItem);
	void RemoveWeapon(SItemComponent *pNewItem);

	void AttachToBack(SItemComponent *pItemToAttach, int slotId);
	void DetachFromBack(int slotId);
	void Attach(SItemComponent *pWeaponToAttach);

	void AttachToHand(SItemComponent *pWeaponToAttach);
	void AttachToFace(SItemComponent *pItemToAttach);
	void DetachFromHand();

	void SelectSlot(int slotId);
	void RemoveWeapon(int slotId) { 
		//Empty the slot
		pWeapon[slotId] = nullptr;
		//Set selected weapon to null
		pSelectedWeapon = nullptr;
		pLastSelectedWeapon = nullptr;
	}
	bool ActivateBackpack(SItemComponent *pBackpack);

	SItemComponent *GetSelectedWeapon() { return pSelectedWeapon; }
	SItemComponent *GetGasmask() { return pCurrentGasmask; }

	void Show();

	//Network
	bool ServerAddItem(SAddItemParams&& p, INetChannel *pNetChannel);
	bool ClientAddItem(SAddItemParams&& p, INetChannel *pNetChannel);
	bool RequestAddItem(SItemComponent *pNewItem);
	//Network

protected:

	void SelectWeapon(SItemComponent *pWeaponToSelect);
	void DeselectWeapon();

private:

	float m_fInventoryCapKilo = 100.f;
	float m_fCurrentWeight = 0.f;

	SItemComponent *pWeapon[WEAPON_CAPACITY] = { nullptr };
	SItemComponent *pGasmask[GASMASK_CAPACITY] = { nullptr };
	SItemComponent *pBackpack = nullptr;

	//UI Stuff

	IUIElement *pUIInventory;
	IUIAction *pInventoryShow;
	IUIAction *pInventoryHide;
	IUIAction *pInventoryShowCursor;
	IUIAction *pInventoryHideCursor;
	IUIAction *pShowCrosshair;
	IUIActionManager *pManager;

	//Item selection

	SItemComponent *pCurrentGasmask = nullptr;
	SItemComponent *pSelectedWeapon = nullptr;
	SItemComponent *pLastSelectedWeapon = nullptr;

public:

	bool bIsInventoryOpened = false;
	int iHealthPackAmount = 0;
};