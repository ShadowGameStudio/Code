#include "StdAfx.h"
#include "Player.h"
#include "Bullet.h"
#include "ItemComponent.h"
#include "HealthpackComponent.h"
#include "PlayAreaComponent.h"

void CPlayerComponent::InitializeInput() {

	// Register an action, and the callback that will be sent when it's triggered
	m_pInputComponent->RegisterAction("player", "moveleft", [this](int activationMode, float value) { HandleInputFlagChange((TInputFlags)EInputFlag::MoveLeft, activationMode);  });
	// Bind the 'A' key the "moveleft" action
	m_pInputComponent->BindAction("player", "moveleft", eAID_KeyboardMouse, EKeyId::eKI_A);

	m_pInputComponent->RegisterAction("player", "moveright", [this](int activationMode, float value) { HandleInputFlagChange((TInputFlags)EInputFlag::MoveRight, activationMode);  });
	m_pInputComponent->BindAction("player", "moveright", eAID_KeyboardMouse, EKeyId::eKI_D);

	m_pInputComponent->RegisterAction("player", "moveforward", [this](int activationMode, float value) { HandleInputFlagChange((TInputFlags)EInputFlag::MoveForward, activationMode);  });
	m_pInputComponent->BindAction("player", "moveforward", eAID_KeyboardMouse, EKeyId::eKI_W);

	m_pInputComponent->RegisterAction("player", "moveback", [this](int activationMode, float value) { HandleInputFlagChange((TInputFlags)EInputFlag::MoveBack, activationMode);  });
	m_pInputComponent->BindAction("player", "moveback", eAID_KeyboardMouse, EKeyId::eKI_S);

	m_pInputComponent->RegisterAction("player", "movesprint", [this](int activationMode, float value) { HandleInputFlagChange((TInputFlags)EInputFlag::MoveSprint, activationMode);  });
	m_pInputComponent->BindAction("player", "movesprint", eAID_KeyboardMouse, EKeyId::eKI_LShift);

	m_pInputComponent->RegisterAction("player", "movejump", [this](int activationMode, float value) { HandleInputFlagChange((TInputFlags)EInputFlag::MoveJump, activationMode); });
	m_pInputComponent->BindAction("player", "movejump", eAID_KeyboardMouse, EKeyId::eKI_Space);

	m_pInputComponent->RegisterAction("player", "mouse_rotateyaw", [this](int activationMode, float value) 
	{
		if (bFreezePlayer) return; m_mouseDeltaRotation.x -= value;
		NetMarkAspectsDirty(kInputAspect);
	});
	m_pInputComponent->BindAction("player", "mouse_rotateyaw", eAID_KeyboardMouse, EKeyId::eKI_MouseX);

	m_pInputComponent->RegisterAction("player", "mouse_rotatepitch", [this](int activationMode, float value) 
	{ 
		if (bFreezePlayer) return;  m_mouseDeltaRotation.y -= value; 
		NetMarkAspectsDirty(kInputAspect);
	});
	m_pInputComponent->BindAction("player", "mouse_rotatepitch", eAID_KeyboardMouse, EKeyId::eKI_MouseY);

	m_pInputComponent->RegisterAction("player", "use", [this](int activationMode, float value) { Action_Use(activationMode); });
	m_pInputComponent->BindAction("player", "use", eAID_KeyboardMouse, EKeyId::eKI_F);

	m_pInputComponent->RegisterAction("player", "inv_toggle", [this](int activationMode, float value) { Action_InventoryToggle(activationMode); });
	m_pInputComponent->BindAction("player", "inv_toggle", eAID_KeyboardMouse, EKeyId::eKI_I);

	m_pInputComponent->RegisterAction("player", "dI", [this](int activationMode, float value) { Action_DI(activationMode); });
	m_pInputComponent->BindAction("player", "dI", eAID_KeyboardMouse, EKeyId::eKI_L);

	m_pInputComponent->RegisterAction("player", "select_item_0", [this](int activationMode, float value) { Action_SelectSlot(activationMode, 0); });
	m_pInputComponent->BindAction("player", "select_item_0", eAID_KeyboardMouse, EKeyId::eKI_1);

	m_pInputComponent->RegisterAction("player", "select_item_1", [this](int activationMode, float value) { Action_SelectSlot(activationMode, 1); });
	m_pInputComponent->BindAction("player", "select_item_1", eAID_KeyboardMouse, EKeyId::eKI_2);
	
	m_pInputComponent->RegisterAction("player", "tp", [this](int activationMode, float value) { Action_TP(activationMode); });
	m_pInputComponent->BindAction("player", "tp", eAID_KeyboardMouse, EKeyId::eKI_F1);

	m_pInputComponent->RegisterAction("player", "leanLeft", [this](int activationMode, float value) {Action_LeanLeft(activationMode); });
	m_pInputComponent->BindAction("player", "leanLeft", eAID_KeyboardMouse, EKeyId::eKI_Q);

	m_pInputComponent->RegisterAction("player", "leanRight", [this](int activationMode, float value) {Action_LeanRight(activationMode); });
	m_pInputComponent->BindAction("player", "leanRight", eAID_KeyboardMouse, EKeyId::eKI_E);

	m_pInputComponent->RegisterAction("player", "attack", [this](int activationMode, float value) {Action_Attack(activationMode); });
	m_pInputComponent->BindAction("player", "attack", eAID_KeyboardMouse, EKeyId::eKI_Mouse1);

	m_pInputComponent->RegisterAction("player", "heal", [this](int activationMode, float value) {Action_Heal(activationMode); });
	m_pInputComponent->BindAction("player", "heal", eAID_KeyboardMouse, EKeyId::eKI_H);

	m_pInputComponent->RegisterAction("player", "spawnDome", [this](int activationMode, float value) { Action_SpawnDome(activationMode); });
	m_pInputComponent->BindAction("player", "spawnDome", eAID_KeyboardMouse, EKeyId::eKI_O);

}

