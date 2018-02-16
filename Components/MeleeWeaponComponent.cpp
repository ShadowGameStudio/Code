#include "StdAfx.h"
#include "MeleeWeaponComponent.h"
#include "HealthComponent.h"

static void RegisterMeleeWeapon(Schematyc::IEnvRegistrar& registrar) {
	Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
	{
		Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(CMeleeWeaponComponent));
		// Functions
		{
		}
	}
}
CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterMeleeWeapon)

void CMeleeWeaponComponent::InitializeClass() {}

void CMeleeWeaponComponent::ProcessEventClass(SEntityEvent & event) {

	switch (event.event) {
	case ENTITY_EVENT_COLLISION:

		if (pOwnerEntity) {

			EventPhysCollision * physCollision = reinterpret_cast<EventPhysCollision *>(event.nParam[0]);
			if (physCollision) {

				//This Entity
				IPhysicalEntity *pThisEntityPhysics = physCollision->pEntity[0];
				IEntity *pThisEntity = gEnv->pEntitySystem->GetEntityFromPhysics(pThisEntityPhysics);
				IPhysicalEntity *pColliderPhysics = physCollision->pEntity[1];
				IEntity *pColliderEntity = gEnv->pEntitySystem->GetEntityFromPhysics(pColliderPhysics);

				if (pColliderEntity && pColliderEntity != m_pEntity && pColliderEntity != pOwnerEntity) {

					if (CHealthComponent *pVictimHealth = pColliderEntity->GetComponent<CHealthComponent>()) {

						if (pVictimHealth->IsAlive() && bIsAttacking) {
							bIsAttacking = false;
					//		pVictimHealth->Add((-props.fDamage));
						}
					}
				}
			}
		}

		break;

	case ENTITY_EVENT_TIMER:

		if (event.nParam[0] == Timer_Attack) {
			
			bIsAttacking = false;

		}

		break;

	}

}

void CMeleeWeaponComponent::ReflectType(Schematyc::CTypeDesc<CMeleeWeaponComponent>& desc) {

	desc.SetGUID("{1B241BD0-DA6F-44A0-A7C0-EA14269D5839}"_cry_guid);
	desc.AddBase<SItemComponent>();
	desc.SetEditorCategory("Items");
	desc.SetLabel("Melee Weapon Component");
	desc.SetDescription("Melee Weapon Component");
	desc.AddMember(&CMeleeWeaponComponent::sItemProperties, 'itep', "ItemProperties", "Item Settings", "All item properties", SItemProperties());
	desc.AddMember(&CMeleeWeaponComponent::props, 'melp', "MeeleProperties", "Meele Settings", "Meele weapon properties", SMeeleWeaponProperties());

}

void CMeleeWeaponComponent::StartAttack() {
		
	bIsAttacking = true;
	m_pEntity->SetTimer(Timer_Attack, 1000);

}

void CMeleeWeaponComponent::StopAttack() {

	bIsAttacking = false;
	m_pEntity->KillTimer(Timer_Attack);

}
