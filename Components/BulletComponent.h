/*-------------------------------------------------------------------------------

Author : Ivar Jönsson
Project : BR
Purpose : Handels the bullet functionality

-------------------------------------------------------------------------------*/

#pragma once

#include <CryEntitySystem/IEntityComponent.h>

class CRayBulletComponent : public IEntityComponent {
public:
	CRayBulletComponent() = default;

	virtual void Initialize() override;
	virtual uint64 GetEventMask() const override;
	virtual void ProcessEvent(const SEntityEvent& event) override;
	static void ReflectType(Schematyc::CTypeDesc<CRayBulletComponent>& desc);

	void ShootBullet(CPlayerComponent *pPlayer);

	float fShootingRange = 0;

}; 