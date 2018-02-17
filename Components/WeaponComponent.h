/*-------------------------------------------------------------------------------

Author : Ivar Jönsson
Project : BR
Purpose : Handels all the weapon logic

-------------------------------------------------------------------------------*/

#pragma once

#include <CryEntitySystem/IEntityComponent.h>
#include "ItemComponent.h"

class CWeaponComponent : public SItemComponent {

	struct SWeaponProperties {

		inline bool operator==(const SWeaponProperties& rhs) const { return 0 == memcmp(this, &rhs, sizeof(rhs)); }
		inline bool operator!=(const SWeaponProperties& rhs) const { return 0 != memcmp(this, &rhs, sizeof(rhs)); }

		bool bIsMeele;
		int iMaxAmmo;
		int iClipAmmo;
		float fDamage;
		Schematyc::CSharedString sBulletType;

		static void ReflectType(Schematyc::CTypeDesc<SWeaponProperties>& desc) {

			desc.SetGUID("{C72E995C-D55B-4656-A659-AFC4B1A5EC90}"_cry_guid);
			desc.SetLabel("Weapon Properties");
			desc.SetDescription("All the weapon specific properties");
			desc.AddMember(&SWeaponProperties::bIsMeele, 'bism', "IsMeele", "Is Meele", "Check if weapon is meele", false);
			desc.AddMember(&SWeaponProperties::fDamage, 'fdam', "Damage", "Damage", "Sets the damage done by the weapon", 0.f);
			desc.AddMember(&SWeaponProperties::iClipAmmo, 'icla', "ClipAmmo", "Clip Ammo", "Sets the amount of ammo in a magazine", 0);
			desc.AddMember(&SWeaponProperties::iMaxAmmo, 'imax', "MaxAmmo", "Max Ammo", "Max amount of ammo you can carry for this weapon", 0);
			desc.AddMember(&SWeaponProperties::sBulletType, 'sbut', "BulletType", "Bullet Type", "Sets the bullet type", "");

		}

	};

	enum TIMER_EVENT {
		Timer_Attack = 0
	};

public:
	CWeaponComponent() = default;

	virtual void InitializeClass() override;
	virtual void ProcessEventClass(SEntityEvent& event) override;
	static void ReflectType(Schematyc::CTypeDesc<CWeaponComponent>& desc);

	bool GetIsMeele() { return GetWeaponProperties()->bIsMeele; }
	int GetMaxAmmo() { return GetWeaponProperties()->iMaxAmmo; }
	int GetClipAmmo() { return GetWeaponProperties()->iClipAmmo; }
	float GetDamage() { return GetWeaponProperties()->fDamage; }

	//Non-meele weapon specific
	void Shoot();

	//Meele weapon specific
	void StartAttack();
	void StopAttack();

	SWeaponProperties *GetWeaponProperties() { return &sWeaponProperties; }

private:
	bool bIsAttacking = false;

protected:
	SWeaponProperties sWeaponProperties;

};