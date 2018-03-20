#include "StdAfx.h"
#include "GasmaskComponent.h"

static void RegisterGasmask(Schematyc::IEnvRegistrar& registrar) {
	Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
	{
		Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(CGasmaskComponent));
		// Functions
		{
		}
	}
}

CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterGasmask)

void CGasmaskComponent::InitializeClass() {}
void CGasmaskComponent::ProcessEventClass(const SEntityEvent & event) {}

void CGasmaskComponent::ReflectType(Schematyc::CTypeDesc<CGasmaskComponent>& desc) {

	desc.SetGUID("{AD3B9B4E-7084-4637-B2F9-EE79C4979217}"_cry_guid);
	desc.AddBase<SItemComponent>();
	desc.SetEditorCategory("Items");
	desc.SetLabel("Gasmask Component");
	desc.SetDescription("Handels all the gasmask functionality");
	desc.AddMember(&CGasmaskComponent::sGasmaskProperties, 'gmp', "GasmaskProperties", "Gasmask Properties", "Sets the Gasmask properties", SGasmaskProperties());
	desc.AddMember(&CGasmaskComponent::sItemProperties, 'itep', "ItemProperties", "Item Settings", "All item settings", SItemProperties());

}
	