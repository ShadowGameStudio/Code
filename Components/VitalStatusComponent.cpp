#include "StdAfx.h"
#include "VitalStatusComponent.h"
#include <CryEntitySystem\IEntity.h>
#include <CrySchematyc\Env\IEnvRegistrar.h>
#include <CrySchematyc\Env\Elements\EnvComponent.h>

void SVitalStatusComponent::Initialize() {

	props.fValue = props.fMax;
	props = props;
	InitializeClass();

}

uint64 SVitalStatusComponent::GetEventMask() const {
	return ENTITY_EVENT_BIT(ENTITY_EVENT_UPDATE);
}

void SVitalStatusComponent::ProcessEvent(const SEntityEvent & event) {

	ProcessClassEvent(event);

}

void SVitalStatusComponent::ReflectType(Schematyc::CTypeDesc<SVitalStatusComponent>& desc) {

	desc.SetGUID("{F92A24C4-90A8-4783-B88F-7B73DCE55AAD}"_cry_guid);

}

//Regenerates health and/or Stamina
void SVitalStatusComponent::Regenerate() {

	//If it should regenerate, continue
	if (bRegenerate) {

		bRegenerate = props.fValue == props.fLastValue;
		props.fLastValue = props.fValue;
		
		if (auto *pUI = m_pEntity->GetComponent<CUIComponent>()) {
			pUI->SetHealthbar();
		}

		if (props.fValue < props.fMax) {
			props.fValue += props.fRegenerationRatio;
			if (auto *pUI =  m_pEntity->GetComponent<CUIComponent>()) {
				pUI->SetHealthbar();
			}
		}

	}

}
