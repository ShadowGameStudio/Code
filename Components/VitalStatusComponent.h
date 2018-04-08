/*-------------------------------------------------------------------------------

Author : Ivar Jönsson
Project : BR
Purpose : Vital Status is base class for various vital properties for living ents.

-------------------------------------------------------------------------------*/

#pragma once

#include <CryEntitySystem/IEntityComponent.h>
#include "UIComponent.h"

struct SVitalStatusComponent : public IEntityComponent {

	struct SProperties {

		inline bool operator==(const SProperties& rhs) const { return 0 == memcmp(this, &rhs, sizeof(rhs)); }
		inline bool operator!=(const SProperties& rhs) const { return 0 != memcmp(this, &rhs, sizeof(rhs)); }

		float fValue = 0.f;
		float fLastValue = 0.f;
		float fMax = 0.f;
		float fRegenerationRatio = 0.f;
		bool bAlwaysRegenerate = false;

		static void ReflectType(Schematyc::CTypeDesc<SProperties>& desc) {


			desc.SetGUID("{2CE67854-39E1-43B6-A383-C8A3FB33EE9A}"_cry_guid);
			desc.SetLabel("Vital Properties");
			desc.SetDescription("Vital Properties");
			desc.AddMember(&SProperties::fMax, 'fmax', "MaxValue", "Maximum value", "Maximum value", 100.f);
			desc.AddMember(&SProperties::fRegenerationRatio, 'freg', "RegenRatio", "Regeneration ratio", "Regeneration ratio", 0.02f);

		}

	};
	SVitalStatusComponent() = default;

	virtual void Initialize() override;
	virtual void InitializeClass() = 0;
	virtual uint64 GetEventMask() const override;
	virtual void ProcessEvent(const SEntityEvent& event) override;
	virtual void ProcessClassEvent(const SEntityEvent& event) = 0;
	static void ReflectType(Schematyc::CTypeDesc<SVitalStatusComponent>& desc);

	//Vitalsystem
	void Regenerate();
	void Add(float amount) 
	{ 
		props.fValue += amount;
		if (auto *pUI = m_pEntity->GetComponent<CUIComponent>()) {
			pUI->SetHealthbar();
		}
	}

	void Set(float amount) 
	{ 
		props.fValue = amount; 
		if (auto *pUI = m_pEntity->GetComponent<CUIComponent>()) {
			pUI->SetHealthbar();
		}
	}
	void SetMax(float amount) { props.fMax = amount; }
	void SetRegenerationRatio(float reg) { props.fRegenerationRatio = reg; }

	void AlwaysRegenerate(bool alwaysRegenerate) { props.bAlwaysRegenerate = alwaysRegenerate; }
	bool AlwaysRegenerate() { return props.bAlwaysRegenerate; }

	float Get() { return props.fValue; }
	float GetMax() { return props.fMax; }
	float GetRegenerationRatio() { return props.fRegenerationRatio; }
	SProperties *GetProperties() { return &props; }

protected:

	SProperties props;
	SProperties startProps;

public:

	bool bRegenerate = false;

};