void CPlayerComponent::HandleInputFlagChange(TInputFlags flags, int activationMode, EInputFlagType type) {
	
	if (bFreezePlayer)
		return;

	switch (type) {
	case EInputFlagType::Hold:
	{
		if (activationMode == eIS_Released) {
			m_inputFlags &= ~flags;
		}
		else {
			m_inputFlags |= flags;
		}
	}
	break;
	case EInputFlagType::Toggle:
	{
		if (activationMode == eIS_Released) {
			// Toggle the bit(s)
			m_inputFlags ^= flags;
		}
	}
	break;
	}

	if (!gEnv->bServer) {
		NetMarkAspectsDirty(kInputAspect);
	}

}

bool CPlayerComponent::NetSerialize(TSerialize ser, EEntityAspects aspect, uint8 profile, int flags) {

	if (aspect == kInputAspect) {
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

	return true;
}

void CPlayerComponent::Action_Use(int activationMode) {

	if (bFreezePlayer)
		return;
	if (activationMode == eIS_Pressed) {

		if (pTargetItem)
			PickUp(pTargetItem);

	}

}

void CPlayerComponent::Action_InventoryToggle(int activationMode) {

	if (activationMode == eIS_Pressed) {

		GetInventory()->bIsInventoryOpened = !GetInventory()->bIsInventoryOpened;
		GetInventory()->Show();

	}

}

void CPlayerComponent::Action_DI(int activationMode) {

	if (bFreezePlayer)
		return;
	if (activationMode == eIS_Pressed) {
			if (SItemComponent *pSelectedItem = GetInventory()->GetSelectedWeapon())
				pSelectedItem->Drop();

	}

}

void CPlayerComponent::Action_SelectSlot(int activationMode, int slotId) {

	if (bFreezePlayer)
		return;
	if (activationMode == eIS_Pressed) {

 		GetInventory()->SelectSlot(slotId);

	}

}

void CPlayerComponent::Action_TP(int activationMode) {

	if (bFreezePlayer)
		return;

	if (activationMode == eIS_Pressed) {
	
		if (bIsTP)
			bIsTP = false;
		else
			bIsTP = true;

	}

}

void CPlayerComponent::Action_LeanRight(int activationMode) {

	if (bFreezePlayer)
		return;

	//Checks if player is in third person
	//If not, it will lean the player
	if (!bIsTP) {
		if (activationMode == eIS_Pressed) {
			//If it is leaned and to the left it will lean back
			if (bIsLeaned && bIsLeanedLeft) {

				fLeanDown = 0.5f;
				fLeanSide = 0.0f;

				bIsLeanedLeft = false;
				bIsLeaned = false;
			
			
			}
			//Else if it is none it will lean it to the right
			else if(!bIsLeanedLeft && bIsLeaned){
		
				fLeanDown = 0.4f;
				fLeanSide = 0.2f;

				bIsLeanedRight = false;
				bIsLeanedLeft = true;
				bIsLeaned = true;
		
			} 
			//Else it will just lean to the rigth
			else {

				fLeanDown = 0.4f;
				fLeanSide = 0.2f;

				bIsLeanedRight = false;
				bIsLeanedLeft = true;
				bIsLeaned = true;

			}
		}
	}
}

void CPlayerComponent::Action_LeanLeft(int activationMode) {

	if (bFreezePlayer)
		return;
	//As in lean right
	if (!bIsTP) {
		if (activationMode == eIS_Pressed) {
			if (bIsLeaned && bIsLeanedRight) {

				fLeanDown = 0.5f;
				fLeanSide = 0.0f;

				bIsLeanedRight = false;
				bIsLeaned = false;
		
			} 
			else if(!bIsLeanedRight && bIsLeaned){
			
				fLeanDown = 0.4f;
				fLeanSide = -0.2f;
		
				bIsLeanedLeft = false;
				bIsLeanedRight = true;
				bIsLeaned = true;
		
			} 
			else {

				fLeanDown = 0.4f;
				fLeanSide = -0.2f;

				bIsLeanedLeft = false;
				bIsLeanedRight = true;
				bIsLeaned = true;
			}
		}
	}
}

void CPlayerComponent::Action_Attack(int activationMode) {
			
	if (SItemComponent *pSelectedItem = m_pInventoryComponent->GetSelectedWeapon()) {
		if (CWeaponComponent *pSelectedWeapon = pSelectedItem->GetEntity()->GetComponent<CWeaponComponent>()) {
			//Check if weapon is meele
			if (pSelectedWeapon->GetIsMeele()) {
				
				if (bFreezePlayer || activationMode == eIS_Down)
					return;
				
				else if (activationMode == eIS_Pressed) {
					pSelectedWeapon->StartAttack();
				}
				else if (activationMode == eIS_Released) {
					pSelectedWeapon->StopAttack();
				}
			}
			//Else if weapon is non-meele
			else {

				CPlayerComponent *pPlayer = m_pEntity->GetComponent<CPlayerComponent>();

				//If firemode is automatic
				if (pSelectedWeapon->GetFiremodeProperties()->bIsAuto) {
					while (activationMode == eIS_Down) {
						//If weapon has ammo, shoot
						if (pSelectedWeapon->GetCurrAmmo() >= 1) {
							pSelectedWeapon->SetPlayer(pPlayer);
							pSelectedWeapon->Shoot();
						}
						//else reload
						else {
							pSelectedWeapon->Reload();
						}
					}
				}
				//If firemode is semi-auto or single, because they do the same thing
				else if (pSelectedWeapon->GetFiremodeProperties()->bIsSemi || pSelectedWeapon->GetFiremodeProperties()->bIsSingle) {
					if (activationMode == eIS_Pressed) {
						if (pSelectedWeapon->GetCurrAmmo() >= 1) {
							pSelectedWeapon->SetPlayer(pPlayer);
							pSelectedWeapon->Shoot();
						}
						else {
							pSelectedWeapon->Reload();
						}
					}
				}
				else
					return;
			}
		}	
	}
}

void CPlayerComponent::Action_Heal(int activationMode) { 

	if (bFreezePlayer)
		return;

	if (m_pInventoryComponent->iHealthPackAmount >= 1) {

		//fix this!!!

		m_pInventoryComponent->iHealthPackAmount -= 1;

		CryLogAlways("Amount %i", m_pInventoryComponent->iHealthPackAmount);

	} else {
		CryLogAlways("YOU HAVE NO PACKS!");
	}

}

void CPlayerComponent::Action_SpawnDome(int activationMode) {

	if (bFreezePlayer)
		return;

	CPlayAreaComponent PA;

	if (activationMode == eIS_Pressed)
		PA.SpawnPlayArea();

}
