/*-------------------------------------------------------------------------------

Author : Ivar Jönsson
Project : BR
Purpose : The base Healthpack component

-------------------------------------------------------------------------------*/

#pragma once

#include <CryEntitySystem/IEntityComponent.h>
#include "ItemComponent.h"
#include "HealthComponent.h"

class CHealthpackComponent : public SItemComponent {

	struct SHealthpackProperties {

		inline bool operator==(const SHealthpackProperties& rhs) const { return 0 == memcmp(this, &rhs, sizeof(rhs)); }
		inline bool operator!=(const SHealthpackProperties& rhs) const { return 0 != memcmp(this, &rhs, sizeof(rhs)); }

		float fHealthAddAmount = 0.f;

		static void ReflectType(Schematyc::CTypeDesc<SHealthpackProperties>& desc) {

			desc.SetGUID("{F14C28AC-9C45-4D88-90E9-8E43BB822D45}"_cry_guid);
			desc.SetLabel("Healthpack Properties");
			desc.SetDescription("Healthpack Properties");
			desc.AddMember(&SHealthpackProperties::fHealthAddAmount, 'fhad', "HealthAddAmount", "Health Add Amount", "Health Add Amount", 0.f);

		}

	};

public:
	CHealthpackComponent() = default;

	virtual void InitializeClass() override;
	virtual void ProcessEventClass(const SEntityEvent& event) override;
	static void ReflectType(Schematyc::CTypeDesc<CHealthpackComponent>& desc);

	CHealthComponent HP;

	void AddHealth() { HP.Add(10.f); }

private:

	SHealthpackProperties props;

};