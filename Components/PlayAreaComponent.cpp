#include "StdAfx.h"
#include "PlayAreaComponent.h"
#include "PlayAreaSpawnerComponent.h"
#include "SpawnPoint.h"
#include <CryEntitySystem/IEntitySystem.h>
#include <CryNetwork/Rmi.h>

using ClDamagePlayerRMI = SRmi<RMI_WRAP(&CPlayAreaComponent::ClDamagePlayer)>;

static void RegisterPlayAreaComponent(Schematyc::IEnvRegistrar& registrar) {
	Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
	{
		Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(CPlayAreaComponent));
		// Functions
		{
		}
	}
}

CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterPlayAreaComponent)

void CPlayAreaComponent::Initialize() {

	//ClDamagePlayerRMI
	{
		const bool bIsServerCall = false;
		const ERMIAttachmentType attachmentType = eRAT_NoAttach;
		const ENetReliabilityType reliability = eNRT_UnreliableUnordered;

		ClDamagePlayerRMI::Register(this, attachmentType, bIsServerCall, reliability);
	}

	//Binds the entity to the network
	m_pEntity->GetNetEntity()->BindToNetwork();

	m_pEntity->GetNetEntity()->EnableDelegatableAspect(eEA_Physics, false);

}

uint64 CPlayAreaComponent::GetEventMask() const {
	return ENTITY_EVENT_BIT(ENTITY_EVENT_TIMER) | ENTITY_EVENT_BIT(ENTITY_EVENT_UPDATE) | ENTITY_EVENT_BIT(ENTITY_EVENT_LEVEL_LOADED);
}

void CPlayAreaComponent::ProcessEvent(const SEntityEvent & event) {

	switch (event.event) {
	case ENTITY_EVENT_TIMER:

		if (event.nParam[0] == Timer_Decrease) {

			//Changes the scale. Called every frame
			if (fScaleX >= fScaleTo) {
				fScaleX -= fDecreaseAmount;
				m_pEntity->SetScale(Vec3(fScaleX, fScaleY, fScaleZ));
			}
			else
				bIsDecreased = true;
		}

		if (event.nParam[0] == Timer_DecreaseAfter) {
			bTimerSet = false;
			bIsDecreased = false;

			//Decreses the size of the play area
			if (iTimesDecreased == 0) {
				fScaleTo -= 5.f;
				iTimesDecreased++;
			}
			else if (iTimesDecreased == 1) {
				fScaleTo -= 5.f;
				iTimesDecreased++;
			}
			else if (iTimesDecreased == 2) {
				fScaleTo -= 4.f;
				iTimesDecreased++;
			}

		}

		if (event.nParam[0] == Timer_Damage) {

			bTimerDamageSet = false;

			int channelId = pPlayerToDamage->GetNetEntity()->GetChannelId();
			EntityId Id = pPlayerToDamage->GetId();

			ClDamagePlayerRMI::InvokeOnClient(this, SDamageParams{ Id }, channelId);


		}

		break;

	case ENTITY_EVENT_LEVEL_LOADED:
		//	SpawnPlayArea();
		break;

	case ENTITY_EVENT_UPDATE:

		//Set up Update function
		SEntityUpdateContext* pCtx = (SEntityUpdateContext*)event.nParam[0];
		Update(pCtx->fFrameTime);

	}


}

void CPlayAreaComponent::ReflectType(Schematyc::CTypeDesc<CPlayAreaComponent>& desc) {

	desc.SetGUID("{85B3587A-E3BA-4F9B-8C60-8B6876A6404D}"_cry_guid);
	desc.SetEditorCategory("Gameplay");
	desc.SetLabel("Play Area Component");
	desc.SetDescription("Play Area Component");
	desc.SetComponentFlags({ IEntityComponent::EFlags::Transform, IEntityComponent::EFlags::Socket, IEntityComponent::EFlags::Attach });


}

void CPlayAreaComponent::DamagePlayer(IEntity* pPlayer) {

	//If player is not in area
	if (pPlayer) {

		if (!bTimerDamageSet) {
			m_pEntity->SetTimer(Timer_Damage, 1000);
			pPlayerToDamage = pPlayer;
			bTimerDamageSet = true;
		}

	}

}

bool CPlayAreaComponent::ClDamagePlayer(SDamageParams && p, INetChannel * pNetChannel) {

	IEntity* pPlayer = gEnv->pEntitySystem->GetEntity(p.Id);

	if (CHealthComponent* pHealth = pPlayer->GetComponent<CHealthComponent>()) {

		pHealth->Add((-10.f));

	}

	return true;
}

//Makes the play area smaller over time
void CPlayAreaComponent::DecreasePlayArea() {

	//Decreases the size on the Play Area
	//Makes sure that the Play Area only decreases by fDecreaseAmount every 1ms

	//Checks if it already has been decreased or not
	if(!bIsDecreased)
		m_pEntity->SetTimer(Timer_Decrease, 1);
	else if (!bTimerSet) {
		m_pEntity->SetTimer(Timer_DecreaseAfter, iDecreaseTime);
		bTimerSet = true;
	}
}

void CPlayAreaComponent::Update(float frameTime) {

	//Calls DecreasePlayArea every frame
	if (IEntity* pEntity = gEnv->pEntitySystem->FindEntityByName("PlayArea")) {
			
		if (CPlayAreaComponent *pPlayArea = pEntity->GetComponent<CPlayAreaComponent>()) {
				pPlayArea->DecreasePlayArea();
		}

	}

}

