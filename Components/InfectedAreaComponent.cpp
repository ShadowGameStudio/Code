#include "StdAfx.h"

#include "InfectedAreaComponent.h"
#include <CrySchematyc\Env\Elements\EnvComponent.h>
#include <CrySchematyc\Env\IEnvRegistrar.h>

static void RegisterInfectedAreaComponent(Schematyc::IEnvRegistrar& registrar) {
	Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
	{
		Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(CInfectedAreaComponent));
		// Functions
		{
		}
	}
}

CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterInfectedAreaComponent )

void CInfectedAreaComponent::Initialize() {}

uint64 CInfectedAreaComponent::GetEventMask() const {
	return uint64();
}

void CInfectedAreaComponent::ProcessEvent(SEntityEvent & event) {}

void CInfectedAreaComponent::ReflectType(Schematyc::CTypeDesc<CInfectedAreaComponent>& desc) {

	desc.SetGUID("{3E8C4D67-7A56-4A77-93AF-5AA0512A7580}"_cry_guid);
	desc.SetEditorCategory("Gameplay");
	desc.SetLabel("Infected Area Component");
	desc.SetDescription("Handels all the infected area functionality");
	desc.SetComponentFlags({ IEntityComponent::EFlags::Transform, IEntityComponent::EFlags::Socket, IEntityComponent::EFlags::Attach });
	desc.AddMember(&CInfectedAreaComponent::sInfectedAreaProperties, 'iasp', "InfectedAreaProperties", "Infected Area Properties", "All the properties for the infected area", SInfectedAreaProperties());

}

