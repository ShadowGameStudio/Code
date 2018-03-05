/*-------------------------------------------------------------------------------

Author : Ivar Jönsson
Project : BR
Purpose : Handels all the weapon logic

-------------------------------------------------------------------------------*/

#pragma once

#include <CryEntitySystem/IEntityComponent.h>
#include "ItemComponent.h"
#include "CryNetwork\Rmi.h"

struct NoParams {
	void SerializeWith(TSerialize ser) {}
};

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
			desc.AddMember(&SWeaponProperties::sBulletType, 'sbut', "BulletType", "Bullet Type", "Sets the bullet type", "");

		}

	};

	struct SFiremodeProperties {

		inline bool operator==(const SFiremodeProperties& rhs) const { return 0 == memcmp(this, &rhs, sizeof(rhs)); }
		inline bool operator!=(const SFiremodeProperties& rhs) const { return 0 != memcmp(this, &rhs, sizeof(rhs)); }

		bool bIsAuto;
		int iFireRate;
		bool bIsSemi;
		bool bIsSingle;

		static void ReflectType(Schematyc::CTypeDesc<SFiremodeProperties>& desc) {

			desc.SetGUID("{7F766352-CC30-43B3-B606-05F2C9201A0A}"_cry_guid);
			desc.SetLabel("Firemode Properties");
			desc.SetDescription("The different properties for firemodes(not used if IsMeele). You can choose multiple");
			desc.AddMember(&SFiremodeProperties::bIsAuto, 'bisa', "IsAutomatic", "Is Automatic", "Check if weapon should have automatic fire", false);
			desc.AddMember(&SFiremodeProperties::iFireRate, 'ifra', "FireRate", "Fire Rate", "Input at what rate you want the weapon to fire", 0);
			desc.AddMember(&SFiremodeProperties::bIsSemi, 'biss', "IsSemi", "Is Semi-Automatic", "Check if weapon should have semi-automatic fire", false);
			desc.AddMember(&SFiremodeProperties::bIsSingle, 'bis', "IsSingle", "Is Single", "Check if weapon should have single fire", false);

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
	int GetCurrAmmo() { return iCurrAmmo;  }
	float GetDamage() { return GetWeaponProperties()->fDamage; }

	//Non-meele weapon specific
	void Reload();
	void SetPlayer(CPlayerComponent *pPlayer);
	void Shoot(){ SRmi<RMI_WRAP(&CWeaponComponent::SvShoot)>::InvokeOnServer(this, NoParams{}); }

	//Network
	//Client
	bool ClShoot(NoParams&& p, INetChannel *);
	//Server
	bool SvShoot(NoParams&& p, INetChannel *) { 
		SRmi<RMI_WRAP(&CWeaponComponent::ClShoot)>::InvokeOnAllClients(this, NoParams{});
		return true; 
	}

	//Meele weapon specific
	void StartAttack();
	void StopAttack();

	SWeaponProperties *GetWeaponProperties() { return &sWeaponProperties; }
	SFiremodeProperties *GetFiremodeProperties() { return &sFiremodeProperties; }

private:
	bool bIsAttacking = false;
	int iCurrAmmo = GetWeaponProperties()->iClipAmmo;


protected:
	SWeaponProperties sWeaponProperties;
	SFiremodeProperties sFiremodeProperties;
	CPlayerComponent *pPlayerShooting = nullptr;

};