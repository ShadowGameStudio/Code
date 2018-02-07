#pragma once

#include "StdAfx.h"

#include <CrySchematyc\ResourceTypes.h>
#include <CrySchematyc\Reflection\TypeDesc.h>

struct SLootSpawnProperties {

	inline bool operator==(const SLootSpawnProperties& rhs) const { return 0 == memcmp(this, &rhs, sizeof(rhs)); }
	inline bool operator!=(const SLootSpawnProperties& rhs) const { return 0 != memcmp(this, &rhs, sizeof(rhs)); }

	int iLootRarity;

};

static void ReflectType(Schematyc::CTypeDesc<SLootSpawnProperties>& desc) {

	desc.SetGUID("{F5784ABE-1C84-4018-B761-984990FDE573}"_cry_guid);
	desc.SetLabel("Physics Properties");
	desc.SetDescription("Physics Properties");
	desc.AddMember(&SLootSpawnProperties::iLootRarity, 'ilry', "Loot Rarity", "Loot Rarity", "Loot Rarity(1 lowest, 5 highest)", 0);

}