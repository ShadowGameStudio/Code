/*-------------------------------------------------------------------------------

Author : Ivar Jönsson
Project : BR
Purpose : The component that handels the infected areas in houses

-------------------------------------------------------------------------------*/

#pragma once

#include <CryEntitySystem/IEntityComponent.h>

class CInfectedAreaComponent : public IEntityComponent {

	struct SInfectedAreaProperties {

		inline bool operator==(const SInfectedAreaProperties& rhs) const { return 0 == memcmp(this, &rhs, sizeof(rhs)); }
		inline bool operator!=(const SInfectedAreaProperties& rhs) const { return 0 != memcmp(this, &rhs, sizeof(rhs)); }

		int iGasmaskLevel;

		static void ReflectType(Schematyc::CTypeDesc<SInfectedAreaProperties>& desc) {

			desc.SetGUID("{E7E90344-2DE6-4DA7-8829-98C41E4E6D7F}"_cry_guid);
			desc.SetLabel("Infected Area Properties");
			desc.SetDescription("The different properties for the infected area");
			desc.AddMember(&SInfectedAreaProperties::iGasmaskLevel, 'igml', "GasmaskLevel", "Gasmask Level", "Sets the gasmask level", 0);

		}

	};

public:
	CInfectedAreaComponent() = default;
	//CComponent::~CComponent();

	virtual void Initialize() override;
	virtual uint64 GetEventMask() const override;
	virtual void ProcessEvent(SEntityEvent& event) override;
	static void ReflectType(Schematyc::CTypeDesc<CInfectedAreaComponent>& desc);

	//Functions
	void Entering();
	void Leaving();

	//Vars
	bool bIsInside;
	bool bPlayerHasMask;

protected:

	SInfectedAreaProperties sInfectedAreaProperties;

};