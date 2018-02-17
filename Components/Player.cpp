#include "StdAfx.h"
#include "Player.h"

#include "Bullet.h"
#include "SpawnPoint.h"
#include "FlashUI\FlashUI.h"

#include <CryRenderer/IRenderAuxGeom.h>
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

	if (gEnv->IsEditor()) {

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
	
	}

	if(gEnv->IsEditor())
		Revive();

	m_pEntity->GetNetEntity()->EnableDelegatableAspect(eEA_Physics, false);
}

uint64 CPlayerComponent::GetEventMask() const
{
	return BIT64(ENTITY_EVENT_START_GAME) | BIT64(ENTITY_EVENT_UPDATE) | BIT64(ENTITY_EVENT_TIMER) | BIT64(ENTITY_EVENT_NET_BECOME_LOCAL_PLAYER);
}

void CPlayerComponent::ProcessEvent(SEntityEvent& event)
{
	switch (event.event)
	{

	case ENTITY_EVENT_NET_BECOME_LOCAL_PLAYER:
	{
		LocalPlayerInitialize();
	}
	break;

	case ENTITY_EVENT_START_GAME:
	{
		// Revive the entity when gameplay starts
		if(gEnv->IsEditor())
			Revive();
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

		if (m_pCameraComponent && m_pAnimationComponent && m_pCharacterController) {

		if(gEnv->bServer)
			UpdateMovementRequest(pCtx->fFrameTime);
		
		if(!gEnv->bServer)
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

		if (!bIsInitialized) {
			if (!gEnv->IsEditor()) {

				if (!gEnv->bServer)
					m_pCameraComponent = m_pEntity->GetComponent<Cry::DefaultComponents::CCameraComponent>();
				
				m_pCharacterController = m_pEntity->GetComponent<Cry::DefaultComponents::CCharacterControllerComponent>();
				m_pAnimationComponent = m_pEntity->GetComponent<Cry::DefaultComponents::CAdvancedAnimationComponent>();

				if (m_pCharacterController)
					m_pCharacterController->SetTransformMatrix(Matrix34::Create(Vec3(1.f), IDENTITY, Vec3(0, 0, 1.f)));

				if (m_pAnimationComponent) {

					m_pAnimationComponent->LoadFromDisk();
					m_idleFragmentId = m_pAnimationComponent->GetFragmentId("Idle");
					m_walkFragmentId = m_pAnimationComponent->GetFragmentId("Walk");
					m_rotateTagId = m_pAnimationComponent->GetTagId("Rotate");

				}

				if (m_pCameraComponent && m_pAnimationComponent && m_pCharacterController) {
					bIsInitialized = true;
					Revive();
				}

			}
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

	if (pNewItem->GetItemType() == 5) {
		if (GetInventory()->iHealthPackAmount < HEALTH_PACK_CAPACITY) {

			if (GetInventory()->AddItem(pNewItem))
				pNewItem->PickUp(m_pEntity);

		} else {

			CryLogAlways("You're full!");

		}
	} else if (pNewItem->GetItemType() == 2 || 4) {

		if (GetInventory()->AddItemQuickAccess(pNewItem)) {

			pNewItem->PickUp(m_pEntity);

			for (int i = 0; i < WEAPON_CAPACITY; i++) {

				pNewItem->GetEntity()->DetachThis();
				AttachToBack(pNewItem, i);

			}

		}
	}
	else {

		if (GetInventory()->AddItem(pNewItem)) {

			pNewItem->PickUp(m_pEntity);

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

bool CPlayerComponent::NetSerialize(TSerialize ser, EEntityAspects aspect, uint8 profile, int flags) {
	
	if (aspect == kMovementAspect) {

		ser.BeginGroup("PlayerInput");

		auto inputs = m_inputFlags;
		auto prevState = m_inputFlags;

		ser.Value("m_inputFlags", m_inputFlags, 'ui8');

		if (ser.IsReading()) {

			auto changedKeys = inputs ^ m_inputFlags;
			auto pressedKeys = changedKeys & inputs;

			if (pressedKeys != 0) {
				HandleInputFlagChange(pressedKeys, eIS_Pressed);
			}

			auto releasedKeys = changedKeys & prevState;
			if (releasedKeys != 0) {
				HandleInputFlagChange(pressedKeys, eIS_Released);
			}
		}

		ser.Value("m_lookOrientation", m_lookOrientation, 'ori3');
		ser.EndGroup();

	}
	if (aspect == kRotationAspect) {

		CryLogAlways("Player is rotating");
		ser.BeginGroup("PlayerRotation");
		ser.Value("m_lookOrientation", m_lookOrientation, 'ori3');
		ser.EndGroup();

	}
	return true;
}

void CPlayerComponent::LocalPlayerInitialize() {

	m_pCameraComponent = m_pEntity->GetOrCreateComponent<Cry::DefaultComponents::CCameraComponent>();
	m_pInventoryComponent = m_pEntity->GetOrCreateComponentClass<CInventoryComponent>();
	InitializeInput(); 
}