#include "StdAfx.h"
#include "ItemComponent.h"
#include <CrySchematyc\Env\Elements\EnvComponent.h>

//Network shorts
using ServerPickupRMI = SRmi<RMI_WRAP(&SItemComponent::ServerPickup)>;
using ClientPickupRMI = SRmi<RMI_WRAP(&SItemComponent::ClientPickup)>;


 void SItemComponent::Initialize() {
	 
	//Load basic item stuff
	LoadGeometry();
	Physicalize();
	CreateItemName();
	InitializeClass();

	//Pickup RMI(Server)
	{
		const bool bIsServerCall = true;
		const ERMIAttachmentType attachmentType = eRAT_NoAttach;
		const ENetReliabilityType reliability = eNRT_UnreliableOrdered;

		ServerPickupRMI::Register(this, attachmentType, bIsServerCall, reliability);
	}
	//Pickup RMI(Client)
	{
		const bool bIsServerCall = false;
		const ERMIAttachmentType attachmentType = eRAT_NoAttach;
		const ENetReliabilityType reliability = eNRT_UnreliableOrdered;

		ClientPickupRMI::Register(this, attachmentType, bIsServerCall, reliability);
	}

	//Enables the item to have it's aspects delegated
	m_pEntity->GetNetEntity()->EnableDelegatableAspect(GetNetSerializeAspectMask(), true);
	//If it's server, continue
	if (gEnv->bServer) {
		//Binds the object to the network
		gEnv->pNetContext->BindObject(GetEntityId(), 0, GetNetSerializeAspectMask(), true);
	}
	//Bind the entity to network
	m_pEntity->GetNetEntity()->BindToNetwork();

}
 
 uint64 SItemComponent::GetEventMask() const {
	 return ENTITY_EVENT_BIT(ENTITY_EVENT_COMPONENT_PROPERTY_CHANGED) | ENTITY_EVENT_BIT(ENTITY_EVENT_COLLISION) | ENTITY_EVENT_BIT(ENTITY_EVENT_TIMER);
}
 
 void SItemComponent::ProcessEvent(const SEntityEvent & event) {
	 
	switch (event.event) {
	case ENTITY_EVENT_COMPONENT_PROPERTY_CHANGED:
	{
				 
		if (sItemProperties != sPrevItemProperties) {
					 
			sPrevItemProperties = sItemProperties;
					 
			LoadGeometry();
			Physicalize();	 
		}
	}
	break;		 
	}
	ProcessEventClass(event);
}
 
 void SItemComponent::ReflectType(Schematyc::CTypeDesc<SItemComponent>& desc) {
	 
	desc.SetGUID("{A77F1211-9E03-4697-9E04-DC35F6E15B89}"_cry_guid);

}
 
 void SItemComponent::LoadGeometry() {
	 
	string sGeomPath = GetProperties()->sRenderProperties.sGeomPath.value.c_str();
	 
	if (sGeomPath.empty())
		return;
	 
	m_pEntity->LoadGeometry(GEOMETRY_SLOT, sGeomPath);
	 	 
}
 
 void SItemComponent::Physicalize() {
	 
	SEntityPhysicalizeParams physParams;
	physParams.mass = GetProperties()->sPhysicsProperties.fMass;
	physParams.type = PE_RIGID;
	m_pEntity->Physicalize(physParams);
		 
}
 
 void SItemComponent::CreateItemName() {
	 
	string sLongName = m_pEntity->GetClass()->GetName();

	sLongName.erase(0, sLongName.find_last_of(':') + 1);
	string sFirst;
	sFirst.SetAt(0, sLongName[0]);
	sFirst.MakeUpper();
	sLongName.erase(0, 1);
	GetProperties()->sItemName = sFirst + sLongName;
	 
		 
}

 void SItemComponent::Drop(SItemComponent *pWeaponToDrop) {
	 
	if (!pOwnerEntity)
		return;
	
	//If it owner entity has PlayerComponent, continue
	if (CPlayerComponent *pOwnerPlayer = pOwnerEntity->GetComponent<CPlayerComponent>()) {
		//Detach the weapon from players hand
		pOwnerPlayer->GetInventory()->DetachFromHand();

		//Remove the weapon from the inventory
		pOwnerPlayer->GetInventory()->RemoveItem(pWeaponToDrop);
	}
	
	 
	//Stop ignoring owner collision
	pe_action_update_constraint constraintUpdate;
	constraintUpdate.bRemove = 1;
	constraintUpdate.idConstraint = iChildConstraintId;
	m_pEntity->GetPhysicalEntity()->Action(&constraintUpdate);
	 
	constraintUpdate.idConstraint = iOwnerConstraintId;
	pOwnerEntity->GetPhysicalEntity()->Action(&constraintUpdate);
	 
	pe_action_impulse impulse;
	impulse.impulse = pOwnerEntity->GetForwardDir();
	 
	m_pEntity->GetPhysicalEntity()->Action(&impulse);
	 
	pOwnerEntity = nullptr; 
}
 
 bool SItemComponent::IsPickable() {
	 
	 return !pOwnerEntity;	 
}

 //Pickup called on the server
 bool SItemComponent::ServerPickup(SPickUpParams && p, INetChannel * pNetChannel) {
	 //Invoke the pickup on all the clients
	 ClientPickupRMI::InvokeOnAllClients(this, SPickUpParams{ p.Id });
	 
	 return true;
 }

 bool SItemComponent::ClientPickup(SPickUpParams && p, INetChannel * pNetChannel) {

	 //Gets the entity from the entity id
	 IEntity *pNewOwner = gEnv->pEntitySystem->GetEntity(p.Id);

	 if (!pNewOwner)
		 return false;

	 pOwnerEntity = pNewOwner;
	 pOwnerEntity->AttachChild(m_pEntity);


	 //filter collision

	 pe_action_add_constraint constraint;
	 constraint.pt[0] = ZERO;
	 constraint.flags = constraint_ignore_buddy | constraint_inactive;
	 constraint.pBuddy = pOwnerEntity->GetPhysicalEntity();
	 iChildConstraintId = m_pEntity->GetPhysicalEntity()->Action(&constraint);

	 //add collision filtering to owner

	 constraint.flags |= constraint_inactive;
	 constraint.pBuddy = m_pEntity->GetPhysicalEntity();
	 iOwnerConstraintId = pOwnerEntity->GetPhysicalEntity()->Action(&constraint);

	 return true;
 }

 //Called when the player wants to pickup
 void SItemComponent::RequestPickup(IEntity *pNewOwner) {
 
	 //It if already is server, continue
	 if (gEnv->bServer) {
		 
		 if (!pNewOwner)
			 return;

		 pOwnerEntity = pNewOwner;
		 pOwnerEntity->AttachChild(m_pEntity);


		 //filter collision

		 pe_action_add_constraint constraint;
		 constraint.pt[0] = ZERO;
		 constraint.flags = constraint_ignore_buddy | constraint_inactive;
		 constraint.pBuddy = pOwnerEntity->GetPhysicalEntity();
		 iChildConstraintId = m_pEntity->GetPhysicalEntity()->Action(&constraint);

		 //add collision filtering to owner

		 constraint.flags |= constraint_inactive;
		 constraint.pBuddy = m_pEntity->GetPhysicalEntity();
		 iOwnerConstraintId = pOwnerEntity->GetPhysicalEntity()->Action(&constraint);

	 }
	 //If it's not, continue
	 else {
		 //Get the new item owners EntityId
		 EntityId Id = pNewOwner->GetId();
		 //Send the request to the server
		 ServerPickupRMI::InvokeOnServer(this, SPickUpParams{ Id });
	 }

 }
