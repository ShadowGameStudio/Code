#include "StdAfx.h"
#include "DrugComponent.h"
#include <CrySchematyc\Env\Elements\EnvComponent.h>

static void RegisterFoodComponent(Schematyc::IEnvRegistrar& registrar) {
	Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
	{
		Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(CDrugComponent));
		// Functions
		{
		}
	}
}
CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterFoodComponent)

void CDrugComponent::InitializeClass() {}

void CDrugComponent::ProcessEventClass(const SEntityEvent & event) {

	switch (event.event) {
	case ENTITY_EVENT_TIMER:

		if (event.nParam[0] == Timer_Consume) {
			//If it can get the health component, continue
			if (CHealthComponent *pHealth = m_pEntity->GetComponent<CHealthComponent>()) {
				//Sets that there is no consume timer
				pHealth->bTimerSet = false;

			}

		}

		break;
	}

}

void CDrugComponent::ReflectType(Schematyc::CTypeDesc<CDrugComponent>& desc) {

	desc.SetGUID("{292E0DEF-719A-4DDA-9A6F-296AA85A0938}"_cry_guid);
	desc.AddBase<SItemComponent>();
	desc.SetEditorCategory("Items");
	desc.SetLabel("Drug Component");
	desc.SetDescription("Handels all of the drugs functionality");

}

void CDrugComponent::Consume(SItemComponent *pItemToConsume) {

	//Sets the regen timer
	m_pEntity->SetTimer(Timer_Consume, GetDrugProperties()->consumeTimer);
	//If it can get the players health component, continue
	if (CHealthComponent *pHealth = m_pEntity->GetComponent<CHealthComponent>()) {
		//Set the regeneration ration to the on defined in the drugs settings
		pHealth->SetRegenerationRatio(GetDrugProperties()->fRegenerateRatio);
		//Sets that the timer has been set
		pHealth->bTimerSet = true;
		
	}

}
