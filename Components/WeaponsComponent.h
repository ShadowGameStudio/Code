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
	virtual uint64 GetEventMask() const override;
	virtual void ProcessEvent(SEntityEvent& event) override;
	static void ReflectType(Schematyc::CTypeDesc<CWeaponsComponent>& desc);
	float GetDamage() const { return sWeaponProperties.fDamage; }
	float GetBulletDrop() const { return sWeaponProperties.fBulletDrop; }
	int GetMaxAmmo() const { return sWeaponProperties.iMaxAmmo; }
	int GetClipAmmo() const { return sWeaponProperties.iClipAmmo; }
	Schematyc::CSharedString GetAmmoType() const { return sWeaponProperties.sAmmoType; }
	SWeaponProperties* GetWeaponProperties() { return &sWeaponProperties; }

protected:

	SWeaponProperties sWeaponProperties;

};