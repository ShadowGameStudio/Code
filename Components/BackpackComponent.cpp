#include "StdAfx.h"

#include "BackpackComponent.h"

static void RegisterBackpack(Schematyc::IEnvRegistrar& registrar) {
	Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
	{
		Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(CBackpackComponent));
		// Functions
		{
		}
	}
}
CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterBackpack)

void CBackpackComponent::InitializeClass() {}

void CBackpackComponent::ProcessEventClass(SEntityEvent & event) {}

void CBackpackComponent::ReflectType(Schematyc::CTypeDesc<CBackpackComponent>& desc) {

	desc.SetGUID("{A63641C4-747D-4635-8944-780D4744BD5B}"_cry_guid);
	desc.AddBase<SItemComponent>();
	desc.SetEditorCategory("Items");
	desc.SetLabel("Backpack Component");
	desc.SetDescription("Backpack Component");
	desc.AddMember(&CBackpackComponent::sItemProperties, 'itep', "ItemProperties", "Item Settings", "All item properties", SItemProperties());

}