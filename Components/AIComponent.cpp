#include "StdAfx.h"

#include "AIComponent.h"
#include <CrySchematyc\Env\Elements\EnvComponent.h>

static void RegisterAIComponent(Schematyc::IEnvRegistrar& registrar) {
	Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
	{
		Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(CAIComponent));
		// Functions
		{
		}
	}
}

CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterAIComponent)

void CAIComponent::Initialize() {

	m_pAIController = m_pEntity->GetComponent<CAIControllerComponent>();

	m_pAnimationComponent = m_pEntity->GetComponent<Cry::DefaultComponents::CAdvancedAnimationComponent>();
	m_pAnimationComponent->LoadFromDisk();
	m_idleFragmentId = m_pAnimationComponent->GetFragmentId("Idle");
	m_walkFragmentId = m_pAnimationComponent->GetFragmentId("Walk");
	m_rotateTagId = m_pAnimationComponent->GetTagId("Rotate");

}

uint64 CAIComponent::GetEventMask() const {
	return BIT64(ENTITY_EVENT_UPDATE);
}

void CAIComponent::ProcessEvent(SEntityEvent & event) {

	switch (event.event) {
	case ENTITY_EVENT_UPDATE:
		
		SEntityUpdateContext * pCtx = (SEntityUpdateContext*)event.nParam[0];
		
		if (gEnv->bServer)
			UpdateMovementRequest(pCtx->fFrameTime);

		UpdateAnimation(pCtx->fFrameTime);

		break;
	}

}

void CAIComponent::ReflectType(Schematyc::CTypeDesc<CAIComponent>& desc) {

	desc.SetGUID("{8E53C807-64C3-4DA0-9F1F-C0E0885D5EF4}"_cry_guid);
	desc.SetEditorCategory("AI");
	desc.SetLabel("AI Component");
	desc.SetDescription("Handels all the basic AI functionality");
	desc.SetComponentFlags({ IEntityComponent::EFlags::Transform, IEntityComponent::EFlags::Socket, IEntityComponent::EFlags::Attach });

}

void CAIComponent::UpdateMovementRequest(float frameTime) {

	//Create a Vec3
	Vec3 velocity = ZERO;
	//Sets the moving speed for the AI
	float moveSpeed = 100.f;
	//Adds these two together
	velocity.y += moveSpeed * frameTime;
	//Pushes the movement
	m_pAIController->AddVelocity(GetEntity()->GetWorldRotation() * velocity);

}

void CAIComponent::UpdateAnimation(float frameTime) {}
