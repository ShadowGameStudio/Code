/*-------------------------------------------------------------------------------

Author : Ivar Jönsson
Project : BR
Purpose : Health that can be applied to any "living" entities

-------------------------------------------------------------------------------*/

#pragma once

#include "VitalStatusComponent.h"

class CHealthComponent : public SVitalStatusComponent {

public:
	CHealthComponent() = default;

	virtual void InitializeClass() override;
	virtual void ProcessClassEvent(SEntityEvent& event) override;
	static void ReflectType(Schematyc::CTypeDesc<CHealthComponent>& desc);
	
	//Healthsystem
	void Update(float frameTime);
	bool IsAlive() { return bIsAlive; }

	bool bTimerSet = false;

private:

	bool bIsAlive = false;

};