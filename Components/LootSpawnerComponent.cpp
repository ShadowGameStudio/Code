#include "StdAfx.h"
#include "LootSpawnerComponent.h"

static void RegisterLootSpawnerComponent(Schematyc::IEnvRegistrar& registrar) {
	Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
	{
		Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(CLootSpawnerComponent));
		//Funcitons
		{
		}
	}
}

CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterLootSpawnerComponent)

//Called when the component is created
void CLootSpawnerComponent::Initialize() {}

//Gets different events
uint64 CLootSpawnerComponent::GetEventMask() const {
	return BIT64(ENTITY_EVENT_LEVEL_LOADED);
}

//Checks if the events have been called
void CLootSpawnerComponent::ProcessEvent(SEntityEvent & event) {

	switch (event.event) {
	case ENTITY_EVENT_LEVEL_LOADED:
		SpawnItem();
		break;
	}

}

//Exposes to Schematyc
void CLootSpawnerComponent::ReflectType(Schematyc::CTypeDesc<CLootSpawnerComponent>& desc) {

	desc.SetGUID("{C0456549-8A55-41BA-9302-5F13E62982B1}"_cry_guid);
	desc.SetEditorCategory("Gameplay");
	desc.SetLabel("Loot Spawner Component");
	desc.SetDescription("Spawns randomly selected loot based on rarity");
	desc.SetComponentFlags({ IEntityComponent::EFlags::Transform, IEntityComponent::EFlags::Socket, IEntityComponent::EFlags::Attach });
	desc.AddMember(&CLootSpawnerComponent::sLootSpawnerProperties, 'losp', "LootSpawnerProperties", "Loot Spawner Properties", "All the properites for spawning an item", SLootSpawnerProperties());

}

//Handels the actual spawning of the item
void CLootSpawnerComponent::SpawnItem() {

	if (!bIsSpawned) {

		int *pLootAmount = nullptr;

		//Checks what loot rarity it is
		if (GetProperties()->iLootRarity == 1)
			pLootAmount = &sCommonClassAmount;
		else if (GetProperties()->iLootRarity == 2)
			pLootAmount = &sUncommonClassAmount;
		else if (GetProperties()->iLootRarity == 3)
			pLootAmount = &sRareClassAmount;
		else if (GetProperties()->iLootRarity == 4)
			pLootAmount = &sRarerClassAmount;
		else if (GetProperties()->iLootRarity == 5)
			pLootAmount = &sURareClassAmount;

		string sClass = GetRandomClass(pLootAmount);

		//Setup spawn params
		SEntitySpawnParams spawnParams;
		IEntityClass *pItemClass = gEnv->pEntitySystem->GetClassRegistry()->FindClass(sClass);

		spawnParams.pClass = pItemClass;
		spawnParams.sName = "test";
		spawnParams.vPosition = Vec3(574, 516, 34);
		spawnParams.vScale = Vec3(10, 10, 10);

		//Spawning item
		if (IEntity* pEntity = gEnv->pEntitySystem->SpawnEntity(spawnParams))
			bIsSpawned = true;

	}
}

string CLootSpawnerComponent::GetRandomClass(int *classAmount) {
	
	int *pClassAmount = classAmount;

	//Gets a random number
	srand((unsigned int)time(NULL));
	int iRandIndex = rand() % *pClassAmount;

	//Gets the class and returns the class string
	if (GetProperties()->iLootRarity == 1)
		return sCommonClasses[iRandIndex];
	else if (GetProperties()->iLootRarity == 2)
		return sUncommonClasses[iRandIndex];
	else if (GetProperties()->iLootRarity == 3)
		return sRareClasses[iRandIndex];
	else if (GetProperties()->iLootRarity == 4)
		return sRarerClasses[iRandIndex];
	else if (GetProperties()->iLootRarity == 5)
		return sURareClasses[iRandIndex];
	else
		return string();
}
