/*-------------------------------------------------------------------------------

Author : Ivar Jönsson
Project : BR
Purpose : Handels the zipline functionality

-------------------------------------------------------------------------------*/

#pragma once

#include <CryEntitySystem/IEntityComponent.h>
#include "ItemComponent.h"

class CZiplineComponent : public SItemComponent {
public:
	CZiplineComponent() = default;

	virtual void InitializeClass() override;
	virtual void ProcessEventClass(const SEntityEvent& event) override;
	static void ReflectType(Schematyc::CTypeDesc<CZiplineComponent>& desc);

	void OnZiplineAttached();
	void OnZiplineDetached();

	void StartRaycast();

};