/*-------------------------------------------------------------------------------

Author : Ivar Jönsson
Project : 
Purpose : 

-------------------------------------------------------------------------------*/

#pragma once

#include "StdAfx.h"
#include <CryEntitySystem/IEntityComponent.h>

#include "DomeComponent.h"
#include "ItemComponent.h"

class CDomeComponent : public SDomeComponent {
public:
	CDomeComponent() = default;
	//	CMeleeWeaponComponent::~CMeleeWeaponComponent();

	virtual void InitializeClass() override;
	virtual void ProcessEventClass(SEntityEvent& event) override;
	static void ReflectType(Schematyc::CTypeDesc<CDomeComponent>& desc);
};