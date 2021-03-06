#include "StdAfx.h"
#include "WeaponComponent.h"
#include <CryNetwork\Rmi.h>
#include "BulletComponent.h"

using ServerShootRMI = SRmi<RMI_WRAP(&CWeaponComponent::ServerShoot)>;
using ClientShootRMI = SRmi<RMI_WRAP(&CWeaponComponent::ClientShoot)>;

static void RegisterWeapon(Schematyc::IEnvRegistrar& registrar) {
	Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
	{
		Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(CWeaponComponent));
		// Functions
		{
		}
	}
}
CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterWeapon)

void CWeaponComponent::InitializeClass() {

	//Shoot RMI(Server)
	{
		const bool bIsServer = true;
		const ERMIAttachmentType attachmentType = eRAT_NoAttach;
		const ENetReliabilityType reliability = eNRT_UnreliableOrdered;

		ServerShootRMI::Register(this, attachmentType, bIsServer, reliability);
	}

	//Shoot RMI(Client)
	{
		const bool bIsServer = false;
		const ERMIAttachmentType attachmentType = eRAT_NoAttach;
		const ENetReliabilityType reliability = eNRT_UnreliableOrdered;

		ClientShootRMI::Register(this, attachmentType, bIsServer, reliability);
	}

	pBullet = m_pEntity->GetOrCreateComponentClass<CRayBulletComponent>();

}

void CWeaponComponent::ProcessEventClass(const SEntityEvent & event) {

	switch (event.event) {

		case ENTITY_EVENT_COMPONENT_PROPERTY_CHANGED:
		{
			SetProperties();
		}

		case ENTITY_EVENT_COLLISION:
		{
			//Meele weapon specific
			//If weapon is meele
			if (GetWeaponProperties()->bIsMeele) {
				if (pOwnerEntity) {
					EventPhysCollision *physCollison = reinterpret_cast<EventPhysCollision *>(event.nParam[0]);
					if (physCollison) {

						//This Entity
						IPhysicalEntity *pThisEntityPhyics = physCollison->pEntity[0];
						IEntity *pThisEntity = gEnv->pEntitySystem->GetEntityFromPhysics(pThisEntityPhyics);
						IPhysicalEntity *pColliderPhysics = physCollison->pEntity[1];
						IEntity *pColliderEntity = gEnv->pEntitySystem->GetEntityFromPhysics(pColliderPhysics);

						//Gets the colliding entitys health and removes health from it if it's not dead
						if (pColliderEntity && pColliderEntity != m_pEntity && pColliderEntity != pOwnerEntity) {
							if (CHealthComponent *pVictimHealth = pColliderEntity->GetComponent<CHealthComponent>()) {
								if (pVictimHealth->IsAlive() && bIsAttacking) {
									bIsAttacking = false;
									pVictimHealth->Add((-GetWeaponProperties()->fDamage));
								}
							}
						}
					}
				}
			}

			break;
		}

		case ENTITY_EVENT_TIMER:
		{
			//Meele weapon specific
			//If weapon is meele
			if (GetWeaponProperties()->bIsMeele) {
				if (event.nParam[0] == Timer_Attack)
					bIsAttacking = false;
			}
			break;
		}
	}

}

void CWeaponComponent::SetProperties() {

	pBullet->fShootingRange = GetWeaponProperties()->fFireRange;

}

void CWeaponComponent::ReflectType(Schematyc::CTypeDesc<CWeaponComponent>& desc) {

	desc.SetGUID("{77818A15-58D9-4C70-9E53-B9EFD445F13C}"_cry_guid);
	desc.AddBase<SItemComponent>();
	desc.SetEditorCategory("Items");
	desc.SetLabel("Weapon Component");
	desc.SetDescription("Handles all the weapon logic");
	desc.AddMember(&CWeaponComponent::sItemProperties, 'itep', "ItemProperties", "Item Properties", "Sets all of the weapon properties", SItemProperties());
	desc.AddMember(&CWeaponComponent::sWeaponProperties, 'weap', "WeaponProperties", "Weapon Properties", "All of the different propeties for the weapon", SWeaponProperties());
	desc.AddMember(&CWeaponComponent::sFiremodeProperties, 'feap', "FiremodeProperties", "Firemode Properties", "Sets all of the firemode properties", SFiremodeProperties());

}

