/*-------------------------------------------------------------------------------

Author : Ivar Jönsson
Project : BR
Purpose : Handels all of the AI controlling

-------------------------------------------------------------------------------*/

#pragma once

#include <CryEntitySystem/IEntityComponent.h>

class CAIControllerComponent final : public IEntityComponent {

	struct SControllerParams {

		inline bool operator==(const SControllerParams &rhs) const { return 0 == memcmp(this, &rhs, sizeof(rhs)); }
		inline bool operator!=(const SControllerParams &rhs) const { return 0 != memcmp(this, &rhs, sizeof(rhs)); }

		float fMass;
		float fRadius;
		float fHeight;

		static void ReflectType(Schematyc::CTypeDesc<SControllerParams>& desc) {

			desc.SetGUID("{0B974CF2-2744-4858-AA80-C556C06EE9A5}"_cry_guid);
			desc.SetLabel("Controller Params");
			desc.AddMember(&SControllerParams::fHeight, 'fhe', "Height", "Height", "Sets the height of the controller in meters", 2.f);
			desc.AddMember(&SControllerParams::fMass, 'fma', "Mass", "Mass", "Sets the mass of the AI in kilos", 80.f);
			desc.AddMember(&SControllerParams::fRadius, 'fra', "Radius", "Radius", "Sets the radius of the controller", 1.f);

		}

	};

public:
	CAIControllerComponent() = default;

	virtual void Initialize() final;
	virtual void ProcessEvent(SEntityEvent& event) final;
	virtual uint64 GetEventMask() const final;
	static void ReflectType(Schematyc::CTypeDesc<CAIControllerComponent>& desc);

	virtual void AddVelocity(const Vec3 velocity);
	virtual void MoveTo(const Vec3 cords);
	virtual void Physicalize();

	bool IsWalking() const { return m_velocity.GetLength2D() > 0.2f && m_bOnGround; }

	//View
	Ang3 CreateAnglesYPR(const Matrix33& m);
	Ang3 CreateAnglesYPR(const Vec3& vDir, f32 r);
	Matrix33 CreateOrientationYPR(const Ang3& ypr);

protected:

	bool m_bOnGround;
	Vec3 m_velocity = ZERO;

private:

	SControllerParams m_controller;

};