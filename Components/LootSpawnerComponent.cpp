#include "StdAfx.h"
#include "LootSpawnerComponent.h"
#include <CrySystem/ISystem.h>

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
void CLootSpawnerComponent::Initialize() {

	//Gets all of the models to spawn at the start
	GetModelsFromXML();
	//Gets all of the spawn positions from the xml file
	GetSpawnPositionsFromXML();

}

//Gets different events
uint64 CLootSpawnerComponent::GetEventMask() const {
	return ENTITY_EVENT_BIT(ENTITY_EVENT_START_LEVEL) | ENTITY_EVENT_BIT(ENTITY_EVENT_LEVEL_LOADED);
}

//Checks if the events have been called
void CLootSpawnerComponent::ProcessEvent(const SEntityEvent & event) {

	switch (event.event) {
	case ENTITY_EVENT_START_LEVEL:
		SpawnItem();
		break;

	case ENTITY_EVENT_LEVEL_LOADED:
		if (gEnv->IsEditor()) 
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

		//Gets the rarity and select a class
		//Sets sClass to a random value
		GetRandomRarity(GetProperties()->iLootLevel);

		//Setup spawn params
		SEntitySpawnParams spawnParams;
		IEntityClass *pItemClass = gEnv->pEntitySystem->GetClassRegistry()->FindClass(sClass);

		spawnParams.pClass = pItemClass;
		spawnParams.sName = sName;
		spawnParams.vPosition = SpawnPositions[1];
		spawnParams.vScale = Vec3(1, 1, 1);

		//Spawning item
		if (IEntity* pEntity = gEnv->pEntitySystem->SpawnEntity(spawnParams))
			bIsSpawned = true;

	}
}

//Gets a random item class based on item level
void CLootSpawnerComponent::GetRandomClass(int *classAmount, int *rarityType) {
	
	int *pClassAmount = classAmount;

	//Gets a random number
	srand((unsigned int)time(NULL));
	int iRandIndex = rand() % *pClassAmount;

	//Gets the class and returns the class string
	if (*rarityType == 1)
		sClass = sCommonClasses[iRandIndex];
	else if (*rarityType == 2)
		sClass = sUncommonClasses[iRandIndex];
	else if (*rarityType == 3)
		sClass = sRareClasses[iRandIndex];
	else if (*rarityType == 4)
		sClass = sRarerClasses[iRandIndex];
	else if (*rarityType == 5)
		sClass = sURareClasses[iRandIndex];
	else
		return;
}

//Gets the RandomRarity based on item level
void CLootSpawnerComponent::GetRandomRarity(int itemLevel) {

	int iRarityType;

	//If the item level is less than three, continue
	if (itemLevel < 3) {

		//Get random number that will be used below
		srand((unsigned int)time(NULL));
		int iRandRarity = rand() % 2;

		//If the item level is equal to one, continue
		if (itemLevel == 1) {
			//If the random number generated is equal to zero, continue
			if (iRandRarity == 0) {
				//Set the loot class amount to a certain number
				pLootClassAmount = &sCommonClassAmount;
				//Set the rarity type to one
				iRarityType = 1;
				GetModelsFromXML();
			}
			else if (iRandRarity == 1) {
				pLootClassAmount = &sUncommonClassAmount;
				iRarityType = 2;
			}
		}
		//Else if the item level is equal to two, contiune
		else if (itemLevel == 2) {
			if (iRandRarity == 0) {
				pLootClassAmount = &sRareClassAmount;
				iRarityType = 3;
			}
			else if (iRandRarity == 1){
				pLootClassAmount = &sRarerClassAmount;
				iRarityType = 4;
			}
		}
		//Else if the item level is zero, continue
		else if (itemLevel == 0) {
			//Set all level less items to common
			pLootClassAmount = &sCommonClassAmount;
			iRarityType = 5;
		}

	}
	//Else if item level is three, continue
	else if (itemLevel == 3) {
		pLootClassAmount = &sURareClassAmount;
		iRarityType = 5;
	}
	//If it's none of these, return
	else {
		return;
	}
	//Int made for passing on
	int *pRarityType = &iRarityType;

	//Get the random class from
	GetRandomClass(pLootClassAmount, pRarityType);

}

//Get the loot models
void CLootSpawnerComponent::GetModelsFromXML() {

	//Get the rootNode from an XML file
	if (XmlNodeRef rootNode = gEnv->pSystem->LoadXmlFromFile("LootXML.xml", false)) {

		//Get every child of the rootNode
		for (int i = 0, n = rootNode->getChildCount(); i < n; i++) {

			//Get the child
			XmlNodeRef childNode = rootNode->getChild(i);
			int rarity = 0;

			//Get the rarity attribute
			if (childNode->getAttr("rarity", rarity)) {

				//If the rarity is 1, continue
				if (rarity == 1) {
					XmlString path;
					//Get the path
					if (childNode->getAttr("modelPath", path)) {
						//Add the path to the correct vector
						sCommonClasses.push_back(path);

						//Get the items name
						XmlString name;
						if (childNode->getAttr("modelName", name)) {
							sName = name;
						}

					}

				}
				//If the rarity is 2, continue
				else if (rarity == 2) {
					XmlString path;
					//Get the path
					if (childNode->getAttr("modelPath", path)) {
						//Add the path to the correct vector
						sUncommonClasses.push_back(path);

						//Get the items name
						XmlString name;
						if (childNode->getAttr("modelName", name)) {
							sName = name;
						}
					}
				}
				//If the rarity is 3, continue
				else if (rarity == 3) {
					XmlString path;
					//Get the path
					if (childNode->getAttr("modelPath", path)) {
						//Add the path to the correct vector
						sRareClasses.push_back(path);

						//Get the items name
						XmlString name;
						if (childNode->getAttr("modelName", name)) {
							sName = name;
						}
					}
				}
				//If the rarity is 4, continue
				else if (rarity == 4) {
					XmlString path;
					//Get the path
					if (childNode->getAttr("modelPath", path)) {
						//Add the path to the correct vector
						sRarerClasses.push_back(path);

						//Get the items name
						XmlString name;
						if (childNode->getAttr("modelName", name)) {
							sName = name;
						}
					}
				}
				//If the rarity is 5, continue
				else if (rarity == 5) {
					XmlString path;
					//Gets the path
					if (childNode->getAttr("modelPath", path)) {
						//Add the path to the correct vector
						sURareClasses.push_back(path);

						//Get the items name
						XmlString name;
						if (childNode->getAttr("modelName", name)) {
							sName = name;
						}
					}
				}

			}

		}

	}

}

//Get the spawn positions
void CLootSpawnerComponent::GetSpawnPositionsFromXML() {

	//Get the root node from te xml file
	if (XmlNodeRef rootNode = gEnv->pSystem->LoadXmlFromFile("SpawnPositionsXML.xml", false)) {

		//Get every child of the root node
		for (int i = 0, n = rootNode->getChildCount(); i < n; i++) {

			//Get the child
			XmlNodeRef childNode = rootNode->getChild(i);
			Vec3 spawnPos;

			//Get the spawnPositions attribute
			if (childNode->getAttr("spawnPosition", spawnPos)) {
				//Add the position to the vector
				SpawnPositions.push_back(spawnPos);
			}

		}

	}

}
