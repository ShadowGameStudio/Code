/*-------------------------------------------------------------------------------

Author : Ivar Jönsson
Project : BR
Purpose : Handels all the gasmask functionality

-------------------------------------------------------------------------------*/

#pragma once

#include <CryEntitySystem/IEntityComponent.h>
#include "ItemComponent.h"

class CGasmaskComponent final : public SItemComponent {

	struct SGasmaskProperties {

		inline bool operator==(const SGasmaskProperties& rhs) const { return 0 == memcmp(this, &rhs, sizeof(rhs)); }
		inline bool operator!=(const SGasmaskProperties& rhs) const { return 0 != memcmp(this, &rhs, sizeof(rhs)); }

		int iGasmaskLevel;

		static void ReflectType(Schematyc::CTypeDesc<SGasmaskProperties>& desc) {

			desc.SetGUID("{E614F7A0-E1F3-4893-9365-41289DB5C015}"_cry_guid);
			desc.SetLabel("Infected Area Properties");
			desc.SetDescription("The different properties for the infected area");
			desc.AddMember(&SGasmaskProperties::iGasmaskLevel, 'igml', "GasmaskLevel", "Gasmask Level", "Sets the gasmasks level", 0);

		}

	};

public:
	CGasmaskComponent() = default;

	virtual void InitializeClass() override;
	virtual void ProcessEventClass(SEntityEvent& event) override;
	static void ReflectType(Schematyc::CTypeDesc<CGasmaskComponent>& desc);

	SGasmaskProperties *GetGasmaskProperties() { return &sGasmaskProperties; }

protected:

	SGasmaskProperties sGasmaskProperties;

};