#include "StdAfx.h"

#include "BulletComponent.h" 

void CRayBulletComponent::Initialize() {}

uint64 CRayBulletComponent::GetEventMask() const {
	return uint64();
}

void CRayBulletComponent::ProcessEvent(const SEntityEvent & event) {}

void CRayBulletComponent::ReflectType(Schematyc::CTypeDesc<CRayBulletComponent>& desc) {

	desc.SetGUID("{EE15952C-BF3D-4670-936D-9260F2FB711B}"_cry_guid);

}

void CRayBulletComponent::ShootBullet(CPlayerComponent *pPlayer) {

	//Get half of the games window width and height
	const float fHalfRenderWidth = static_cast<float>(gEnv->pRenderer->GetWidth()) * 0.5f;
	const float fHalfRenderHeight = static_cast<float>(gEnv->pRenderer->GetHeight()) * 0.5f;

	//Set the range for the ray to travel
	const float fRange = fShootingRange;

	//Get the positions
	Vec3 cameraCenterNear, cameraCenterFar;
	gEnv->pRenderer->UnProjectFromScreen(fHalfRenderWidth, fHalfRenderHeight, 0, &cameraCenterNear.x, &cameraCenterNear.y, &cameraCenterNear.z);
	gEnv->pRenderer->UnProjectFromScreen(fHalfRenderWidth, fHalfRenderHeight, 1, &cameraCenterFar.x, &cameraCenterFar.y, &cameraCenterFar.z);

	//Set the direction for the ray to travel
	const Vec3 searchDirecton = (cameraCenterFar - cameraCenterNear).GetNormalized() * fRange;

	//Create the array of hits
	std::array<ray_hit, 1> hits;

	//Set the query flags
	const uint32 queryFlags = ent_all;

	//Set the flags to use for the ray
	const uint32 rayFlags = rwi_stop_at_pierceable;

	//Get the hits
	const int numHits = gEnv->pPhysicalWorld->RayWorldIntersection(cameraCenterNear, searchDirecton, queryFlags, rayFlags, hits.data(), hits.max_size());

	//If it hits, continue
	if (numHits > 0) {

		//If it hits an entity, continue
		if (IEntity *pHitEntity = gEnv->pEntitySystem->GetEntityFromPhysics(hits[0].pCollider)) {

			//If it can get the health component, continue
			if (CHealthComponent *pHealth = pHitEntity->GetComponent<CHealthComponent>()) {

				//If the entity is alive, continue
				if (pHealth->IsAlive()) {

					//If it can get the weapon currently selected, continue
					if (SItemComponent *pShootingWeapon = pPlayer->GetInventory()->GetSelectedWeapon()) {

						//If it can get the weapon component, continue
						if (CWeaponComponent *pWeapon = pShootingWeapon->GetEntity()->GetComponent<CWeaponComponent>()) {

							//Remove the health from the hit entity
							pHealth->Add((-pWeapon->GetDamage()));
							 
						}

					}

				}

			}

		}

	}

}
