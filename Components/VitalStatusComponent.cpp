#include "StdAfx.h"
#include "VitalStatusComponent.h"
#include <CryEntitySystem\IEntity.h>
#include <CrySchematyc\Env\IEnvRegistrar.h>
#include <CrySchematyc\Env\Elements\EnvComponent.h>

void SVitalStatusComponent::Initialize() {

	props.fValue = props.fValue;
	props = props;

}

uint64 SVitalStatusComponent::GetEventMask() const {
	return BIT64(ENTITY_EVENT_UPDATE);
}

void SVitalStatusComponent::ReflectType(Schematyc::CTypeDesc<SVitalStatusComponent>& desc) {

	desc.SetGUID("{F92A24C4-90A8-4783-B88F-7B73DCE55AAD}"_cry_guid);

}

void SVitalStatusComponent::Regenerate() {

	if (props.fValue < props.fMax)
		props.fValue += props.fRegenerationRatio;

}
