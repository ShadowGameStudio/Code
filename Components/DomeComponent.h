/*-------------------------------------------------------------------------------

Author : Ivar Jönsson
Project : BR
Purpose : The dome component

-------------------------------------------------------------------------------*/

#pragma once

#include <CryEntitySystem/IEntityComponent.h>
#include "ItemComponent.h"
#include "DomeProperties.h"
#include "DomeSpawnComponent.h"

struct CDomeComponent : public SItemComponent {

public:
	CDomeComponent() = default;

	virtual void InitializeClass() override;
	virtual void ProcessEventClass(SEntityEvent& event) override;
	static void ReflectType(Schematyc::CTypeDesc<CDomeComponent>& desc);

	virtual void PickUp(IEntity *pNewOwner) override;
	virtual void Drop() override;
	virtual bool IsPickable() override;
	virtual void Physicalize() override;
	virtual void LoadGeometry() override;

	virtual void SpawnDome();

	virtual SDomeProperties *GetDomeProperties() { return &sDomeProperties; }
	CDomeComponent *m_pDomeComponent = nullptr;

	virtual float GetMoveTime() { return GetDomeProperties()->fMoveTime; }
	virtual float GetPersistanceTime() { return GetDomeProperties()->fTime; }
	virtual int GetCircleAmount() { return GetDomeProperties()->circleAmount; }

protected:

	SDomeProperties sDomeProperties, sPrevDomeProperties;

};