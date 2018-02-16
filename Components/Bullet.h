#pragma once
#include "HealthComponent.h"
#include "Player.h"

////////////////////////////////////////////////////////
// Physicalized bullet shot from weaponry, expires on collision with another object
////////////////////////////////////////////////////////
class CBulletComponent final : public IEntityComponent {
protected:

	CPlayerComponent *pPlayerShooting = nullptr;

public:
	virtual ~CBulletComponent() {}

	// IEntityComponent
	virtual void Initialize() override
	{
		// Set the model
		const int geometrySlot = 0;
		m_pEntity->LoadGeometry(geometrySlot, "Objects/Default/primitive_sphere.cgf");

		// Load the custom bullet material.
		// This material has the 'mat_bullet' surface type applied, which is set up to play sounds on collision with 'mat_default' objects in Libs/MaterialEffects
		auto *pBulletMaterial = gEnv->p3DEngine->GetMaterialManager()->LoadMaterial("Materials/bullet");
		m_pEntity->SetMaterial(pBulletMaterial);

		// Now create the physical representation of the entity
		SEntityPhysicalizeParams physParams;
		physParams.type = PE_RIGID;
		physParams.mass = 20000.f;
		m_pEntity->Physicalize(physParams);

		// Make sure that bullets are always rendered regardless of distance
		// Ratio is 0 - 255, 255 being 100% visibility
		GetEntity()->SetViewDistRatio(255);

		// Apply an impulse so that the bullet flies forward
		if (auto *pPhysics = GetEntity()->GetPhysics())
		{
			pe_action_impulse impulseAction;

			const float initialVelocity = 1000.f;

			// Set the actual impulse, in this cause the value of the initial velocity CVar in bullet's forward direction
			impulseAction.impulse = GetEntity()->GetWorldRotation().GetColumn1() * initialVelocity;

			// Send to the physical entity
			pPhysics->Action(&impulseAction);
		}
	}
	static void ReflectType(Schematyc::CTypeDesc<CBulletComponent>& desc)
	{
		desc.SetGUID("{B53A9A5F-F27A-42CB-82C7-B1E379C41A2A}"_cry_guid);
	}
	virtual uint64 GetEventMask() const override { return BIT64(ENTITY_EVENT_COLLISION); }
	virtual void ProcessEvent(SEntityEvent& event) override {
		// Handle the OnCollision event, in order to have the entity removed on collision
	
		switch (event.event) {
		case ENTITY_EVENT_COLLISION:
			
			EventPhysCollision *physCollision = reinterpret_cast<EventPhysCollision *>(event.nParam[0]);
			if (physCollision) {

				IPhysicalEntity *pThisEntityPhysics = physCollision->pEntity[0];
				IEntity *pThisEntity = gEnv->pEntitySystem->GetEntityFromPhysics(pThisEntityPhysics);
				IPhysicalEntity *pColliderPhysics = physCollision->pEntity[1];
				IEntity *pColliderEntity = gEnv->pEntitySystem->GetEntityFromPhysics(pColliderPhysics);

				//Gets the player, the players inventory, the weapon he's holding and the damage the weapon does
				//After that it damages the shot player
				if (pColliderEntity && pColliderEntity != m_pEntity && pColliderEntity) {
					if (CHealthComponent *pVictimHealth = pColliderEntity->GetComponent<CHealthComponent>()) {
						if (pVictimHealth->IsAlive()) {
							if (SItemComponent *pSelectedWeapon = pPlayerShooting->GetInventory()->GetSelectedItem()) {
								pVictimHealth->Add((-pSelectedWeapon->GetItemDamage()));
							}
						}
					}
				}
			}
			break;
		}
	}

	//Sets the player, called every time the bullet has been created
	void SetPlayer(CPlayerComponent *pPlayer) {

		pPlayerShooting = pPlayer;

	}
	// ~IEntityComponent
};