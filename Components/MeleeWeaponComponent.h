/*-------------------------------------------------------------------------------

Author : Ivar Jönsson
Project : BR
Purpose : Defines all the melee weapons.

-------------------------------------------------------------------------------*/

#pragma once

#include "StdAfx.h"
#include <CryEntitySystem/IEntityComponent.h>

#include "ItemComponent.h"
#include "MeleeWeaponProperties.h"

class CHealthComponent;

class CMeleeWeaponComponent : public SItemComponent {

	enum TIMER_EVENT {
		Timer_Attack = 0
	};

public:
	CMeleeWeaponComponent() = default;
//	CMeleeWeaponComponent::~CMeleeWeaponComponent();

	virtual void InitializeClass() override;
	virtual void ProcessEventClass(SEntityEvent& event) override;
	static void ReflectType(Schematyc::CTypeDesc<CMeleeWeaponComponent>& desc);

	void StartAttack();
	void StopAttack();

private:

	SMeeleWeaponProperties props;
	bool bIsAttacking = false;

};