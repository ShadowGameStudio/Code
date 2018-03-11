/*-------------------------------------------------------------------------------

Author : Ivar Jönsson
Project : BR
Purpose : Handels all of the AI controlling

-------------------------------------------------------------------------------*/

#pragma once

#include <CryEntitySystem/IEntityComponent.h>

class CAIControllerComponent final : public IEntityComponent {

	struct SMovementParams {

		inline bool operator==(const SMovementParams &rhs) const { return 0 == memcmp(this, &rhs, sizeof(rhs)); }
		inline bool operator==(const SMovementParams &rhs) const { return 0 != memcmp(this, &rhs, sizeof(rhs)); }

		float fMass;


	};

public:
	CAIControllerComponent() = default;

	virtual void Initialize() final;
	virtual void ProcessEvent(SEntityEvent& event) final;
	virtual uint64 GetEventMask() const final;
	static void ReflectType(Schematyc::CTypeDesc<CAIControllerComponent>& desc);

	virtual void AddVelocity(const Vec3 velocity);
	virtual void Physicalize();

};