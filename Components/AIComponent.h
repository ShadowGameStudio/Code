/*-------------------------------------------------------------------------------

Author : Ivar Jönsson
Project : BR
Purpose : Handels all the basic AI functionality

-------------------------------------------------------------------------------*/

#pragma once

#include <CryEntitySystem/IEntityComponent.h>
#include <DefaultComponents\Physics\CharacterControllerComponent.h>
#include <DefaultComponents\Geometry\AdvancedAnimationComponent.h>
#include "AIControllerComponent.h"

class CAIComponent final : public IEntityComponent {
public:
	CAIComponent() = default;

	virtual void Initialize() override;
	virtual uint64 GetEventMask() const override;
	virtual void ProcessEvent(SEntityEvent& event) override;
	static void ReflectType(Schematyc::CTypeDesc<CAIComponent>& desc);

protected:

	void UpdateMovementRequest(float frameTime);
	void UpdateAnimation(float frameTime);


protected:

	CAIControllerComponent *m_pAIController = nullptr;
	Cry::DefaultComponents::CAdvancedAnimationComponent *m_pAnimationComponent = nullptr;

	FragmentID m_idleFragmentId;
	FragmentID m_walkFragmentId;
	TagID m_rotateTagId;

};