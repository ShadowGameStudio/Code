/*-------------------------------------------------------------------------------

Author : Ivar Jönsson
Project : BR
Purpose : The component that handels the infected areas in houses

-------------------------------------------------------------------------------*/

#pragma once

#include <CryEntitySystem/IEntityComponent.h>

class CInfectedAreaComponent final : public IEntityComponent {

	struct SInfectedAreaProperties {

		inline bool operator==(const SInfectedAreaProperties& rhs) const { return 0 == memcmp(this, &rhs, sizeof(rhs)); }
		inline bool operator!=(const SInfectedAreaProperties& rhs) const { return 0 != memcmp(this, &rhs, sizeof(rhs)); }

		int iGasmaskLevel;
		float fInfectedAreaDamage;

		static void ReflectType(Schematyc::CTypeDesc<SInfectedAreaProperties>& desc) {

			desc.SetGUID("{E7E90344-2DE6-4DA7-8829-98C41E4E6D7F}"_cry_guid);
			desc.SetLabel("Infected Area Properties");
			desc.SetDescription("The different properties for the infected area");
			desc.AddMember(&SInfectedAreaProperties::iGasmaskLevel, 'igml', "GasmaskLevel", "Gasmask Level", "Sets the gasmask level", 0);
			desc.AddMember(&SInfectedAreaProperties::fInfectedAreaDamage, 'fiad', "InfectedAreaDamage", "Infected Area Damage", "Sets the damage done by the area if you don't have any gasmask", 0.f);

		}

	};

public:
	CInfectedAreaComponent() = default;

	virtual void Initialize() override;
	virtual uint64 GetEventMask() const override;
	virtual void ProcessEvent(SEntityEvent& event) override;
	static void ReflectType(Schematyc::CTypeDesc<CInfectedAreaComponent>& desc);

	SInfectedAreaProperties *GetInfectedProperties() { return &sInfectedAreaProperties; }

	//Functions
	void Entering(EntityId Id);
	void Leaving(EntityId Id);

protected:

	//Vars
	bool bIsInside = false;
	int iPlayerCount = 0;

	SInfectedAreaProperties sInfectedAreaProperties;

};