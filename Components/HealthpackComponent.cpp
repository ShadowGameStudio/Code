#include "HealthpackComponent.h"
#include <CryEntitySystem\IEntity.h>
#include <CrySchematyc\Env\IEnvRegistrar.h>
#include <CrySchematyc\Env\Elements\EnvComponent.h> 

static void RegisterHealthpack(Schematyc::IEnvRegistrar& registrar) {
	Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
	{
		Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(CHealthpackComponent));
		// Functions
		{
		}
	}
}
CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterHealthpack)

void CHealthpackComponent::InitializeClass() {}

void CHealthpackComponent::ProcessEventClass(const SEntityEvent & event) {}

void CHealthpackComponent::ReflectType(Schematyc::CTypeDesc<CHealthpackComponent>& desc) {

	desc.SetGUID("{7745EC78-2A96-4F50-81D4-479ECD7A9DE7}"_cry_guid);
	desc.AddBase<SItemComponent>();
	desc.SetEditorCategory("Pickups");
	desc.SetLabel("Healthpack Component");
	desc.SetDescription("Healthpack Component");
	desc.AddMember(&CHealthpackComponent::props, 'hpro', "HealthpackProps", "Healthpack Properties", "Sets the Healthpack properties", SHealthpackProperties());
	desc.AddMember(&CHealthpackComponent::sItemProperties, 'itep', "ItemProperties", "Item Settings", "All item settings", SItemProperties());

}