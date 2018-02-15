#pragma once

#include "StdAfx.h"

struct SWeaponProperties {

	inline bool operator==(const SWeaponProperties& rhs) const { return 0 == memcmp(this, &rhs, sizeof(rhs)); }
	inline bool operator!=(const SWeaponProperties& rhs) const { return 0 != memcmp(this, &rhs, sizeof(rhs)); }

	int iClipAmmo = 0;
	int iMaxAmmo = 0;
	
	Schematyc::CSharedString sAmmoType;

	float fDamage = 0.f;
	float fBulletDrop = 0.f;

};

static void ReflectType(Schematyc::CTypeDesc<SWeaponProperties>& desc) {

	desc.SetGUID("{E7B95B8E-C7D0-4AD0-9947-6EDB164C55BD}"_cry_guid);
	desc.SetLabel("Weapon Properites");
	desc.SetDescription("Sets the different properties for the weapon");
	desc.AddMember(&SWeaponProperties::iClipAmmo, 'icam', "ClipAmmo", "Clip Ammo", "Sets the ammo in every magazine", 0);
	desc.AddMember(&SWeaponProperties::iMaxAmmo, 'imam', "MaxAmmo", "Max Ammo", "Sets the maximum amount of ammo you can have for this weapon", 0);
	desc.AddMember(&SWeaponProperties::fBulletDrop, 'fbdp', "BulletDrop", "Bullet Drop", "Sets the bullet drop for the item", 0.f);
	desc.AddMember(&SWeaponProperties::fDamage, 'fdam', "Damage", "Damage", "Sets the amount of damage done by weapon", 0.f);
	desc.AddMember(&SWeaponProperties::sAmmoType, 'saty', "AmmotType", "Ammo Type", "Sets the ammo type that the weapon will be using", "");

}