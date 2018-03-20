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
			pEntering->SetFlags({ Schematyc::EEnvFunctionFlags::Member });
			pEntering->BindInput(1, 'eid', "EntityId", "The entity id of the player leaving", Schematyc::ExplicitEntityId());
			componentScope.Register(pEntering);

			//Exposes the Leaving function to Schematyc so it can be called from there
			auto pLeaving = SCHEMATYC_MAKE_ENV_FUNCTION(&CInfectedAreaComponent::Leaving, "{76FB4A45-21D8-42DB-9900-0B436F8809F8}"_cry_guid, "Leaving");
			pLeaving->SetDescription("Should be called when player is leaving an area");
			pLeaving->SetFlags({ Schematyc::EEnvFunctionFlags::Member });
			pLeaving->BindInput(1, 'eid', "EntityId", "The entity id of the player leaving");
			componentScope.Register(pLeaving);

		}
	}
}

CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterInfectedAreaComponent )

void CInfectedAreaComponent::Initialize() {}

uint64 CInfectedAreaComponent::GetEventMask() const {
	return ENTITY_EVENT_BIT(ENTITY_EVENT_TIMER) | ENTITY_EVENT_BIT(ENTITY_EVENT_UPDATE);
}

void CInfectedAreaComponent::ProcessEvent(const SEntityEvent & event) {

	switch (event.event) {
		case ENTITY_EVENT_TIMER:
		{
			//When the timer has run out, continue
			if (event.nParam[0] == Timer_Damage) {
				//For as many players there is in the area
				for (int i = 0; i < iPlayerCount; i++) {

					IEntity *pPlayer = pPlayerCount[i];
					//If damage mode is zero, return
					if (iDamageMode == 0) {
						return;
					}
					//Else if it's 1, continue
					else if (iDamageMode == 1) {
						//Gets the players health component
						CHealthComponent *pHealth = pPlayer->GetComponent<CHealthComponent>();
						//Remove health from the player
						pHealth->Add((-GetInfectedProperties()->fDamage));
					}
					//Else if it's 2, continue
					else if (iDamageMode == 2) {
						//Gets the players health component
						CHealthComponent *pHealth = pPlayer->GetComponent<CHealthComponent>();
						//Remove health from the player
						pHealth->Add((-GetInfectedProperties()->fWithMaskDamage));
					}

				}

			}
		}
		break;

		case ENTITY_EVENT_UPDATE:
		{
			SEntityUpdateContext *pCtx = (SEntityUpdateContext*)event.nParam[0];
			Update(pCtx->fFrameTime);
		}
		break;
	}

}

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
	//Calls the function that adds the player to the area and such
	PlayerEntered(Id);

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

	//If it can get the players entity, continue
	if (IEntity *pPlayer = gEnv->pEntitySystem->GetEntity(Id)) {
		//Find the player in the vector
		auto player = std::find(pPlayerCount.begin(), pPlayerCount.end(), pPlayer);
		//Set the distance to it to an index int
		auto index = std::distance(pPlayerCount.begin(), player);
		//Erase the player from the vector
		pPlayerCount.erase(pPlayerCount.begin() + (index - 1));

	}

}

//Called when a player has entered the area
void CInfectedAreaComponent::PlayerEntered(EntityId Id) {

	//If it can get the players entity, continue
	if (IEntity *pPlayer = gEnv->pEntitySystem->GetEntity(Id)) {
		//Add the player to the vector
		pPlayerCount.push_back(pPlayer);
		//If it can find the player, continue
		if (std::find(pPlayerCount.begin(), pPlayerCount.end(), pPlayer) != pPlayerCount.end()) {
			//If it can get the players PlayerComponent, continue
			if (CPlayerComponent *pPlayerComponent = pPlayer->GetComponent<CPlayerComponent>()) {
				//If the player is wearing a gasmask, continue
				if (pPlayerComponent->bPlayerIsWearingGasmask == true) {
					//It it can get the players inventory, continue
					if (CInventoryComponent *pInventory = pPlayer->GetComponent<CInventoryComponent>()) {
						//Get the gasmask item
						SItemComponent *pGasmaskItem = pInventory->GetGasmask();
						//Get the gasmask component
						CGasmaskComponent *pGasmask = pGasmaskItem->GetEntity()->GetComponent<CGasmaskComponent>();
						//If the gasmasks level is greater or equal to the infected areas gasmask level, continue
						if (pGasmask->GetGasmaskProperties()->iGasmaskLevel >= GetInfectedProperties()->iGasmaskLevel) {
							//TODO: Check all the time
							//Turns of the players ability to take damage
							pPlayerComponent->bCanTakeInfectedDamage = false;
							//Set the damage mode to zero
							iDamageMode = 0;
						}
						//If it's not, continue
						else {
							//Turns of the players ability to take damage
							pPlayerComponent->bCanTakeInfectedDamage = true;
							//Set the damage to low
							iDamageMode = 1;
						}

					}

				}
				//It it's not, continue
				else {
					//Turns of the players ability to take damage
					pPlayerComponent->bCanTakeInfectedDamage = true;
					//Set the damage mode to high
					iDamageMode = 2;
				}

			}

		}

	}

}

//Called every frame
void CInfectedAreaComponent::Update(float fFrameTime) {

	//Start the damage timer
	if(bIsInside)
		m_pEntity->SetTimer(Timer_Damage, 500);

}