//Reloads the current weapon
void CWeaponComponent::Reload() {

	if (iCurrAmmo == 0) {
		//Remove the amount of ammo in a full clip from max ammo
		GetWeaponProperties()->iMaxAmmo -= GetWeaponProperties()->iClipAmmo;
		//Sets current ammo to the max clip size
		iCurrAmmo = GetWeaponProperties()->iClipAmmo;
	}
	else if (iCurrAmmo >= 1) {
		//Get the value between max clip ammo and current ammo
		int iNewAmmo = GetWeaponProperties()->iClipAmmo - iCurrAmmo;
		//Remove the vaule above from max ammo
		GetWeaponProperties()->iMaxAmmo -= iNewAmmo;
		//Sets current ammo to the max clip size
		iCurrAmmo = GetWeaponProperties()->iClipAmmo;
	}
	else if (iCurrAmmo < 0) {
		//if current ammo is less than zero, set it back to zero
		iCurrAmmo = 0;
	}
}

//Shoots the weapon
void CWeaponComponent::Shoot(EntityId Id) {

	//If the weapon isn't meele, continue
	if (!GetWeaponProperties()->bIsMeele) {

		//If it can get the weapons bullet component, continue
		if (CRayBulletComponent *pBullet = m_pEntity->GetComponent<CRayBulletComponent>()) {

			//Get the entity from the provided Id
			IEntity *pPlayer = gEnv->pEntitySystem->GetEntity(Id);

			//Get the player component from the entity
			if (CPlayerComponent *pPlayerComp = pPlayer->GetComponent<CPlayerComponent>()) {
				
				//Add recoil
				AddRecoil();

				//Shot away a bullet
				pBullet->ShootBullet(pPlayerComp);
				//Remove a bullet from the mag
				iCurrAmmo -= 1;

				//Remove the recoil 
				RemoveRecoil();

				//If it can get the MainUI of the player, continue
				if (CUIComponent *pUI = pPlayer->GetComponent<CUIComponent>()) {

					//Update the ammo text
					pUI->UpdateAmmo();

				}

			}


		}

	}

}

//Adds recoil to the weapon
void CWeaponComponent::AddRecoil() {



}

void CWeaponComponent::RemoveRecoil() {}



//Starting meele attack
void CWeaponComponent::StartAttack() {

	bIsAttacking = true;
	m_pEntity->SetTimer(Timer_Attack, 1000);

}

//Stopping meele attack
void CWeaponComponent::StopAttack() {

	bIsAttacking = false;
	m_pEntity->KillTimer(Timer_Attack);

}

//////////////////Network//////////////////

//Called by the server
bool CWeaponComponent::ClientShoot(SShootParams && p, INetChannel * pNetChannel) {

	//Check so that weapon acually isn't meele
	if (!GetWeaponProperties()->bIsMeele) {

		//Shoot away a bullet
		Shoot(p.playerId);

	}
	return true;
}

//Called by RequestShot
bool CWeaponComponent::ServerShoot(SShootParams && p, INetChannel * pNetChannel) {
	//Shoots on all the clients
	ClientShootRMI::InvokeOnAllClients(this, SShootParams{ p.playerId });

	return true;
}

//Called when player presses fire button
bool CWeaponComponent::RequestShot(EntityId playerId) {
	if (gEnv->bServer) {

		//Shoots the weapon directly
		Shoot(playerId);

	}
	else {
		//Sends request to server
		ServerShootRMI::InvokeOnServer(this, SShootParams{ playerId });
	}

	return false;
}