/*-------------------------------------------------------------------------------

Author : Ivar Jönsson
Project : BR
Purpose : The component that handels the spawning of items at designated places

-------------------------------------------------------------------------------*/

#pragma once

#include <CryEntitySystem/IEntityComponent.h>
#include "LootSpawnProperties.h"
#include <vector>
#include <string>

class CLootSpawnComponent : public IEntityComponent {
public:
	CLootSpawnComponent() = default;

	std::vector<string> sCommonClasses = {"schematyc::items::test", "Test2"};
	std::vector<string> sUncommonClasses = {"test4"};
	std::vector<string> sRareClasses = {"test3"};
	std::vector<string> sRarerClasses = {"test2"};
	std::vector<string> sURareClasses = {"test1"};

	virtual void Initialize() override;
	virtual uint64 GetEventMask() const override;
	virtual void ProcessEvent(SEntityEvent& event) override;
	static void ReflectType(Schematyc::CTypeDesc<CLootSpawnComponent>& desc);

	virtual SLootSpawnProperties *GetProperties() { return &sLootSpawnProperties; }
	void SpawnItems();
	string GetRandomClass(int* classAmount);

	int test = 4;

protected:

	SLootSpawnProperties sLootSpawnProperties;

};