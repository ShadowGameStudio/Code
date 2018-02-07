/*-------------------------------------------------------------------------------

Author : Ivar Jönsson
Project : 
Purpose : 

-------------------------------------------------------------------------------*/

#pragma once

#include <CryEntitySystem/IEntityComponent.h>
#include "LootSpawnProperties.h"

struct SClassNames {

	//Fix lootSpawnComponent able to find random class with right weapon type and such
	string sMeeleWeapon = "CMeeleWeaponComponent";
	string sBackpack = "CBackpackComponent";

};

class CLootSpawnComponent : public IEntityComponent {
public:
	CLootSpawnComponent() = default;

	virtual void Initialize() override;
	virtual uint64 GetEventMask() const override;
	virtual void ProcessEvent(SEntityEvent& event) override;
	static void ReflectType(Schematyc::CTypeDesc<CLootSpawnComponent>& desc);

	virtual SLootSpawnProperties *GetProperties() { return &sLootSpawnProperties; }
	void SpawnItems();

protected:

	SLootSpawnProperties sLootSpawnProperties;

};