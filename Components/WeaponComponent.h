/*-------------------------------------------------------------------------------

Author : Ivar Jönsson
Project : BR
Purpose : Handels all the weapon logic

-------------------------------------------------------------------------------*/

#pragma once

#include <CryEntitySystem/IEntityComponent.h>

class CWeaponComponent : public IEntityComponent {

	struct SWeaponProperties {

		inline bool operator==(const SWeaponProperties& rhs) const { return 0 == memcmp(this, &rhs, sizeof(rhs)); }
		inline bool operator!=(const SWeaponProperties& rhs) const { return 0 != memcmp(this, &rhs, sizeof(rhs)); }



		static void ReflectType(Schematyc::CTypeDesc<SWeaponProperties>& desc) {

			desc.SetGUID("{1D6890C0-82ED-4CA6-8BCE-DC72F79D704B}"_cry_guid);
			desc.SetLabel("Weapon Properties");
			desc.SetDescription("Weapon Properties");

		}

	};

public:
	CWeaponComponent() = default;

	virtual void Initialize() override;
	virtual uint64 GetEventMask() const override;
	virtual void ProcessEvent(SEntityEvent& event) override;
	static void ReflectType(Schematyc::CTypeDesc<CWeaponComponent>& desc);

};