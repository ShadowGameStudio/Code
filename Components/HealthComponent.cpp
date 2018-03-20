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

void CHealthComponent::InitializeClass() {

	bIsAlive = props.fValue > 0.f;

}

void CHealthComponent::ProcessClassEvent(const SEntityEvent & event) {

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
	desc.AddBase<SVitalStatusComponent>();
	desc.SetLabel("Health Component");
	desc.SetDescription("Health Component");
	desc.SetComponentFlags({ IEntityComponent::EFlags::Transform, IEntityComponent::EFlags::Socket, IEntityComponent::EFlags::Attach });
	desc.AddMember(&CHealthComponent::props, 'prop', "HealthProps", "Health Properties", "Sets the different health options", SProperties());

}

//Regenerates health every frame
void CHealthComponent::Update(float frameTime) {

	//Regeneration

	if (bTimerSet) {
		Regenerate();
	}

}
