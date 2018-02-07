#include "StdAfx.h"
#include "Dome.h"

static void RegisterDome(Schematyc::IEnvRegistrar& registrar) {
	Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
	{
		Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(CDomeComponent));
		// Functions
		{
		}
	}
}
CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterDome)

void CDomeComponent::InitializeClass() {}

void CDomeComponent::ProcessEventClass(SEntityEvent & event) {}

void CDomeComponent::ReflectType(Schematyc::CTypeDesc<CDomeComponent>& desc) {

	desc.SetGUID("{9A32AAF0-9CD6-425B-9907-D8933BA16A4A}"_cry_guid);
	desc.AddBase<SDomeComponent>();
	desc.SetEditorCategory("Gameplay");
	desc.SetLabel("Dome Component");
	desc.SetDescription("Dome Component");
	desc.AddMember(&CDomeComponent::sDomeProperties, 'itep', "ItemProperties", "Item Settings", "All item properties", SDomeProperties());

}
