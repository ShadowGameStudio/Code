/*-------------------------------------------------------------------------------

Author : Ivar Jönsson
Project : BR
Purpose : The component that handels the infected areas in houses

-------------------------------------------------------------------------------*/

#pragma once

#include <CryEntitySystem/IEntityComponent.h>

class CInfectedAreaComponent final : public IEntityComponent {

	enum TIMER_EVENT {
		Timer_Damage
	};

	enum EDamageMode {

		eDM_NoDamage,
		eDM_LowDamage,
		eDM_HighDamage

	};

	struct SInfectedAreaProperties {

		inline bool operator==(const SInfectedAreaProperties& rhs) const { return 0 == memcmp(this, &rhs, sizeof(rhs)); }
		inline bool operator!=(const SInfectedAreaProperties& rhs) const { return 0 != memcmp(this, &rhs, sizeof(rhs)); }

		int iGasmaskLevel;
		float fDamage;
		float fWithMaskDamage;

		static void ReflectType(Schematyc::CTypeDesc<SInfectedAreaProperties>& desc) {

			desc.SetGUID("{E7E90344-2DE6-4DA7-8829-98C41E4E6D7F}"_cry_guid);
			desc.SetLabel("Infected Area Properties");
			desc.SetDescription("The different properties for the infected area");
			desc.AddMember(&SInfectedAreaProperties::iGasmaskLevel, 'igml', "GasmaskLevel", "Gasmask Level", "Sets the gasmask level", 0);
			desc.AddMember(&SInfectedAreaProperties::fDamage, 'fiad', "DamageWithOutMask", "Damage with out mask", "Sets the damage done by the area if you don't have any gasmask", 0.f);
			desc.AddMember(&SInfectedAreaProperties::fWithMaskDamage, 'iwmd', "DamageWithMask", "Damage With Mask", "Sets the damage done by the area if your mask level is to low", 0.f);

		}

	};

public:
	CInfectedAreaComponent() = default;

	virtual void Initialize() override;
	virtual uint64 GetEventMask() const override;
	virtual void ProcessEvent(const SEntityEvent& event) override;
	static void ReflectType(Schematyc::CTypeDesc<CInfectedAreaComponent>& desc);

	SInfectedAreaProperties *GetInfectedProperties() { return &sInfectedAreaProperties; }

	//Functions
	void Entering(Schematyc::ExplicitEntityId Id);
	void Leaving(Schematyc::ExplicitEntityId Id);

	void PlayerEntered(EntityId Id);

	void Update(float fFrameTime);

protected:

	//Vars
	bool bIsInside = false;
	bool bIsDamaging = false;
	bool bTimerSet = false;
	int iPlayerCount = 0;
	int iDamageMode = 0;

	SInfectedAreaProperties sInfectedAreaProperties;
	std::vector<IEntity*> pPlayerCount = {};

};