#include "StdAfx.h"

#include "PlayAreaSpawnerComponent.h"

static void RegisterPlayAreaSpawnerComponent(Schematyc::IEnvRegistrar& registrar) {
	Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
	{
		Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(CPlayAreaSpawnerComponent));
		// Functions
		{
		}
	}
}

CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterPlayAreaSpawnerComponent)

void CPlayAreaSpawnerComponent::Initialize() {}

uint64 CPlayAreaSpawnerComponent::GetEventMask() const {
	return ENTITY_EVENT_BIT(ENTITY_EVENT_START_LEVEL);
}

void CPlayAreaSpawnerComponent::ProcessEvent(const SEntityEvent & event) {

	switch (event.event) {

	case ENTITY_EVENT_START_LEVEL:

		//Spawns the play area when the level has been loaded
		SpawnPlayArea();

	break;

	}

}

void CPlayAreaSpawnerComponent::ReflectType(Schematyc::CTypeDesc<CPlayAreaSpawnerComponent>& desc) {

	desc.SetGUID("{2A430502-4B68-4E3D-A506-F9FA1C9BD020}"_cry_guid);
	desc.SetEditorCategory("Gameplay");
	desc.SetLabel("Play Area Spawner Component");
	desc.SetDescription("Play Area Spawner Component");
	desc.SetComponentFlags({ IEntityComponent::EFlags::Transform, IEntityComponent::EFlags::Socket, IEntityComponent::EFlags::Attach });

}

void CPlayAreaSpawnerComponent::SpawnPlayArea() {

	//Setup spawn params for play area
	SEntitySpawnParams spawnParams;

	spawnParams.pClass = gEnv->pEntitySystem->GetClassRegistry()->GetDefaultClass();
	spawnParams.sName = "PlayArea";
	spawnParams.vPosition = m_pEntity->GetWorldPos();
	spawnParams.vScale = Vec3(1, 1, 1);

	//Spawn the entity
	if (IEntity* pEntity = gEnv->pEntitySystem->SpawnEntity(spawnParams)) {

		//Adds PlayAreaComponent to entity
		pEntity->CreateComponentClass<CPlayAreaComponent>();

	}

}
