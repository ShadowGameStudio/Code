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

		int iLootRarity;

		static void ReflectType(Schematyc::CTypeDesc<SLootSpawnerProperties>& desc) {

			desc.SetGUID("{BB7C97D9-51FF-4457-966E-BECA3C702C49}"_cry_guid);
			desc.SetLabel("Loot Spawner Properties");
			desc.SetDescription("Sets the different properties for the loot, such as what loot rarity to spawn");
			desc.AddMember(&SLootSpawnerProperties::iLootRarity, 'ilry', "LootRarity", "Loot Rarity", "Sets the loot rarity(1 lowest, 5 highest)", 0);

		}

	};

public:

	//arrays
	std::vector<string> sCommonClasses = { "schematyc::items::test" };
	//~arrays

	CLootSpawnerComponent() = default;

	virtual void Initialize() override;
	virtual uint64 GetEventMask() const override;
	virtual void ProcessEvent(SEntityEvent& event) override;
	static void ReflectType(Schematyc::CTypeDesc<CLootSpawnerComponent>& desc);

	SLootSpawnerProperties *GetProperties() { return &sLootSpawnerProperties; }
	void SpawnItem();
	string GetRandomClass(int* classAmount);

	int sCommonClassAmount = sCommonClasses.size();
	bool bIsSpawned = false;

protected:

	SLootSpawnerProperties sLootSpawnerProperties;

};