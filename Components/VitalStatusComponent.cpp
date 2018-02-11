#include "StdAfx.h"
#include "VitalStatusComponent.h"
#include <CryEntitySystem\IEntity.h>
#include <CrySchematyc\Env\IEnvRegistrar.h>
#include <CrySchematyc\Env\Elements\EnvComponent.h>

void SVitalStatusComponent::Initialize() {

	props.fValue = props.fValue;
	props = props;
	InitializeClass();

}

uint64 SVitalStatusComponent::GetEventMask() const {
	return BIT64(ENTITY_EVENT_UPDATE);
}

void SVitalStatusComponent::ProcessEvent(SEntityEvent & event) {

	ProcessClassEvent(event);

}

void SVitalStatusComponent::ReflectType(Schematyc::CTypeDesc<SVitalStatusComponent>& desc) {

	desc.SetGUID("{F92A24C4-90A8-4783-B88F-7B73DCE55AAD}"_cry_guid);

}

//Regenerates health and/or Stamina
void SVitalStatusComponent::Regenerate() {

	bool bRegenerate = true;

	//Check if it always should regenerate or not
	//Such as regenerating while sprinting, otherwise it will not do that

	if (!props.bAlwaysRegenerate) {

		bRegenerate = props.fValue == props.fLastValue;
		props.fLastValue = props.fValue;
	
	}

	if (bRegenerate) {

		if (props.fValue < props.fMax)
			props.fValue += props.fRegenerationRatio;
	
	}


}
