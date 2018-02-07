#pragma once

#include "StdAfx.h"

struct SDomeSpawnProperties {

	inline bool operator == (const SDomeSpawnProperties& rhs) const { return 0 == memcmp(this, &rhs, sizeof(rhs)); }
	inline bool operator != (const SDomeSpawnProperties& rhs) const { return 0 == memcmp(this, &rhs, sizeof(rhs)); }

	int spawnpointID;
	Schematyc::GeomFileName sGeomPath;

};

static void ReflectType(Schematyc::CTypeDesc<SDomeSpawnProperties>& desc) {

	desc.SetGUID("{6975F376-32D2-4A01-9DF1-90C6BDDCEFB1}"_cry_guid);
	desc.SetLabel("Dome Properties");
	desc.SetDescription("Dome Properties");
	desc.AddMember(&SDomeSpawnProperties::spawnpointID, 'spn', "SpawnpointID", "Spawnpoint ID", "Sets the ID that the spawnpoint has", 0);
	desc.AddMember(&SDomeSpawnProperties::sGeomPath, 'geom', "GeometryPath", "Geometry path", "Geometry path of this item", "");

}