#include "StdAfx.h"

#include "AIComponent.h"
#include <CrySchematyc\Env\Elements\EnvComponent.h>
#include "Player.h"

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

	//if (m_pAnimationComponent && m_pAIController) {

	//	m_pAnimationComponent->ResetCharacter();
	//	m_pAIController->Physicalize();

	//}

}

uint64 CAIComponent::GetEventMask() const {
	return BIT64(ENTITY_EVENT_TIMER) | BIT64(ENTITY_EVENT_UPDATE) | BIT64(ENTITY_EVENT_START_GAME);
}

void CAIComponent::ProcessEvent(SEntityEvent & event) {

	switch (event.event) {

	case ENTITY_EVENT_TIMER:

		if (event.nParam[0] == Timer_Speed) {

			//Generates random moving speed
			srand((unsigned)time(NULL));
			int speed = rand() % 300 + 100;

			m_fMoveSpeed = (float)speed;
			m_bTimerSet = false;
		}

		break;

	case ENTITY_EVENT_START_GAME:
		m_bGameStarted = true;
		break;

	case ENTITY_EVENT_UPDATE:
		
		SEntityUpdateContext *pCtx = (SEntityUpdateContext*)event.nParam[0];
		
		UpdateMode(pCtx->fFrameTime);
		UpdateMovementRequest(pCtx->fFrameTime);

		UpdateAnimation(pCtx->fFrameTime);
		UpdateVicinity(pCtx->fFrameTime);
		UpdateLookOrientation(pCtx->fFrameTime);
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
