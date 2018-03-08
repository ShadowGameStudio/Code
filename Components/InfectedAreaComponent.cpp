#include "StdAfx.h"

#include "InfectedAreaComponent.h"
#include "GasmaskComponent.h"
#include <CrySchematyc\Env\Elements\EnvComponent.h>
#include <CrySchematyc\Env\IEnvRegistrar.h>
#include <CrySchematyc\Env\Elements\EnvFunction.h>

static void RegisterInfectedAreaComponent(Schematyc::IEnvRegistrar& registrar) {
	Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
	{
		Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(CInfectedAreaComponent));
		// Functions
		{
			//Exposes the Entering function to Schematyc so it can be called from there
			auto pEntering = SCHEMATYC_MAKE_ENV_FUNCTION(&CInfectedAreaComponent::Entering, "{C0D4C3C9-8099-4CD7-B1FE-EC3043A36E6F}"_cry_guid, "Entering");
			pEntering->SetDescription("Should be called when player is entering an area");
			pEntering->SetFlags({ Schematyc::EEnvFunctionFlags::Member, Schematyc::EEnvFunctionFlags::Construction });
			pEntering->BindInput(1, 'eid', "EntityId");
			componentScope.Register(pEntering);

			//Exposes the Leaving function to Schematyc so it can be called from there
			auto pLeaving = SCHEMATYC_MAKE_ENV_FUNCTION(&CInfectedAreaComponent::Leaving, "{76FB4A45-21D8-42DB-9900-0B436F8809F8}"_cry_guid, "Leaving");
			pLeaving->SetDescription("Should be called when player is leaving an area");
			pLeaving->SetFlags({ Schematyc::EEnvFunctionFlags::Member, Schematyc::EEnvFunctionFlags::Construction });
			pLeaving->BindInput(1, 'eid', "EntityId");
			componentScope.Register(pLeaving);

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

//Called when a player is leaving an infected area
void CInfectedAreaComponent::Entering(EntityId Id) {

	//Set that there is a player inside
	bIsInside = true;
	//Increment the player count inside the area
	iPlayerCount++;

	//If it can get the players entity, continue
	if (IEntity *pPlayer = gEnv->pEntitySystem->GetEntity(Id)) {
		//If it can get the players PlayerComponent, continue
		if (CPlayerComponent *pPlayerComponent = pPlayer->GetComponent<CPlayerComponent>()) {
			//If the player is wearing a gasmask, continue
			if (pPlayerComponent->bPlayerIsWearingGasmask == true) {
				//If it can get the players inventory, continue
				if (CInventoryComponent *pInventory = pPlayer->GetComponent<CInventoryComponent>()) {
					//Gets the current gasmask
					SItemComponent *pGasmaskItem = pInventory->GetGasmask();
					//Gets the items gasmask component
					CGasmaskComponent *pGasmask = pGasmaskItem->GetEntity()->GetComponent<CGasmaskComponent>();
					//If the gasmask is equal to or grater than the desired level at the area, continue
					if (pGasmask->GetGasmaskProperties()->iGasmaskLevel >= GetInfectedProperties()->iGasmaskLevel) {
						//Get the players health component
						CHealthComponent *pPlayerHealth = pPlayer->GetComponent<CHealthComponent>();
						//Remove the defined damage
						//TODO: Make it check every second
						pPlayerHealth->Add((-GetInfectedProperties()->fInfectedAreaDamage));

					}
					else {

					}

				}

			}
			//If it's not, continue
			else {



			}

		}
	}

}

//Called when a player is leaving an infected area
void CInfectedAreaComponent::Leaving(EntityId Id) {

	//If the player count is equal to zero it will turn the bIsInsibe boll false
	if (iPlayerCount == 0)
		//Turn the variable false
		bIsInside = false;
	else
		//Else remove one from the player count
		iPlayerCount--;



}

