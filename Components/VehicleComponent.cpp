#include "StdAfx.h"

#include "VehicleComponent.h"
#include <CryEntitySystem/IEntitySystem.h>

//Network shorts
using SvEnterVehicleRMI = SRmi<RMI_WRAP(&CVehicleComponent::SvEnterVehicle)>;
using ClEnterVehicleRMI = SRmi<RMI_WRAP(&CVehicleComponent::ClEnterVehicle)>;


static void RegisterVehicleComponent(Schematyc::IEnvRegistrar &registrar) {
  Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
  {
    Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(CVehicleComponent));
    // Functions
    {
    }
  }
}

CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterVehicleComponent);

void CVehicleComponent::Initialize() { 
    
	LoadGeometry();
	Physicalize();

	//Enter RMI(Server)
	{
		const bool bIsServerCall = true;
		const ERMIAttachmentType attachmentType = eRAT_NoAttach;
		const ENetReliabilityType reliability = eNRT_UnreliableOrdered;

		SvEnterVehicleRMI::Register(this, attachmentType, bIsServerCall, reliability);

	}
	//Enter RMI(Client)
	{
		const bool bIsServerCall = false;
		const ERMIAttachmentType attachmentType = eRAT_NoAttach;
		const ENetReliabilityType reliability = eNRT_UnreliableOrdered; 

		ClEnterVehicleRMI::Register(this, attachmentType, bIsServerCall, reliability);

	}
	//Enables the item to have it's aspects delegated
	m_pEntity->GetNetEntity()->EnableDelegatableAspect(GetNetSerializeAspectMask(), true);
	//If it's server, continue
	if (gEnv->bServer) {
		//Binds the object to the server
		gEnv->pNetContext->BindObject(GetEntityId(), 0, GetNetSerializeAspectMask(), true);
	}
	//Bind the entity to network
	m_pEntity->GetNetEntity()->BindToNetwork();

}
uint64 CVehicleComponent::GetEventMask() const { 
    return ENTITY_EVENT_BIT(ENTITY_EVENT_COMPONENT_PROPERTY_CHANGED); 
}
void CVehicleComponent::ProcessEvent(const SEntityEvent &event) {

	switch (event.event) {
		case ENTITY_EVENT_COMPONENT_PROPERTY_CHANGED:
		{

			if (sVehicleProperties != sPrevVehicleProperties) {
				
				sPrevVehicleProperties = sVehicleProperties;

				LoadGeometry();
				Physicalize();

			}

		}
	
	}

}
void CVehicleComponent::ReflectType(Schematyc::CTypeDesc<CVehicleComponent> &desc) {

    desc.SetGUID("{18FBE08A-1812-43C6-A822-817203FFE6E3}"_cry_guid);
    desc.SetEditorCategory("Gameplay");
    desc.SetLabel("Vehicle component");
    desc.SetDescription("Handels all of the vehicle stuff");
    desc.SetComponentFlags({IEntityComponent::EFlags::Transform, IEntityComponent::EFlags::Socket, IEntityComponent::EFlags::Attach});
	desc.AddMember(&CVehicleComponent::sVehicleProperties, 'svp', "VehicleProperties", "Vehicle Properties", "Sets all of the vehicle properties", SVehicleProperties());


}

void CVehicleComponent::LoadGeometry() {

	string sGeomPath = GetProperties()->sRenderProperties.sGeomPath.value.c_str();

	if (sGeomPath.empty())
		return;

	m_pEntity->LoadGeometry(0, sGeomPath);

}
void CVehicleComponent::Physicalize() {

	SEntityPhysicalizeParams physParams;

	physParams.mass = GetProperties()->sPhysicsProperties.fMass;
	physParams.type = PE_WHEELEDVEHICLE;
	m_pEntity->Physicalize(physParams);

}

bool CVehicleComponent::ClEnterVehicle(SEnterParams && p, INetChannel * pNetChannel) {
	return true;
}

bool CVehicleComponent::ClLeaveVehicle(SLeaveParams && p, INetChannel * pNetChannel) {
	return true;
}

bool CVehicleComponent::SvEnterVehicle(SEnterParams && p, INetChannel * pNetChannel) {

	//Get the vehicle entity
	IEntity *pVehicle = gEnv->pEntitySystem->GetEntity(p.VecId);

	//If it can get the vehicle component from the entity, continue
	if (CVehicleComponent *pVehicleComponent = pVehicle->GetComponent<CVehicleComponent>()) {

		//If the vehicle isn't full, continue
		if (pVehicleComponent->iCurrentPassengers < pVehicleComponent->GetProperties()->iMaxPassengers) {

			//If it doesn't have a drivet, continue
			if (!pVehicleComponent->bHasDriver) {



			}
			else {

			}

		}
		else {
			return false;
		}

	}

	return false;
}

bool CVehicleComponent::SvLeaveVehicle(SLeaveParams && p, INetChannel * pNetChannel) {
	return false;
}

void CVehicleComponent::RequestEnter(IEntity *pNewPassenger, IEntity *pVehicle) {

	//If it's not server, continue
	if (!gEnv->bServer) {
		//Gets the new passengers EntityId
		EntityId PassId = pNewPassenger->GetId();
		EntityId VecId = pVehicle->GetId();
		//Sends a request to the server
		SvEnterVehicleRMI::InvokeOnServer(this, SEnterParams{ PassId, VecId });
	
	}


}

void CVehicleComponent::RequestLeave(IEntity *pPassenger, IEntity *pVehicle) {
}
