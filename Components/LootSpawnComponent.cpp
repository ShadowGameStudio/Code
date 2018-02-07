#include "LootSpawnComponent.h"

static void RegisterLootSpawnComponent(Schematyc::IEnvRegistrar& registrar) {
	Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
	{
		Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(CLootSpawnComponent));
		// Functions
		{
		}
	}
}

CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterLootSpawnComponent)

void CLootSpawnComponent::Initialize() {}

uint64 CLootSpawnComponent::GetEventMask() const {
	return BIT64(ENTITY_EVENT_LEVEL_LOADED);
}

void CLootSpawnComponent::ProcessEvent(SEntityEvent & event) {

	switch (event.event) {
	case ENTITY_EVENT_LEVEL_LOADED:
	
		SpawnItems();

		break;
	}

}

void CLootSpawnComponent::ReflectType(Schematyc::CTypeDesc<CLootSpawnComponent>& desc) {

	desc.SetGUID("{7881EF65-440B-4063-8BF1-83F4A1FA6DB6}"_cry_guid);
	desc.SetEditorCategory("Gameplay");
	desc.SetLabel("Loot Spawn Component");
	desc.SetDescription("Loot Spawn Component");
	desc.SetComponentFlags({ IEntityComponent::EFlags::Transform, IEntityComponent::EFlags::Socket, IEntityComponent::EFlags::Attach });
	desc.AddMember(&CLootSpawnComponent::sLootSpawnProperties, 'losp', "LootSpawnProperties", "Loot Spawn Properties", "The different loot spawn properties", SLootSpawnProperties());

}

void CLootSpawnComponent::SpawnItems() {

	if (GetProperties()->iLootRarity == 1) {

		//Setup spawn params for the loot
		SEntitySpawnParams spawnParams;

		spawnParams.pClass = gEnv->pEntitySystem->GetClassRegistry()->FindClass("");
		spawnParams.sName = "";
		spawnParams.vPosition = Vec3(500, 580, 30);
		spawnParams.vScale = Vec3(1, 1, 1);

		//Spawn the entity
		if (IEntity* pEntity = gEnv->pEntitySystem->SpawnEntity(spawnParams)) {

			pEntity->CreateComponentClass<class test>(); 

		}

	}
	else if (GetProperties()->iLootRarity == 2) {

	}
	else if (GetProperties()->iLootRarity == 3) {

	}
	else if (GetProperties()->iLootRarity == 4) {

	}
	else if (GetProperties()->iLootRarity == 5) {

	}

}
