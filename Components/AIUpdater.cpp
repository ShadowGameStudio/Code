#include "StdAfx.h"
#include "AIComponent.h"

void CAIComponent::UpdateMovementRequest(float frameTime) {

	//Create a Vec3
	Vec3 velocity = ZERO;

	if (m_bTimerSet == false) {
		m_pEntity->SetTimer(Timer_Speed, 5000);
		m_bTimerSet = true;
	}


	//Adds these two together
	velocity.y += m_fMoveSpeed * frameTime;
	//Pushes the movement
	m_pAIController->AddVelocity(GetEntity()->GetWorldRotation() * velocity);

}

void CAIComponent::UpdateAnimation(float frameTime) {

	const float angularVelocityTurningThreshold = 0.174; // [rad/s]

														 // Update tags and motion parameters used for turning
	const bool isTurning = std::abs(m_averagedHorizontalAngularVelocity.Get()) > angularVelocityTurningThreshold;
	m_pAnimationComponent->SetTagWithId(m_rotateTagId, isTurning);
	if (isTurning) {
		// TODO: This is a very rough predictive estimation of eMotionParamID_TurnAngle that could easily be replaced with accurate reactive motion 
		// if we introduced IK look/aim setup to the character's model and decoupled entity's orientation from the look direction derived from mouse input.

		const float turnDuration = 1.0f; // Expect the turning motion to take approximately one second.
		m_pAnimationComponent->SetMotionParameter(eMotionParamID_TurnAngle, m_horizontalAngularVelocity * turnDuration);
	}

	// Update active fragment
	const auto& desiredFragmentId = m_pAIController->IsWalking() ? m_walkFragmentId : m_idleFragmentId;
	if (m_activeFragmentId != desiredFragmentId) {
		m_activeFragmentId = desiredFragmentId;
		m_pAnimationComponent->QueueFragmentWithId(m_activeFragmentId);
	}

}

void CAIComponent::UpdateVicinity(float frameTime) {

	Vec3 AIPos = m_pEntity->GetWorldPos() + (m_pEntity->GetForwardDir() * PERCEPTION_RANGE);
	IPhysicalEntity **pEntityList = NULL;
	int num = gEnv->pEntitySystem->GetPhysicalEntitiesInBox(AIPos, PERCEPTION_RANGE, pEntityList);

	float fLastDist = 100.f;
	float fCurrDist = 0.f;
	CPlayerComponent *pPlayerWithinVicinity = nullptr;

	//Check every entity in the vicinity
	for (int i = 0; i < num; i++) {
		//If it can set pPhys to the entity, continue
		if (IPhysicalEntity *pPhys = pEntityList[i]) {
			//If it can get the entity itself, continue
			if (IEntity *pInteractingEntity = gEnv->pEntitySystem->GetEntityFromPhysics(pPhys)) {
				//If it can get the player from the entity, continue
				if (CPlayerComponent *pPlayer = pInteractingEntity->GetComponent<CPlayerComponent>()) {

					const Vec3 playerPos = pInteractingEntity->GetWorldPos();
					const Vec3 diff = playerPos - m_pEntity->GetWorldPos();

					fCurrDist = sqrt(powf(diff.x, 2.f) + (diff.y, 2.f));

					if (fCurrDist < fLastDist) {

						fLastDist = fCurrDist;
						m_fDistanceFromPlayer = fCurrDist;

						Vec3 heading = (m_pEntity->GetWorldPos() - pPlayer->GetEntity()->GetWorldPos()).normalized();

						m_lookOrientation = (Quat)heading;

						// Update entity rotation as the player turns
						// We only want to affect Z-axis rotation, zero pitch and roll 
						Ang3 ypr = m_pAIController->CreateAnglesYPR(Matrix33(m_lookOrientation));
						ypr.y = 0;
						ypr.z = 0;
						if (m_bGameStarted) {
							const Quat correctedOrientation = Quat(m_pAIController->CreateOrientationYPR(ypr));
							// Send updated transform to the entity, only orientation changes
							GetEntity()->SetPosRotScale(GetEntity()->GetWorldPos(), correctedOrientation, Vec3(1, 1, 1));
						}

					}

				}

			}

		}

	}

	pPlayerWithinVec = pPlayerWithinVicinity;

}

void CAIComponent::UpdateLookOrientation(float frameTime) {

	Ang3 ypr = m_pAIController->CreateAnglesYPR(Matrix33(m_lookOrientation));

	//Yaw
	ypr.x;
	//Pitch
	ypr.y;

	//Roll (skip)
	ypr.z = 0;

	m_lookOrientation = Quat(m_pAIController->CreateOrientationYPR(ypr));

}

void CAIComponent::UpdateMode(float frameTime) {

	//If the distance to player is greaer than the chase range, continue
	if (m_fDistanceFromPlayer >= CHASE_RANGE) {
		//Start looking at player
		AIMode = eAIM_Idle;
	}
	//Else if the distance to player is less than chase range and greater than attack range, continue
	else if(m_fDistanceFromPlayer <= CHASE_RANGE && m_fDistanceFromPlayer > ATTACK_RANGE){
		//Start chasing player
		AIMode = eAIM_Chasing;
	}
	//Else if distance to player is less than attack range, continue
	else if (m_fDistanceFromPlayer <= ATTACK_RANGE) {
		//Start attacking player
		AIMode = eAIM_Attacking;
	}

}
