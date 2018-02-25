#include "StdAfx.h"
#include "Player.h"

#include "Bullet.h"
#include "SpawnPoint.h"
#include "FlashUI\FlashUI.h"

#include <CryRenderer/IRenderAuxGeom.h>
#include "CrySchematyc\Env\Elements\EnvComponent.h"
#include "CryNetwork\Rmi.h"
#include "ItemComponent.h"
#include "InventoryComponent.h"

static void RegisterPlayer(Schematyc::IEnvRegistrar& registrar) {
	Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
	{
		Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(CPlayerComponent));
		// Functions
		{
		}
	}
}

CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterPlayer)

void CPlayerComponent::Initialize()
{

	// Get the input component, wraps access to action mapping so we can easily get callbacks when inputs are triggered
	m_pInputComponent = m_pEntity->GetOrCreateComponent<Cry::DefaultComponents::CInputComponent>();
	m_pHealthComponent = m_pEntity->GetOrCreateComponentClass<CHealthComponent>();

	//Sets health, stamina and such
	SetPlayerParams();

	m_pCharacterController = m_pEntity->GetOrCreateComponent<Cry::DefaultComponents::CCharacterControllerComponent>();
	m_pCharacterController->SetTransformMatrix(Matrix34::Create(Vec3(1.f), IDENTITY, Vec3(0, 0, 1.f)));

	m_pAnimationComponent = m_pEntity->GetOrCreateComponent<Cry::DefaultComponents::CAdvancedAnimationComponent>();
	m_pAnimationComponent->SetMannequinAnimationDatabaseFile("Animations/Mannequin/ADB/FirstPerson.adb");
	m_pAnimationComponent->SetCharacterFile("Objects/Characters/SampleCharacter/thirdperson.cdf");
	m_pAnimationComponent->SetControllerDefinitionFile("Animations/Mannequin/ADB/FirstPersonControllerDefinition.xml");
	m_pAnimationComponent->SetDefaultScopeContextName("FirstPersonCharacter");
	m_pAnimationComponent->SetDefaultFragmentName("Idle");
	m_pAnimationComponent->SetAnimationDrivenMotion(false);
	m_pAnimationComponent->LoadFromDisk();
	m_idleFragmentId = m_pAnimationComponent->GetFragmentId("Idle");
	m_walkFragmentId = m_pAnimationComponent->GetFragmentId("Walk");
	m_rotateTagId = m_pAnimationComponent->GetTagId("Rotate");

	Revive();
	m_pEntity->GetNetEntity()->EnableDelegatableAspect(eEA_Physics, false);
	bIsInitialized = true;
}

void CPlayerComponent::LocalPlayerInitialize() {

	//Init local player components
	m_pCameraComponent = m_pEntity->GetOrCreateComponent<Cry::DefaultComponents::CCameraComponent>();
	m_pInventoryComponent = m_pEntity->GetOrCreateComponentClass<CInventoryComponent>();
	//Initializes all the different inputs
	InitializeInput();
	//Set camera clipping
	m_pCameraComponent->SetNearPlane(0.1f);
}

uint64 CPlayerComponent::GetEventMask() const
{
	return BIT64(ENTITY_EVENT_START_GAME) | BIT64(ENTITY_EVENT_UPDATE) | BIT64(ENTITY_EVENT_TIMER) | BIT64(ENTITY_EVENT_NET_BECOME_LOCAL_PLAYER);
}

void CPlayerComponent::ProcessEvent(SEntityEvent& event)
{
	switch (event.event)
	{

	case ENTITY_EVENT_START_GAME:
	{
		// Revive the entity when gameplay starts
		if(gEnv->IsEditor())
			Revive();
	}
	break;

	case ENTITY_EVENT_NET_BECOME_LOCAL_PLAYER:
	{
		LocalPlayerInitialize();
	}
	break;

	case ENTITY_EVENT_TIMER:

		if (event.nParam[0] == Timer_jumpTime) {
			jumpVel = 0.f;
		}

		break;

	case ENTITY_EVENT_UPDATE:
	{
		SEntityUpdateContext* pCtx = (SEntityUpdateContext*)event.nParam[0];

		if (gEnv->bServer)
			UpdateMovementRequest(pCtx->fFrameTime);
		
		if (!gEnv->bServer)
			UpdateLookDirectionRequest(pCtx->fFrameTime);
		
		UpdateAnimation(pCtx->fFrameTime);

		if (m_pCameraComponent) {

			if (bIsTP)
				UpdateCamera(pCtx->fFrameTime);
			else
				UpdateFPCamera(pCtx->fFrameTime);

			Update(pCtx->fFrameTime);
		}
	}
	break;
	}
}

