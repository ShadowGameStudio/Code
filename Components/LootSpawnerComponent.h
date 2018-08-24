/*-------------------------------------------------------------------------------

Author : Ivar Jönsson
Project : BR
Purpose : Handels all of the loot spawning

-------------------------------------------------------------------------------*/

#pragma once

#include <CryEntitySystem/IEntityComponent.h>

class CLootSpawnerComponent : public IEntityComponent {

	struct SLootSpawnerProperties {

		inline bool operator==(const SLootSpawnerProperties& rhs) const { return 0 == memcmp(this, &rhs, sizeof(rhs)); }
		inline bool operator!=(const SLootSpawnerProperties& rhs) const { return 0 != memcmp(this, &rhs, sizeof(rhs)); }

		//If zero it doesn't have any and counts as common loot?
		int iLootLevel;

		static void ReflectType(Schematyc::CTypeDesc<SLootSpawnerProperties>& desc) {

			desc.SetGUID("{BB7C97D9-51FF-4457-966E-BECA3C702C49}"_cry_guid);
			desc.SetLabel("Loot Spawner Properties");
			desc.SetDescription("Sets the different properties for the loot, such as what loot rarity to spawn");
			desc.AddMember(&SLootSpawnerProperties::iLootLevel, 'ilry', "LootLevel", "Loot Level", "Sets the loot level(1 lowest, 3 highest)", 0);

		}

	};

public:

	//arrays
	std::vector<XmlString> sCommonClasses = {};
	std::vector<XmlString> sUncommonClasses = {};
	std::vector<XmlString> sRareClasses = {};
	std::vector<XmlString> sRarerClasses = {};
	std::vector<XmlString> sURareClasses = {};
	//~arrays

	CLootSpawnerComponent() = default;

	virtual void Initialize() override;
	virtual uint64 GetEventMask() const override;
	virtual void ProcessEvent(const SEntityEvent& event) override;
	static void ReflectType(Schematyc::CTypeDesc<CLootSpawnerComponent>& desc);

	SLootSpawnerProperties *GetProperties() { return &sLootSpawnerProperties; }
	void SpawnItem();
	void GetRandomClass(int *classAmount, int *rarityType);
	void GetRandomRarity(int itemLevel);

	void GetModelsFromXML();

	int sCommonClassAmount = sCommonClasses.size();
	int sUncommonClassAmount = sUncommonClasses.size();
	int sRareClassAmount = sRareClasses.size();
	int sRarerClassAmount = sRarerClasses.size();
	int sURareClassAmount = sURareClasses.size();

	//Int that will be used to pass the amount of classes in the specified vector 
	int *pLootClassAmount = nullptr;
	string sClass = "";
	XmlString sName = "";

	bool bIsSpawned = false;

protected:

	SLootSpawnerProperties sLootSpawnerProperties;

};