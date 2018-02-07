#include "StdAfx.h"

#include "DomeSpawnComponent.h"

static void RegisterDomeSpawnComponent(Schematyc::IEnvRegistrar& registrar) {
	Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
	{
		Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(CDomeSpawnComponent));
		// Functions
		{
		}
	}
}

CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterDomeSpawnComponent)

void CDomeSpawnComponent::ReflectType(Schematyc::CTypeDesc<CDomeSpawnComponent>& desc) {

	desc.SetGUID("{C809E32B-B250-41B6-8E5A-F0DDB6A84B55}"_cry_guid);
	desc.SetEditorCategory("Gameplay");
	desc.SetLabel("DomeSpawnpoint");
	desc.SetDescription("This spawnpoint is used to define different locations for the dome to spawn at");
	desc.SetComponentFlags({ IEntityComponent::EFlags::Transform, IEntityComponent::EFlags::Socket, IEntityComponent::EFlags::Attach });
	desc.AddMember(&CDomeSpawnComponent::sDomeSpawnProperties, 'dsp', "DomeSpawnProperties", "Dome Spawn Properties", "Sets the different Dome Spawn Properties", SDomeSpawnProperties());

}

void CDomeSpawnComponent::SpawnDome(IEntity * otherEntity) {

	int spawnpointIDrand = rand() % 3;

	int spawnpoint = GetSpawnpointID();

	switch (spawnpointIDrand) {
	case 0:
		if (spawnpoint == 1) {
			otherEntity->SetWorldTM(m_pEntity->GetWorldTM());
		}
		break;
	case 1:
		if (spawnpoint == 2) {
			otherEntity->SetWorldTM(m_pEntity->GetWorldTM());
		}
		break;
	case 2:
		if (spawnpoint == 3) {
			otherEntity->SetWorldTM(m_pEntity->GetWorldTM());
		}
	}

}
