/*-------------------------------------------------------------------------------

Author : Ivar Jönsson
Project : BR
Purpose : Weapons component, handles all the games weapons and new weapons derive from here.

-------------------------------------------------------------------------------*/

#pragma once

#include "StdAfx.h"
#include <CryEntitySystem/IEntityComponent.h>

#include "ItemComponent.h"
#include "WeaponProperties.h"

class CWeaponsComponent : public SItemComponent {
public:
	CWeaponsComponent() = default;
	CWeaponsComponent::~CWeaponsComponent();

	virtual void InitializeClass() override;
	virtual uint64 GetEventMask() const override;
	virtual void ProcessEvent(SEntityEvent& event) override;
	static void ReflectType(Schematyc::CTypeDesc<CWeaponsComponent>& desc);
};