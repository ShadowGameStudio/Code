/*-------------------------------------------------------------------------------

Author : Ivar Jönsson
Project : BR
Purpose : Vital Status is base class for various vital properties for living ents.

-------------------------------------------------------------------------------*/

#pragma once

#include <CryEntitySystem/IEntityComponent.h>

struct SVitalStatusComponent : public IEntityComponent {

	struct SHealthProperties {

		inline bool operator==(const SHealthProperties& rhs) const { return 0 == memcmp(this, &rhs, sizeof(rhs)); }
		inline bool operator!=(const SHealthProperties& rhs) const { return 0 != memcmp(this, &rhs, sizeof(rhs)); }

		float fValue = 0.f;
		float fMax = 0.f;
		float fRegenerationRatio = 0.f;

		static void ReflectType(Schematyc::CTypeDesc<SHealthProperties>& desc) {


			desc.SetGUID("{2CE67854-39E1-43B6-A383-C8A3FB33EE9A}"_cry_guid);
			desc.SetLabel("Vital Properties");
			desc.SetDescription("Vital Properties");
			desc.AddMember(&SHealthProperties::fMax, 'fmax', "MaxValue", "Maximum value", "Maximum value", 100.f);
			desc.AddMember(&SHealthProperties::fRegenerationRatio, 'freg', "RegenRatio", "Regeneration ratio", "Regeneration ratio", 0.02f);

		}

	};
	SVitalStatusComponent() = default;

	virtual void Initialize() override;
	virtual uint64 GetEventMask() const override;
	static void ReflectType(Schematyc::CTypeDesc<SVitalStatusComponent>& desc);

	//Vitalsystem
	void Regenerate();
	void Add(float healthAmount) { props.fValue += healthAmount; }
	void Set(float healthAmount) { props.fValue = healthAmount; }
	void SetRegenerationRatio(float reg) { props.fRegenerationRatio = reg; }
	float Get() { return props.fValue; }
	float GetMax() { return props.fMax; }
	float GetRegenerationRatio() { return props.fRegenerationRatio; }
	SHealthProperties *GetProperties() { return &props; }

protected:

	SHealthProperties props;
	SHealthProperties startProps;

};