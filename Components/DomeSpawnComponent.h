/*-------------------------------------------------------------------------------

Author : Ivar Jönsson
Project : BR
Purpose : Entites for defining different spawn points for the dome

-------------------------------------------------------------------------------*/

#pragma once

#include <CryEntitySystem/IEntityComponent.h>
#include "DomeSpawnProperties.h"

class CDomeSpawnComponent : public IEntityComponent {
public:
	CDomeSpawnComponent() = default;
	
	virtual SDomeSpawnProperties *GetDomeSpawnProperties() { return &sDomeSpawnProperties; }

	static void ReflectType(Schematyc::CTypeDesc<CDomeSpawnComponent>& desc);
	void SpawnDome(IEntity* otherEntity);

	virtual int GetSpawnpointID() { return GetDomeSpawnProperties()->spawnpointID; }

protected:

	SDomeSpawnProperties sDomeSpawnProperties;

};

