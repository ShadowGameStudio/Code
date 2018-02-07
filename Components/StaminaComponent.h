/*-------------------------------------------------------------------------------

Author : Ivar Jönsson
Project : 
Purpose : 

-------------------------------------------------------------------------------*/

#pragma once

#include <CryEntitySystem/IEntityComponent.h>

class CComponent : public IEntityComponent {
public:
	CComponent() = default;
	CComponent::~CComponent();
};