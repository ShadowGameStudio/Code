#include "StdAfx.h"

#include "VehicleComponent.h"
#include <CryEntitySystem/IEntitySystem.h>
#include <DefaultComponents/Physics/Vehicles/VehicleComponent.h>

//Network shorts
using SvEnterVehicleRMI = SRmi<RMI_WRAP(&CVehicleComponent::SvEnterVehicle)>;
using SvLeaveVehicleRMI = SRmi<RMI_WRAP(&CVehicleComponent::SvLeaveVehicle)>;


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
   
	CreateVehicleName();

	//Enter RMI(Server)
	{
		const bool bIsServerCall = true;
		const ERMIAttachmentType attachmentType = eRAT_NoAttach;
		const ENetReliabilityType reliability = eNRT_UnreliableOrdered;

		SvEnterVehicleRMI::Register(this, attachmentType, bIsServerCall, reliability);

	}

	//Leave RMI(Server)
	{
		const bool bIsServerCall = true;
		const ERMIAttachmentType attachmentType = eRAT_NoAttach;
		const ENetReliabilityType reliability = eNRT_UnreliableOrdered;

		SvLeaveVehicleRMI::Register(this, attachmentType, bIsServerCall, reliability);

	}
	
	//Enables the item to have it's aspects delegated
	m_pEntity->GetNetEntity()->EnableDelegatableAspect(GetNetSerializeAspectMask(), true);
	m_pEntity->GetNetEntity()->EnableDelegatableAspect(eEA_Physics, false);
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

bool CVehicleComponent::SvEnterVehicle(SEnterParams && p, INetChannel * pNetChannel) {

	//Get the vehicle entity
	IEntity *pVehicle = gEnv->pEntitySystem->GetEntity(p.VecId);

	//If it can get the vehicle component from the entity, continue
	if (CVehicleComponent *pVehicleComponent = pVehicle->GetComponent<CVehicleComponent>()) {

		//If the vehicle isn't full, continue
		if (pVehicleComponent->iCurrentPassengers < pVehicleComponent->GetProperties()->iMaxPassengers) {

			//If it doesn't have a driver, continue
			if (!pVehicleComponent->bHasDriver) {

				//Gets the player entity
				IEntity *pPlayer = gEnv->pEntitySystem->GetEntity(p.PassId);

				//Gets the location of the drivers seat
				Vec3 vehiclePos = pVehicle->GetWorldTM().GetTranslation();
				Vec3 driverSeat = vehiclePos + GetProperties()->vDriversSeat;

				//Sets the players position to the driver seat
				pPlayer->SetPos(driverSeat);

				//Adds the new player to the count
				pVehicleComponent->iCurrentPassengers++;
				//Sets that it now has a driver.
				pVehicleComponent->bHasDriver = true;

				//Get the players PlayerComponent
				if (CPlayerComponent *pPlayerComp = pPlayer->GetComponent<CPlayerComponent>()) {

					//Set that the player now is in a vehicle
					pPlayerComp->bIsDriver = true;
					pPlayerComp->bIsPassenger = false;

					pPlayerComp->pPlayerVehicle = m_pEntity;

					DelegateAuthorityToClient(p.VecId, p.channelId);

				}

			}
			else {

				//Move player to the next slot

				//Adds the new player to the count
				pVehicleComponent->iCurrentPassengers++;

			}

		}
		else {

			//Give the player a message that the vehicle is full
			Vec2 screenMid = { gEnv->pRenderer->GetWidth() / 2.f, gEnv->pRenderer->GetHeight() / 2.f };
			ColorF pfWhite = { 1.f, 1.f, 1.f, 1.f };
			string sMessage = GetVehicleName();
			gEnv->pRenderer->GetIRenderAuxGeom()->Draw2dLabel(screenMid.x, screenMid.y, 1.5f, pfWhite, true, sMessage + "is full!");

			return false;
		}

	}

	return false;
}

bool CVehicleComponent::SvLeaveVehicle(SLeaveParams && p, INetChannel * pNetChannel) {

	//Get the vehicle
	IEntity *pVehicle = gEnv->pEntitySystem->GetEntity(p.VecId);

	//Get the vehicle component
	if (CVehicleComponent *pVehicleComp = pVehicle->GetComponent<CVehicleComponent>()) {
	
		//Get the player entity
		IEntity *pPlayer = gEnv->pEntitySystem->GetEntity(p.PassId);

		//Get vehicle pos
		Vec3 vehiclePos = pVehicle->GetWorldTM().GetTranslation();
		Vec3 dropLoc = vehiclePos + Vec3(3, 0, 0);

		//Set the players position to outside the vehicle
		pPlayer->SetPos(dropLoc);

		//Adds the new player to the count
		pVehicleComp->iCurrentPassengers--;
		//Sets that it now has a driver.
		pVehicleComp->bHasDriver = false;

		if (CPlayerComponent *pPlayerComp = pPlayer->GetComponent<CPlayerComponent>()) {

			//Set that the player now is in a vehicle
			pPlayerComp->bIsDriver = false;
			pPlayerComp->bIsPassenger = false;

			pPlayerComp->pPlayerVehicle = nullptr;

		}

	}

	return false;
}

void CVehicleComponent::DelegateAuthorityToClient(const EntityId controlledEntity, const uint16 channelId) {

	INetChannel* pNetChannel = gEnv->pGameFramework->GetNetChannel(channelId);
	gEnv->pGameFramework->GetNetContext()->DelegateAuthority(controlledEntity, pNetChannel);

}

void CVehicleComponent::RequestEnter(IEntity *pNewPassenger, IEntity *pVehicle) {

	if (CPlayerComponent *pPlayer = pNewPassenger->GetComponent<CPlayerComponent>()) {

		if (pPlayer->bIsDriver || pPlayer->bIsPassenger) {

			RequestLeave(pNewPassenger, pVehicle);

		}
		else {

			//Gets the new passengers EntityId
			EntityId PassId = pNewPassenger->GetId();
			EntityId VecId = pVehicle->GetId();
			uint16 channelId = pNewPassenger->GetNetEntity()->GetChannelId();
			//Sends a request to the server
			SvEnterVehicleRMI::InvokeOnServer(this, SEnterParams{ PassId, VecId, channelId });

		}

	}

}

void CVehicleComponent::RequestLeave(IEntity *pPassenger, IEntity *pVehicle) {

	EntityId PassId = pPassenger->GetId();
	EntityId VecId = pVehicle->GetId();

	SvLeaveVehicleRMI::InvokeOnServer(this, SLeaveParams{ PassId, VecId });

}

void CVehicleComponent::CreateVehicleName() {

	string sLongName = m_pEntity->GetClass()->GetName();

	sLongName.erase(0, sLongName.find_last_of(':') + 1);
	string sFirst;
	sFirst.SetAt(0, sLongName[0]);
	sFirst.MakeUpper();
	sLongName.erase(0, 1);
	GetProperties()->sVehicleName = sFirst + sLongName;
}

bool CVehicleComponent::NetSerialize(TSerialize ser, EEntityAspects aspect, uint8 profile, int flags) {
	return false;
}
