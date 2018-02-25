/*-------------------------------------------------------------------------------

Author : Ivar J�nsson
Project : BR
Purpose : Base structure for items. All types of items will derive from here.

-------------------------------------------------------------------------------*/

#pragma once

#include <CryEntitySystem/IEntityComponent.h>
#include "ItemProperties.h"
#include "CryNetwork\Rmi.h"

struct PickUpParams {
	EntityId playerId;
	void SerializeWith(TSerialize ser) {
		ser.Value("itemId", playerId, 'eid');
	}
};

struct SItemComponent : public IEntityComponent {
public:
	SItemComponent() = default;
//	SItemComponent::~SItemComponent();

	virtual void Initialize() override;
	virtual void InitializeClass() = 0;
	virtual uint64 GetEventMask() const override;
	virtual void ProcessEvent(SEntityEvent& event) override;
	virtual void ProcessEventClass(SEntityEvent& event) = 0;
	static void ReflectType(Schematyc::CTypeDesc<SItemComponent>& desc);
	//geometry and physics
	virtual void LoadGeometry();
	virtual void Physicalize();

	virtual SItemProperties *GetProperties() { return &sItemProperties; }
	virtual void CreateItemName();
	virtual string GetItemName() { return GetProperties()->sItemName;  }

	virtual int GetItemType() { return GetProperties()->itemType; }
	virtual float GetItemWeight() { return GetProperties()->itemWeight; }

	virtual void PickUp(EntityId id) { SRmi<RMI_WRAP(&SItemComponent::SvPickUp)>::InvokeOnServer(this, PickUpParams{ id }); }
	virtual void Drop();
	virtual bool IsPickable();

	//Network
	//RMI
	//Client
	bool ClPickUp(PickUpParams&& p, INetChannel *);
	//Server
	bool SvPickUp(PickUpParams&& p, INetChannel *) {
		SRmi<RMI_WRAP(&SItemComponent::ClPickUp)>::InvokeOnAllClients(this, PickUpParams{ p.playerId });
		return true;
	}
	//

protected:
	SItemProperties sItemProperties, sPrevItemProperties;

	//Item owners, after and before pickup
	IEntity *pOwnerEntity = nullptr;
	IEntity *pNewOwner = nullptr;

	EntityId pickUpItemId;

	int iChildConstraintId = 0;
	int iOwnerConstraintId = 0;

};