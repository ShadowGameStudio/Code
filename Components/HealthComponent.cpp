#include "StdAfx.h"
#include "HealthComponent.h"

static void RegisterHealth(Schematyc::IEnvRegistrar& registrar) {
	Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
	{
		Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(CHealthComponent));
		// Functions
		{
		}
	}
}
CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterHealth)

void CHealthComponent::Initialize() {

	healthProps.fHealthValue = healthProps.fMaxHealth;
	healthProps.bIsAlive = healthProps.fHealthValue > 0.f;
	startHealthProps = healthProps;

}

uint64 CHealthComponent::GetEventMask() const {
	return BIT64(ENTITY_EVENT_UPDATE);
}

void CHealthComponent::ProcessEvent(SEntityEvent & event) {

	switch (event.event) {
	case ENTITY_EVENT_UPDATE:

		SEntityUpdateContext * pCtx = (SEntityUpdateContext*)event.nParam[0];
		Update(pCtx->fFrameTime);

		break;
	}

}

void CHealthComponent::ReflectType(Schematyc::CTypeDesc<CHealthComponent>& desc) {

	desc.SetGUID("{213D6A99-5A8C-4F6B-946C-ACC80949ED3D}"_cry_guid);
	desc.SetEditorCategory("Entities");
	desc.SetLabel("Health Component");
	desc.SetDescription("Health Component");
	desc.SetComponentFlags({ IEntityComponent::EFlags::Transform, IEntityComponent::EFlags::Socket, IEntityComponent::EFlags::Attach });
	desc.AddMember(&CHealthComponent::healthProps, 'prop', "HealthProps", "Health Properties", "Sets the different health options", SHealthProperties());

}

void CHealthComponent::Update(float frameTime) {

	//Regeneration

	if (healthProps.bIsAlive) {
		
		if (healthProps.fHealthValue < healthProps.fMaxHealth)
			healthProps.fHealthValue += healthProps.fHealthRegenerationRatio;

	}

	if (healthProps.fHealthValue > healthProps.fMaxHealth)
		healthProps.fHealthValue = healthProps.fMaxHealth;

		healthProps.bIsAlive = healthProps.fHealthValue > 0.f;

	if (!healthProps.bIsAlive)
		healthProps.fHealthValue = 0.f;

}
