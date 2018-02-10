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

		int *pLootAmount = &test;
		string sClass = GetRandomClass(pLootAmount);

		//Setup spawn params for the loot
		SEntitySpawnParams spawnParams;

		spawnParams.pClass = gEnv->pEntitySystem->GetClassRegistry()->FindClass(sClass);
		spawnParams.sName = sClass;
		spawnParams.vPosition = m_pEntity->GetWorldPos();
		spawnParams.vScale = Vec3(1, 1, 1);

		//Spawn the entity
		if (IEntity* pEntity = gEnv->pEntitySystem->SpawnEntity(spawnParams)) {

			//fix so that this also gets class
			pEntity->CreateComponentClass<CLootSpawnComponent>();
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

string CLootSpawnComponent::GetRandomClass(int* classAmount) {

	int* pClassAmount = classAmount;
	
	//Gets a random number
	srand((unsigned int)time(NULL));
	int RandIndex = rand() % *pClassAmount;
	
	//Returns the value at the randomized position
	if (GetProperties()->iLootRarity == 1)
		return sCommonClasses[RandIndex];
	else if (GetProperties()->iLootRarity == 2)
		return sUncommonClasses[RandIndex];
	else if (GetProperties()->iLootRarity == 3)
		return sRareClasses[RandIndex];
	else if (GetProperties()->iLootRarity == 4)
		return sRarerClasses[RandIndex];
	else if (GetProperties()->iLootRarity == 5)
		return sURareClasses[RandIndex];
	else
		return "NULL";
}
