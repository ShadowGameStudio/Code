/*-------------------------------------------------------------------------------

Author : Ivar Jönsson
Project : BR
Purpose : Handles the stamina, derived from VitalStatus

-------------------------------------------------------------------------------*/

#pragma once

#include "VitalStatusComponent.h"

class CStaminaComponent : public SVitalStatusComponent {

public:

	CStaminaComponent() = default;

	virtual void InitializeClass() override;
	virtual void ProcessClassEvent(SEntityEvent& event) override;
	static void ReflectType(Schematyc::CTypeDesc<CStaminaComponent>& desc);

	//Stamina System
	void Update(float frameTime);

};