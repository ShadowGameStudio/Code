/*-------------------------------------------------------------------------------

Author : Ivar Jönsson
Project : BR
Purpose : Handels the backpack stuff

-------------------------------------------------------------------------------*/

#pragma once

#include <CryEntitySystem/IEntityComponent.h>
#include "ItemComponent.h"

class CBackpackComponent : public SItemComponent {
public:
	CBackpackComponent() = default;
	//	CMeleeWeaponComponent::~CMeleeWeaponComponent();

	virtual void InitializeClass() override;
	virtual void ProcessEventClass(SEntityEvent& event) override;
	static void ReflectType(Schematyc::CTypeDesc<CBackpackComponent>& desc);
};