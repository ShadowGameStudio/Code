/*-------------------------------------------------------------------------------

Author : Ivar Jönsson
Project : BR
Purpose : The base class for the play area spawn location entity

-------------------------------------------------------------------------------*/

#pragma once

#include <CryEntitySystem/IEntityComponent.h>

class CPlayAreaSpawnComponent : public IEntityComponent {

	struct SPlayAreaSpawnProperties {

		inline bool operator==(const SPlayAreaSpawnProperties& rhs) const { return 0 == memcmp(this, &rhs, sizeof(rhs)); }
		inline bool operator!=(const SPlayAreaSpawnProperties& rhs) const { return 0 != memcmp(this, &rhs, sizeof(rhs)); }

		Vec3 vPos = Vec3(0, 0, 0);

		static void ReflectType(Schematyc::CTypeDesc<SPlayAreaSpawnProperties>& desc) {

			desc.SetGUID("{565B37FA-2B94-4802-AC13-8D1F44BD4BB3}"_cry_guid);
			desc.SetLabel("Play Area Spawn Properties");
			desc.SetDescription("Play Area Spawn Properties");
			desc.AddMember(&SPlayAreaSpawnProperties::vPos, 'vpos', "Position", "Position", "Position", Vec3(0, 0, 0));

		}

	};

public:
	CPlayAreaSpawnComponent() = default;

	virtual void Initialize() override;
	virtual void ProcessEvent(SEntityEvent& event) override;
	virtual uint64 GetEventMask() const override;
	static void ReflectType(Schematyc::CTypeDesc<CPlayAreaSpawnComponent>& desc);

	SPlayAreaSpawnProperties *GetProperties() { return &props; }
	void SpawnPlayArea(IEntity* otherEntity);

	bool bIsInvisible = false;


private:

	SPlayAreaSpawnProperties props;

};