void CPlayerComponent::ReflectType(Schematyc::CTypeDesc<CPlayerComponent>& desc) {

	desc.SetGUID("{63F4C0C6-32AF-4ACB-8FB0-57D45DD14725}"_cry_guid);
	desc.SetEditorCategory("Players");
	desc.SetLabel("Player Component");
	desc.SetDescription("Player component");
	desc.SetComponentFlags({ IEntityComponent::EFlags::Transform, IEntityComponent::EFlags::Socket, IEntityComponent::EFlags::Attach });

}

void CPlayerComponent::SetPlayerParams() {

	//Health values
	m_pHealthComponent->SetMax(100.f);
	m_pHealthComponent->SetRegenerationRatio(0.02f);

}

void CPlayerComponent::Revive()
{
	SpawnAtSpawnPoint();
	GetEntity()->Hide(false);
	GetEntity()->SetWorldTM(Matrix34::Create(Vec3(1, 1, 1), IDENTITY, GetEntity()->GetWorldPos()));

	if (m_pAnimationComponent && m_pCharacterController) {

		m_pAnimationComponent->ResetCharacter();
		m_pCharacterController->Physicalize();

	}

	m_pEntity->FreeSlot(1);

	m_inputFlags = 0;
	m_mouseDeltaRotation = ZERO;
	m_mouseDeltaSmoothingFilter.Reset();

	m_activeFragmentId = FRAGMENT_ID_INVALID;

	m_lookOrientation = IDENTITY;
	m_horizontalAngularVelocity = 0.0f;
	m_averagedHorizontalAngularVelocity.Reset();


}

void CPlayerComponent::SpawnAtSpawnPoint()
{
	// We only handle default spawning below for the Launcher
	// Editor has special logic in CEditorGame
	if (gEnv->IsEditor())
		return;

	// Spawn at first default spawner
	auto *pEntityIterator = gEnv->pEntitySystem->GetEntityIterator();
	pEntityIterator->MoveFirst();

	while (!pEntityIterator->IsEnd())
	{
		IEntity *pEntity = pEntityIterator->Next();

		if (auto* pSpawner = pEntity->GetComponent<CSpawnPointComponent>())
		{
			pSpawner->SpawnEntity(m_pEntity);
			break;
		}
	}
}

void CPlayerComponent::PickUp(SItemComponent *pNewItem) {

	CInventoryComponent InventoryComponent;

	if (!pNewItem)
		return;

	EntityId playerId = m_pEntity->GetId();

	if (pNewItem->GetItemType() == 5) {
		if (GetInventory()->iHealthPackAmount < HEALTH_PACK_CAPACITY) {

			if (GetInventory()->AddItem(pNewItem))

				pNewItem->PickUp(playerId);

		} else {

			CryLogAlways("You're full!");

		}
	} else if (pNewItem->GetItemType() == 2 || 4) {

		if (GetInventory()->AddItemQuickAccess(pNewItem)) {

			pNewItem->PickUp(playerId);

			for (int i = 0; i < WEAPON_CAPACITY; i++) {

				pNewItem->GetEntity()->DetachThis();
				AttachToBack(pNewItem, i);

			}

		}
	}
	else {

		if (GetInventory()->AddItem(pNewItem)) {

			pNewItem->PickUp(playerId);

		}

	}
}

void CPlayerComponent::AttachToBack(SItemComponent *pWeaponToAttach, int slotId) {

	string slotString = ToString(slotId);
	CPlayerComponent *player = m_pEntity->GetComponent<CPlayerComponent>();

	CEntityAttachment *attachmentItem = new CEntityAttachment();
	attachmentItem->SetEntityId(pWeaponToAttach->GetEntityId());

	if (ICharacterInstance *pCharacter = player->GetAnimations()->GetCharacter()) {

		if (IAttachmentManager *pAttMan = pCharacter->GetIAttachmentManager()) {

			string attName = "back_att_0" + slotString;
			if (IAttachment *pAttachment = pAttMan->GetInterfaceByName(attName)) {

				pAttachment->AddBinding(attachmentItem);

			}

		}

	}

}
