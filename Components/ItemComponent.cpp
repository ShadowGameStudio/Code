#include "StdAfx.h"
#include "ItemComponent.h"
#include <CrySchematyc\Env\Elements\EnvComponent.h>

 void SItemComponent::Initialize() {
	 
	//Load basic item stuff
	LoadGeometry();
	Physicalize();
	CreateItemName();
	InitializeClass();
}
 
 uint64 SItemComponent::GetEventMask() const {
	 return BIT64(ENTITY_EVENT_COMPONENT_PROPERTY_CHANGED) | BIT64(ENTITY_EVENT_COLLISION) | BIT64(ENTITY_EVENT_TIMER);
}
 
 void SItemComponent::ProcessEvent(SEntityEvent & event) {
	 
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

 void SItemComponent::Drop() {
	 
	if (!pOwnerEntity)
		return;
	 
	CPlayerComponent * pOwnerPlayer = pOwnerEntity->GetComponent<CPlayerComponent>();
	 
	pOwnerPlayer->GetInventory()->DetachFromHand();
	 
		 	//Stop ignoring owner collision
	pe_action_update_constraint constraintUpdate;
	constraintUpdate.bRemove = 1;
	constraintUpdate.idConstraint = iChildConstraintId;
	m_pEntity->GetPhysicalEntity()->Action(&constraintUpdate);
	 
	constraintUpdate.idConstraint = iOwnerConstraintId;
	pOwnerEntity->GetPhysicalEntity()->Action(&constraintUpdate);
	 
	pe_action_impulse impulse;
	impulse.impulse = pOwnerEntity->GetForwardDir() * 1000.f;
	 
	m_pEntity->GetPhysicalEntity()->Action(&impulse);
	 
	pOwnerEntity = nullptr; 
}
 
 bool SItemComponent::IsPickable() {
	 
	 return !pOwnerEntity;	 
}
