#pragma once
#include "ItemComponent.h"
#include "WeaponComponent.h"

////////////////////////////////////////////////////////
// Physicalized bullet shot from weaponry, expires on collision with another object
////////////////////////////////////////////////////////
class CBulletComponent final : public IEntityComponent {
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

	// Reflect type to set a unique identifier for this component
	static void ReflectType(Schematyc::CTypeDesc<CBulletComponent>& desc)
	{
		desc.SetGUID("{B53A9A5F-F27A-42CB-82C7-B1E379C41A2A}"_cry_guid);
	}

	virtual uint64 GetEventMask() const override { return BIT64(ENTITY_EVENT_COLLISION); }
	virtual void ProcessEvent(SEntityEvent& event) override {

		switch (event.event) {
		case ENTITY_EVENT_COLLISION:

			gEnv->pEntitySystem->RemoveEntity(GetEntityId());

			EventPhysCollision *physCollision = reinterpret_cast<EventPhysCollision *>(event.nParam[0]);
			if (physCollision) {

				IPhysicalEntity *pThisEntityPhysics = physCollision->pEntity[0];
				IEntity *pThisEntity = gEnv->pEntitySystem->GetEntityFromPhysics(pThisEntityPhysics);
				IPhysicalEntity *pColliderPhysics = physCollision->pEntity[1];
				IEntity *pColliderEntity = gEnv->pEntitySystem->GetEntityFromPhysics(pColliderPhysics);

				if (pColliderEntity && pColliderEntity != m_pEntity && pColliderEntity) {

					if (CHealthComponent *pVicitmHealth = pColliderEntity->GetComponent<CHealthComponent>()) {

						//Fix so that it gets the player that is shootings weapon
						if (true) {

						}

						if (pVicitmHealth->IsAlive()) {
							pVicitmHealth->Add(())
						}

					}
					else
						return;

			break;
		}

	}
	// ~IEntityComponent
};
