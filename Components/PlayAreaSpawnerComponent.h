/*-------------------------------------------------------------------------------

Author : Ivar Jönsson
Project : BR
Purpose : Handels the spawning of the play area

-------------------------------------------------------------------------------*/

#pragma once

#include <CryEntitySystem/IEntityComponent.h>

class CPlayAreaSpawnerComponent : public IEntityComponent {
public:
	CPlayAreaSpawnerComponent() = default;

	virtual void Initialize() override;
	virtual uint64 GetEventMask() const override;
	virtual void ProcessEvent(const SEntityEvent& event) override;
	static void ReflectType(Schematyc::CTypeDesc<CPlayAreaSpawnerComponent>& desc);

	void SpawnPlayArea();

};