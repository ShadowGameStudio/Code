/*-------------------------------------------------------------------------------

Author : Ivar J�nsson
Project : BR
Purpose : Base structure for items. All types of items will derive from here.

-------------------------------------------------------------------------------*/

#pragma once

#include <CryEntitySystem/IEntityComponent.h>
#include "ItemProperties.h"

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
	virtual float GetItemDamage() { return GetProperties()->fItemDamage; }

	virtual void PickUp(IEntity *pNewOwner);
	virtual void Drop();
	virtual bool IsPickable();

protected:
	SItemProperties sItemProperties, sPrevItemProperties;

	IEntity *pOwnerEntity = nullptr;

	int iChildConstraintId = 0;
	int iOwnerConstraintId = 0;

};