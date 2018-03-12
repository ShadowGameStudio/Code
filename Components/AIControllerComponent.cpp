#include "StdAfx.h"
#include "AIControllerComponent.h"

static void RegisterAIControllerComponent(Schematyc::IEnvRegistrar& registrar) {
	Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
	{
		Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(CAIControllerComponent));
		// Functions
		{
		}
	}
}

CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterAIControllerComponent)

void CAIControllerComponent::Initialize() {}

void CAIControllerComponent::ProcessEvent(SEntityEvent& event) {}

uint64 CAIControllerComponent::GetEventMask() const {
	return uint64();
}

void CAIControllerComponent::ReflectType(Schematyc::CTypeDesc<CAIControllerComponent>& desc) {

	desc.SetGUID("{D40EFF18-F783-4513-91AB-84878D4B6CE2}"_cry_guid);
	desc.SetLabel("AI Contoller Component");
	desc.SetDescription("Handels the AI's control");
	desc.SetComponentFlags({ IEntityComponent::EFlags::Socket, IEntityComponent::EFlags::Attach, IEntityComponent::EFlags::Singleton });

}

//Adds some sort of velocity
void CAIControllerComponent::AddVelocity(const Vec3 velocity) {

	//If it can get the AIs physical entity, continue
	if (IPhysicalEntity* pPhysicalEntity = m_pEntity->GetPhysicalEntity()) {
		//Create a action move object
		pe_action_move moveAction;

		moveAction.iJump = 1;
		moveAction.dir = velocity;
		//Push the action
		pPhysicalEntity->Action(&moveAction);

	}

}

void CAIControllerComponent::MoveTo(const Vec3 cords) {



}

//Physicalizes the AI
void CAIControllerComponent::Physicalize() {

	//Physicalizes the AI as type livig
	SEntityPhysicalizeParams physParams;
	physParams.type = PE_LIVING;
	physParams.nSlot = GetOrMakeEntitySlotId();

	physParams.mass = m_controller.fMass;

	pe_player_dimensions playerDimensions;

	playerDimensions.bUseCapsule = true;
	playerDimensions.sizeCollider = Vec3(m_controller.fRadius * 0.5f, 1.f, m_controller.fHeight * 0.5f);
	
	if (playerDimensions.bUseCapsule) {
		playerDimensions.sizeCollider.z *= 0.5f;
	}

	playerDimensions.heightPivot = 0.f;
	// Offset collider upwards if the user requested it
	playerDimensions.heightCollider = m_pTransform != nullptr ? m_pTransform->GetTranslation().z : 0.f;
	playerDimensions.groundContactEps = 0.004f;

	physParams.pPlayerDimensions = &playerDimensions;

	m_pEntity->Physicalize(physParams);

	m_pEntity->UpdateComponentEventMask(this);

}
