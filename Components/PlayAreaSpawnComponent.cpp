#include "StdAfx.h"
#include "PlayAreaSpawnComponent.h"

static void RegisterPlayAreaSpawn(Schematyc::IEnvRegistrar& registrar) {
	Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
	{
		Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(CPlayAreaSpawnComponent));
		// Functions
		{
		}
	}
}
CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterPlayAreaSpawn)

void CPlayAreaSpawnComponent::Initialize() {

	//Set geom and make visible
	const int geometrySlot = 0;
	m_pEntity->LoadGeometry(geometrySlot, "Objects/Default/primitive_sphere.cgf");
	m_pEntity->Invisible(bIsInvisible);

	//Set material
	auto *pSpawnMaterial = gEnv->p3DEngine->GetMaterialManager()->LoadMaterial("Materials/default");
	m_pEntity->SetMaterial(pSpawnMaterial);

	//Physicalize
	SEntityPhysicalizeParams physParams;
	physParams.type = PHYS_GEOM_TYPE_NO_COLLIDE;
	physParams.mass = 20000.f;
	m_pEntity->Physicalize(physParams);

	//Set view distance
	GetEntity()->SetViewDistRatio(255);

}

uint64 CPlayAreaSpawnComponent::GetEventMask() const {
	return ENTITY_EVENT_BIT(ENTITY_EVENT_START_LEVEL), ENTITY_EVENT_BIT(ENTITY_EVENT_COMPONENT_PROPERTY_CHANGED);
}

void CPlayAreaSpawnComponent::ProcessEvent(const SEntityEvent & event) {

	switch (event.event) {
	case ENTITY_EVENT_START_LEVEL:
		bIsInvisible = true;
		m_pEntity->Invisible(bIsInvisible);
		break;

	case ENTITY_EVENT_COMPONENT_PROPERTY_CHANGED:

		Initialize();

	}

}

void CPlayAreaSpawnComponent::ReflectType(Schematyc::CTypeDesc<CPlayAreaSpawnComponent>& desc) {

	desc.SetGUID("{104174BD-DAFD-4CB3-8E98-1369944AB5EC}"_cry_guid);
	desc.SetEditorCategory("Gameplay");
	desc.SetLabel("Play Area Spawn");
	desc.SetDescription("Play Area Spawn");
	desc.SetComponentFlags({ IEntityComponent::EFlags::Transform, IEntityComponent::EFlags::Socket, IEntityComponent::EFlags::Attach });
	desc.AddMember(&CPlayAreaSpawnComponent::props, 'prop', "PlayAreaSpawnProps", "Play Area Spawn Properties", "Sets the different play area spawn properties", SPlayAreaSpawnProperties());

}

void CPlayAreaSpawnComponent::SpawnPlayArea(IEntity * otherEntity) {

	otherEntity->SetPos(m_pEntity->GetWorldPos());

}
