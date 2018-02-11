#include "StdAfx.h"
#include "StaminaComponent.h"
#include <CryEntitySystem\IEntity.h>
#include <CrySchematyc\Env\IEnvRegistrar.h>
#include <CrySchematyc\Env\Elements\EnvComponent.h>

static void RegisterStaminaComponent(Schematyc::IEnvRegistrar& registrar) {
	Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
	{
		Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(CStaminaComponent));
		// Functions
		{
		}
	}
}
CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterStaminaComponent)

void CStaminaComponent::InitializeClass() {

}

void CStaminaComponent::ProcessClassEvent(SEntityEvent & event) {

	switch (event.event) {
	case ENTITY_EVENT_UPDATE:

		SEntityUpdateContext * pCtx = (SEntityUpdateContext*)event.nParam[0];
		Update(pCtx->fFrameTime);

		break;
	}

}

void CStaminaComponent::ReflectType(Schematyc::CTypeDesc<CStaminaComponent>& desc) {

	desc.SetGUID("{B5D1C916-DDB9-4CC7-9FF9-F5CCEFF81774}"_cry_guid);
	desc.AddBase<SVitalStatusComponent>();
	desc.SetEditorCategory("Entities");
	desc.SetLabel("Stamina Component");
	desc.SetDescription("Stamina Component");
	desc.SetComponentFlags({ IEntityComponent::EFlags::Transform, IEntityComponent::EFlags::Socket, IEntityComponent::EFlags::Attach });
	desc.AddMember(&CStaminaComponent::props, 'prop', "StaminaProps", "Stamina Properties", "Sets the different stamina options", SProperties());

}

//Regenerates Stamina every frame
void CStaminaComponent::Update(float frameTime) {

	//Regeneration
	Regenerate();

	if (props.fValue > props.fMax)
		props.fValue = props.fMax;

	if (props.fValue < 0.f)
		props.fValue = 0.f;

}
