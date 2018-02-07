/*-------------------------------------------------------------------------------

Author : Ivar Jönsson
Project : BR
Purpose : Health that can be applied to any "living" entities

-------------------------------------------------------------------------------*/

#pragma once

#include <CryEntitySystem/IEntityComponent.h>

class CHealthComponent : public IEntityComponent {

	struct SHealthProperties {

		inline bool operator==(const SHealthProperties& rhs) const { return 0 == memcmp(this, &rhs, sizeof(rhs)); }
		inline bool operator!=(const SHealthProperties& rhs) const { return 0 != memcmp(this, &rhs, sizeof(rhs)); }

		float fHealthValue = 0.f;
		float fMaxHealth = 0.f;
		float fHealthRegenerationRatio = 0.f;
		bool bIsAlive = false;

		static void ReflectType(Schematyc::CTypeDesc<SHealthProperties>& desc) {

			desc.SetGUID("{2CE67854-39E1-43B6-A383-C8A3FB33EE9A}"_cry_guid);
			desc.SetLabel("Health Properties");
			desc.SetDescription("Health Properties");
			desc.AddMember(&SHealthProperties::fMaxHealth, 'fmax', "MaxHealth", "Maximum health", "Maximum health", 100.f);
			desc.AddMember(&SHealthProperties::fHealthRegenerationRatio, 'freg', "RegenRatio", "Regeneration ratio", "Regeneration ratio", 0.02f);

		}

	};

public:
	CHealthComponent() = default;

	virtual void Initialize() override;
	virtual uint64 GetEventMask() const override;
	virtual void ProcessEvent(SEntityEvent& event) override;
	static void ReflectType(Schematyc::CTypeDesc<CHealthComponent>& desc);
	
	//Healthsystem
	void Update(float frameTime);
	void Add(float healthAmount) { healthProps.fHealthValue += healthAmount;  }
	void Set(float healthAmount) { healthProps.fHealthValue = healthAmount;  }
	void SetRegenerationRatio(float reg) { healthProps.fHealthRegenerationRatio = reg; }
	float Get() { return healthProps.fHealthValue; }
	float GetMax() { return healthProps.fMaxHealth; }
	float GetRegenerationRatio() { return healthProps.fHealthRegenerationRatio;  }
	bool IsAlive() { return healthProps.bIsAlive; }
	SHealthProperties *GetProperties() { return &healthProps;  }

private:

	SHealthProperties healthProps;
	SHealthProperties startHealthProps;

};