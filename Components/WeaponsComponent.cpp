#include "StdAfx.h"
#include "WeaponsComponent.h"

static void RegisterWeapon(Schematyc::IEnvRegistrar& registrar) {
	Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
	{
		Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(CWeaponsComponent));
		// Functions
		{
		}
	}
}
CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterWeapon)

void CWeaponsComponent::InitializeClass() {



}

void CWeaponsComponent::ProcessEventClass(SEntityEvent & event) {}

void CWeaponsComponent::ReflectType(Schematyc::CTypeDesc<CWeaponsComponent>& desc) {

	desc.SetGUID("{07BDE070-B935-4E3B-8C46-5DA9A9843071}"_cry_guid);
	desc.AddBase<SItemComponent>();
	desc.SetEditorCategory("Weapons");
	desc.SetLabel("Weapons Component");
	desc.SetDescription("Component that handels all the weapons capabileties");
	desc.AddMember(&CWeaponsComponent::sItemProperties, 'itep', "ItemProperties", "Item Properties", "All of the basic items properties", SItemProperties());
	desc.AddMember(&CWeaponsComponent::sWeaponProperties, 'weap', "WeaponProperties", "Weapon Properties", "The different properties for the weapon", SWeaponProperties());

}
