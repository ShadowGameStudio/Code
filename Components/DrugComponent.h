/*-------------------------------------------------------------------------------

Author : Ivar Jönsson
Project : BR
Purpose : Handels all the drugs funcitonality

-------------------------------------------------------------------------------*/

#pragma once

#include <CryEntitySystem/IEntityComponent.h>
#include "ItemComponent.h"

class CDrugComponent : public SItemComponent {

	enum Timer {

		Timer_Consume

	};

	struct SDrugProperties {

		inline bool operator==(const SDrugProperties& rhs) const { return 0 == memcmp(this, &rhs, sizeof(rhs)); }
		inline bool operator!=(const SDrugProperties& rhs) const { return 0 != memcmp(this, &rhs, sizeof(rhs)); }

		int consumeTimer;
		float fRegenerateRatio;

		static void ReflectType(Schematyc::CTypeDesc<SDrugProperties>& desc) {

			desc.SetGUID("{5E855CA6-F32B-46D9-90C2-DBF273AAE9E5}"_cry_guid);
			desc.SetLabel("Food Properties");
			desc.SetDescription("Food Properties");
			desc.AddMember(&SDrugProperties::consumeTimer, 'cst', "ConsumeTimer", "Consume Timer", "Sets the time it takes to consume", 0);
			desc.AddMember(&SDrugProperties::fRegenerateRatio, 'frr', "RegenerateRatio", "Regenerate Ratio", "Sets the regeneration ratio when this item is consumed", 0.f);

		}

	};

public:
	CDrugComponent() = default;

	virtual void InitializeClass() override;
	virtual void ProcessEventClass(SEntityEvent& event) override;
	static void ReflectType(Schematyc::CTypeDesc<CDrugComponent>& desc);

	void Consume(SItemComponent *pItemToConsume);

	SDrugProperties *GetDrugProperties() { return &sDrugProperties; }

protected:

	SDrugProperties sDrugProperties;

};