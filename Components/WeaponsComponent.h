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
#include "Bullet.h"

class CWeaponsComponent : public SItemComponent {
public:
	CWeaponsComponent() = default;

	virtual void InitializeClass() override;
	virtual void ProcessEventClass(SEntityEvent& event) override;
	static void ReflectType(Schematyc::CTypeDesc<CWeaponsComponent>& desc);
	float GetBulletDrop() { return GetWeaponProperties()->fBulletDrop; }
	int GetMaxAmmo() { return GetWeaponProperties()->iMaxAmmo; }
	int GetClipAmmo() { return GetWeaponProperties()->iClipAmmo; }
	Schematyc::CSharedString GetAmmoType() { return GetWeaponProperties()->sAmmoType; }
	SWeaponProperties* GetWeaponProperties() { return &sWeaponProperties; }

protected:

	SWeaponProperties sWeaponProperties;

};